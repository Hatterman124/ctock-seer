#include "struc.h"
#include <pybind11/embed.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <cmath>

bool is_date_format(std::string s)
{
	// Absolute garbage. Looking at this makes me want to throw up.
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
	else
		return false;
}

void python_get_df(std::vector<std::string>& cmd, database& db)
{
	pybind11::size_t                        n_of_row {};
	const std::vector<dataframe>::size_type df_index {db.df.size()};
	std::istringstream                      date {};
	pybind11::size_t i_m {};

	if (!is_date_format(cmd[2]) || !is_date_format(cmd[3])) {
		std::cout << "Date format is incorrect\n";
		return;
	}

	pybind11::object pd_df {
		pybind11::module_::import(
			"yfinance"
		).attr("Ticker")(cmd[1]).attr("history")(
			pybind11::arg("start") = cmd[2],
			pybind11::arg("end")   = cmd[3]
		)
	};
	n_of_row = pybind11::len(pd_df);
	if (n_of_row == 0) {
		std::cout << "Table is empty\nReturning\n";
		return;
	}
	db.ticker.push_back(cmd[1]);
	db.df.resize(df_index + 1);
	db.df[df_index].sr.resize(n_of_row);
	db.df[df_index].sr_dif.resize(n_of_row - 1);
	db.df[df_index].sr_per.resize(n_of_row - 1);
	db.df[df_index].sr_log.resize(n_of_row - 1);
	db.df[df_index].sr_up.resize(n_of_row - 1);

	//Convert Python pandas dataframe into c++ dataframe
	date.str(static_cast<std::string>(pybind11::str(
		pd_df.attr("iloc")[pybind11::cast(0)].attr("name")
	)));
	date >> parse("%Y-%m-%d" ,db.df[df_index].sr[0].date);
	db.df[df_index].sr[0].open        = pybind11::float_(
		pd_df.attr("iat")[pybind11::make_tuple(0, 0)]
	);
	db.df[df_index].sr[0].high        = pybind11::float_(
		pd_df.attr("iat")[pybind11::make_tuple(0, 1)]
	);
	db.df[df_index].sr[0].low         = pybind11::float_(
		pd_df.attr("iat")[pybind11::make_tuple(0, 2)]
	);
	db.df[df_index].sr[0].close       = pybind11::float_(
		pd_df.attr("iat")[pybind11::make_tuple(0, 3)]
	);
	db.df[df_index].sr[0].volume      = pybind11::int_(
		pd_df.attr("iat")[pybind11::make_tuple(0, 4)]
	);
	db.df[df_index].sr[0].dividends   = pybind11::float_(
		pd_df.attr("iat")[pybind11::make_tuple(0, 5)]
	);
	db.df[df_index].sr[0].stocksplits = pybind11::float_(
		pd_df.attr("iat")[pybind11::make_tuple(0, 6)]
	);
	db.df[df_index].sr[0].index = 0;
	for (pybind11::size_t i {1}; i < n_of_row; ++i) {
		i_m = i - 1;

		// Date
		date.str(static_cast<std::string>(pybind11::str(
			pd_df.attr("iloc")[pybind11::cast(i)].attr("name")
		)));
		date >> parse("%Y-%m-%d" ,db.df[df_index].sr[i].date);
		db.df[df_index].sr_dif[i_m].date = db.df[df_index].sr[i].date;
		db.df[df_index].sr_per[i_m].date = db.df[df_index].sr[i].date;
		db.df[df_index].sr_log[i_m].date = db.df[df_index].sr[i].date;
		db.df[df_index].sr_up[i_m].date = db.df[df_index].sr[i].date;

		// Open
		db.df[df_index].sr[i].open        = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 0)]
		);
		db.df[df_index].sr_dif[i_m].open = db.df[df_index].sr[i].open -
		                                   db.df[df_index].sr[i_m].open;
		db.df[df_index].sr_per[i_m].open
		= db.df[df_index].sr_dif[i_m].open /
		  db.df[df_index].sr[i_m].open;
		db.df[df_index].sr_log[i_m].open
		= std::log(db.df[df_index].sr[i].open /
		           db.df[df_index].sr[i_m].open);
		if (db.df[df_index].sr_dif[i_m].open > 0)
			db.df[df_index].sr_up[i_m].open = true;

		// High
		db.df[df_index].sr[i].high        = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 1)]
		);
		db.df[df_index].sr_dif[i_m].high = db.df[df_index].sr[i].high -
		                                   db.df[df_index].sr[i_m].high;
		db.df[df_index].sr_per[i_m].high
		= db.df[df_index].sr_dif[i_m].high /
		  db.df[df_index].sr[i_m].high;
		db.df[df_index].sr_log[i_m].high
		= std::log(db.df[df_index].sr[i].high /
		           db.df[df_index].sr[i_m].high);
		if (db.df[df_index].sr_dif[i_m].high > 0)
			db.df[df_index].sr_up[i_m].high = true;

		// Low
		db.df[df_index].sr[i].low         = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 2)]
		);
		db.df[df_index].sr_dif[i_m].low = db.df[df_index].sr[i].low -
		                                  db.df[df_index].sr[i_m].low;
		db.df[df_index].sr_per[i_m].low
		= db.df[df_index].sr_dif[i_m].low /
		  db.df[df_index].sr[i_m].low;
		db.df[df_index].sr_log[i_m].low
		= std::log(db.df[df_index].sr[i].low /
		           db.df[df_index].sr[i_m].low);
		if (db.df[df_index].sr_dif[i_m].low > 0)
			db.df[df_index].sr_up[i_m].low = true;

		// Close
		db.df[df_index].sr[i].close       = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 3)]
		);
		db.df[df_index].sr_dif[i_m].close
		= db.df[df_index].sr[i].close -
		  db.df[df_index].sr[i_m].close;
		db.df[df_index].sr_per[i_m].close
		= db.df[df_index].sr_dif[i_m].close /
		  db.df[df_index].sr[i_m].close;
		db.df[df_index].sr_log[i_m].close
		= std::log(db.df[df_index].sr[i].close /
		           db.df[df_index].sr[i_m].close);
		if (db.df[df_index].sr_dif[i_m].close > 0)
			db.df[df_index].sr_up[i_m].close = true;

		// Volume
		db.df[df_index].sr[i].volume      = pybind11::int_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 4)]
		);
		db.df[df_index].sr_dif[i_m].volume
		= db.df[df_index].sr[i].volume -
		  db.df[df_index].sr[i_m].volume;
		db.df[df_index].sr_per[i_m].volume
		= static_cast<float>(db.df[df_index].sr_dif[i_m].volume) /
		  static_cast<float>(db.df[df_index].sr[i_m].volume);
		db.df[df_index].sr_log[i_m].volume
		= std::log(static_cast<float>(db.df[df_index].sr[i].volume) /
		           static_cast<float>(db.df[df_index].sr[i_m].volume));
		if (db.df[df_index].sr_dif[i_m].volume > 0)
			db.df[df_index].sr_up[i_m].volume = true;

		// Dividends
		db.df[df_index].sr[i].dividends   = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 5)]
		);
		db.df[df_index].sr_dif[i_m].dividends
		= db.df[df_index].sr[i].dividends -
		  db.df[df_index].sr[i_m].dividends;
		db.df[df_index].sr_per[i_m].dividends
		= db.df[df_index].sr_dif[i_m].dividends /
		  db.df[df_index].sr[i_m].dividends;
		db.df[df_index].sr_log[i_m].dividends
		= std::log(db.df[df_index].sr[i].dividends /
		           db.df[df_index].sr[i_m].dividends);
		if (db.df[df_index].sr_dif[i_m].dividends > 0)
			db.df[df_index].sr_up[i_m].dividends = true;

		// Stocksplits
		db.df[df_index].sr[i].stocksplits = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 6)]
		);
		db.df[df_index].sr_dif[i_m].stocksplits
		= db.df[df_index].sr[i].stocksplits -
		  db.df[df_index].sr[i_m].stocksplits;
		db.df[df_index].sr_per[i_m].stocksplits
		= db.df[df_index].sr_dif[i_m].stocksplits /
		  db.df[df_index].sr[i_m].stocksplits;
		db.df[df_index].sr_log[i_m].stocksplits
		= std::log(db.df[df_index].sr[i].stocksplits /
		           db.df[df_index].sr[i_m].stocksplits);
		if (db.df[df_index].sr_dif[i_m].stocksplits > 0)
			db.df[df_index].sr_up[i_m].stocksplits = true;

		// Index
		// I'm going to kill myself. I don't remember making this.
		db.df[df_index].sr[i].index
		= std::chrono::duration_cast<std::chrono::days>(
			std::chrono::sys_days(db.df[df_index].sr[i    ].date) -
			std::chrono::sys_days(db.df[df_index].sr[i - 1].date)
		  ).count()
		  + db.df[df_index].sr[i - 1].index;
		db.df[df_index].sr_dif[i_m].index
		= db.df[df_index].sr[i].index -
		  db.df[df_index].sr[1].index;
		db.df[df_index].sr_per[i_m].index
		= db.df[df_index].sr_dif[i_m].index;
		db.df[df_index].sr_log[i_m].index
		= db.df[df_index].sr_dif[i_m].index;
		db.df[df_index].sr_up[i_m].index
		= db.df[df_index].sr_dif[i_m].index;
	}
	db.df[df_index].start_date = db.df[df_index].sr[0].date;
	db.df[df_index].end_date   = db.df[df_index].sr[n_of_row - 1].date;
	PyDict_Clear(PyModule_GetDict(PyImport_AddModule("__main__")));

	return;
}