#include "tdatabase.h"
#include "getw.h"
#include <pybind11/embed.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

tdataframe::tdataframe(const std::string &ticker,
                       std::istringstream date_beg,
                       std::istringstream date_end ) : ticker {ticker}
{
	date_beg >> std::chrono::parse("%Y-%m-%d", tdataframe::date_beg);
	date_end >> std::chrono::parse("%Y-%m-%d", tdataframe::date_end);

	return;
}

void tdataframe::print([[maybe_unused]] const int precision) const
{
	// Precision is needed foe std::setw() in the future when
	// multiple floats are outputed.

	const int vindex_w {
		getw<std::vector<stockrow>::size_type>(
			tdataframe::sr.size() - 1uz
		)
	};
	const int index_w {
		getw<std::vector<stockrow>::size_type>(
			tdataframe::sr.size() - 1uz
		)
	};

	for (std::vector<stockrow>::size_type i {};
	     i < tdataframe::sr.size();
	     ++i)
		std::cout << std::setw(vindex_w)
		          << i << ": "
		          << std::setw(index_w)
		          << tdataframe::sr[i].index << ": "
		          << tdataframe::sr[i].date << " | "
		          << tdataframe::sr[i].close << '\n';

	return;
}

tdatabase::~tdatabase()
{
	for (tdataframe *tmp : tdatabase::df)
		delete tmp;

	return;
}

int tdatabase::get(const std::string &ticker,
                   const std::string &date_beg,
                   const std::string &date_end )
{
	if (!tdatabase::is_date_format(date_beg) ||
	    !tdatabase::is_date_format(date_end)   ) {
		std::cout << "Date is not in correct format\n";
		return 2;
	}

	pybind11::object df_pd {
		pybind11::module_::import(
			"yfinance"
		).attr("Ticker")(ticker).attr("history")(
			pybind11::arg("start") = date_beg,
			pybind11::arg("end")   = date_end
		)
	};

	pybind11::size_t n_row {pybind11::len(df_pd)};

	if (n_row == 0) {
		std::cout << "Table is empty\n";
		return 1;
	}

	/*
	 * 	df_pd.attr("iloc")[COORD].attr("name")
	 * is equivalent to
	 * 	df_pd.iloc[COORD]
	 * in Python.
	 * COORD can be type pybind11::int_ or pybind11::tuple.
	 * It needs to be casted into pybind11::str then std::string
	 * to be readable.
	 */

	tdatabase::df.push_back(new tdataframe {
		ticker,
		std::istringstream {static_cast<std::string>(
			static_cast<pybind11::str>(df_pd.attr("iloc")[
				PY_ZERO
			].attr("name"))
		)},
		std::istringstream {static_cast<std::string>(
			static_cast<pybind11::str>(df_pd.attr("iloc")[
				static_cast<pybind11::int_>(n_row - 1)
			].attr("name"))
		)}
	});

	// Convert Pandas dataframe into tdataframe.

	/*
	 * TO DO:
	 * Find a way to fix all these
	 * disgusting multi-level casting
	 */

	std::istringstream date_buf {};

	tdatabase::df[size]->sr.resize(n_row);
	// Date
	date_buf.str(static_cast<pybind11::str>(df_pd.attr("iloc")[
		PY_ZERO
	].attr("name")));
	date_buf >> std::chrono::parse("%Y-%m-%d",
	                               tdatabase::df[size]->sr[0].date);

	// Index
	tdatabase::df[size]->sr[PY_ZERO].index = 0;

	// Closing Price
	tdatabase::df[size]->sr[PY_ZERO].close = static_cast<std::float16_t>(
		static_cast<double>(
			static_cast<pybind11::float_>(
				df_pd.attr("iat")[
					pybind11::make_tuple(PY_ZERO, 3)
				]
			)
		)
	);
	for (pybind11::size_t i {1}; i < n_row; ++i) {
		// Date
		date_buf.str(static_cast<pybind11::str>(df_pd.attr("iloc")[
			static_cast<pybind11::int_>(i)
		].attr("name")));
		date_buf >> std::chrono::parse("%Y-%m-%d",
		                               tdatabase::df[size]->sr[i].date);

		// Index
		tdatabase::df[size]->sr[i].index =
			std::chrono::duration_cast<std::chrono::days>(
				std::chrono::sys_days(
					tdatabase::df[size]->sr[i    ].date
				) - std::chrono::sys_days(
					tdatabase::df[size]->sr[i - 1].date
				)
			).count() + tdatabase::df[size]->sr[i - 1].index;

		// Closing Price
		tdatabase::df[size]->sr[i].close = static_cast<std::float16_t>(
			static_cast<double>(
				static_cast<pybind11::float_>(
					df_pd.attr("iat")[
						pybind11::make_tuple(i, 3)
					]
				)
			)
		);
	}

	++tdatabase::size;

	return 0;
}

void tdatabase::print() const
{
	if (tdatabase::size == 0)
		return;

	const int width {
		getw<std::vector<tdataframe*>::size_type>(tdatabase::size - 1uz)
	};

	for (std::vector<tdataframe*>::size_type i {};
	     i < tdatabase::size;
	     ++i)
		std::cout << std::setw(width) << i << ": "
		          << tdatabase::df[i]->ticker << " | "
		          << tdatabase::df[i]->date_beg << " | "
		          << tdatabase::df[i]->date_end << " | size: "
		          << tdatabase::df[i]->sr.size() << '\n';

	return;
}

bool tdatabase::is_date_format(const std::string &s) const
{
	if (s.size() == 10
	    && s[0] >= '0' && s[0] <= '9'
	    && s[1] >= '0' && s[1] <= '9'
	    && s[2] >= '0' && s[2] <= '9'
	    && s[3] >= '0' && s[3] <= '9'
	    && s[4] == '-'
	    && s[5] >= '0' && s[5] <= '9'
	    && s[6] >= '0' && s[6] <= '9'
	    && s[7] == '-'
	    && s[8] >= '0' && s[8] <= '9'
	    && s[9] >= '0' && s[9] <= '9')
		return true;
	return false;
}