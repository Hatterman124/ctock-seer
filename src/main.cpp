#include "stockrow.h"
#include "manip.h"
#include "print.h"
#include <pybind11/embed.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

void python_get_df(std::vector<stockrow>& df,
                   const std::string stockname,
                   const std::string date0,
                   const std::string date1)
{
	pybind11::size_t n_of_row{};
	std::string date {};
	stockrow temprow {};

	/*
	 * Get Python pandas dataframe of stock data
	 *
	 * c++ equivalent of
	 * pd_df = yfinance.Ticker("STOCK").history(start='DATE0', end='DATE1')
	 */
	const pybind11::object pd_df {
		pybind11::module_::import(
			"yfinance"
		).attr("Ticker")(stockname).attr("history")(
			pybind11::arg("start") = date0,
			pybind11::arg("end")   = date1
		)
	};

	//Return if Ticker returned empty.
	n_of_row = pybind11::len(pd_df);
	if (n_of_row == 0) {
		return;
	}

	//Convert Python pandas dataframe into c++ dataframe
	for (pybind11::size_t i {0}; i < n_of_row; ++i) {
		date = static_cast<std::string>(
			pybind11::str(
				pd_df.attr(
					"iloc"
				)[pybind11::cast(i)].attr("name")
			)
		);
		temprow.year        = std::stoi(date.substr( 0, 4));
		temprow.month       = std::stoi(date.substr( 5, 2));
		temprow.day         = std::stoi(date.substr( 8, 2));
		temprow.open        = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 0)]
		);
		temprow.high        = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 1)]
		);
		temprow.low         = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 2)]
		);
		temprow.close       = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 3)]
		);
		temprow.volume      = pybind11::int_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 4)]
		);
		temprow.dividends   = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 5)]
		);
		temprow.stocksplits = pybind11::float_(
			pd_df.attr("iat")[pybind11::make_tuple(i, 6)]
		);
		df.push_back(temprow);
	}

	return;
}

void python_print_debug_info()
{
	std::cout << "<START DEBUG INFORMATION>\n";
	pybind11::module_ sys {pybind11::module_::import("sys")};
	pybind11::module_ pandas {pybind11::module_::import("pandas")};
	pybind11::module_ yfinance {pybind11::module_::import("yfinance")};
	pybind11::print(sys.attr("path"));
	std::cout << "python version:   " << std::flush;
	pybind11::print(sys.attr("version"));
	std::cout << "pandas version:   " << std::flush;
	pybind11::print(pandas.attr("__version__"));
	std::cout << "yfinance version: " << std::flush;
	pybind11::print(yfinance.attr("__version__"));
	std::cout << "< END DEBUG INFORMATION >\n\n";

	return;
}

inline void python_scope_guard(std::array<std::vector<stockrow>,
                                          number_of_df>& db)
{
	/*
	 * Python interpreter starts with
	 * pybind11::scoped_interpreter guard{};
	 * and it ends at the end of this scope (at the end of this function).
	 */
	pybind11::scoped_interpreter guard{};

	//Uncomment to grab version number of Python and modules on start
	//python_print_debug_info();
	//return;

	python_get_df(db[0], "AAPL", "2023-01-01", "2024-12-31");
	python_get_df(db[1], "TSLA", "2023-01-01", "2024-12-31");
	python_get_df(db[2], "AMZN", "2023-01-01", "2024-12-31");
	python_get_df(db[3], "V",    "2023-01-01", "2024-12-31");
	python_get_df(db[4], "MSFT", "2023-01-01", "2024-12-31");

	return;
}

int main() {
	std::array<std::vector<stockrow>,   number_of_df> db;   //Regular
	std::array<std::vector<stockrow_f>, number_of_df> dbf;  //Change
	std::array<std::vector<stockrow_f>, number_of_df> dbfp; //Change %
	std::array<std::vector<stockrow_b>, number_of_df> dbb;  //Up or Down

	std::cout.precision(10);
	python_scope_guard(db);
	for (int i {0}; i < number_of_df; ++i) {
		get_change(db[i], dbf[i]);
		get_change_per(db[i], dbfp[i]);
		get_up_or_down(dbf[i], dbb[i]);
	}
	// db[0] refers to the dataframe of Apple as it is index 0.
	// db[1] refers to the dataframe of Tesla as it is index 1.
	// print_df(db[0]);             Print dataframe
	// print_df_float(dbf[0]);      Print change in value
	// print_df_float_per(dbfp[0]); Print change in value as percentage
	// print_df_bool(dbb[0]);       Print if change is positive or not
	print_df(db[0]);
	print_df_float(dbf[0]);
	print_df_float_per(dbfp[0]);
	print_df_bool(dbb[0]);

	return 0;
}