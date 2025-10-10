#include <pybind11/embed.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

//Number of different stocks or the number of dataframe per company.
constexpr int number_of_df {3};

/*
 * Structure of Database
 * Array of Vector of Struct
 *
 * Array:  db
 * Vector: df
 * struct: stockrow
 */

struct stockrow {
	int    year;
	int    month;
	int    day;
	int    hour;
	int    minute;
	int    second;
	double open;
	double high;
	double low;
	double close;
	int    volume;
	double dividends;
	double stocksplits;
};

struct stockrow_p {
	int    year;
	int    month;
	int    day;
	int    hour;
	int    minute;
	int    second;
	double open;
	double high;
	double low;
	double close;
	double volume;
	double dividends;
	double stocksplits;
};

template <typename T>
int getw(T greatest_n)
{
	int n {static_cast<int>(greatest_n)};
	int width {};

	if (n == 0)
		return 1;
	for (int i {}; n != 0; ++i) {
		++width;
		n /= 10;
	}

	return width;
}

void print_dfp(const std::vector<stockrow_p> df)
{
	double greatest_open {};
	double greatest_high {};
	double greatest_low {};
	double greatest_close {};
	double greatest_volume {};
	double greatest_dividends {};
	double greatest_stocksplits {};

	//Find digit length of each member so width of column can be set.
	for (std::vector<stockrow>::size_type i {0}; i < df.size(); ++i ) {
		if (greatest_open        < abs(df[i].open)
		    && !std::isinf(df[i].open))
			greatest_open        = abs(df[i].open);
		if (greatest_high        < abs(df[i].high)
		    && !std::isinf(df[i].high))
			greatest_high        = abs(df[i].high);
		if (greatest_low         < abs(df[i].low)
		    && !std::isinf(df[i].low))
			greatest_low         = abs(df[i].low);
		if (greatest_close       < abs(df[i].close)
		    && !std::isinf(df[i].close))
			greatest_close       = abs(df[i].close);
		if (greatest_volume      < abs(df[i].volume)
		    && !std::isinf(df[i].volume))
			greatest_volume      = abs(df[i].volume);
		if (greatest_dividends   < abs(df[i].dividends)
		    && !std::isinf(df[i].dividends))
			greatest_dividends   = abs(df[i].dividends);
		if (greatest_stocksplits < abs(df[i].stocksplits)
		    && !std::isinf(df[i].stocksplits))
			greatest_stocksplits = abs(df[i].stocksplits);
	}

	//Print
	for (std::vector<stockrow>::size_type i {}; i < df.size(); ++i ) {
		std::cout << df[i].year        << '-'
		          << std::setfill ('0') << std::setw(2)
		          << df[i].month       << '-'
		          << std::setw(2)
		          << df[i].day         << '-'
		          << std::setw(2)
		          << df[i].hour        << '-'
		          << std::setw(2)
		          << df[i].minute      << '-'
		          << std::setw(2)
		          << df[i].second      << " | "
		          << std::setfill (' ')
		          << std::setw(getw<double>(greatest_open)        + 12)
		          << std::fixed << std::setprecision(10)
		          << std::showpos
		          << df[i].open        << "% | "
		          << std::setw(getw<double>(greatest_high)        + 12)
		          << df[i].high        << "% | "
		          << std::setw(getw<double>(greatest_low)         + 12)
		          << df[i].low         << "% | "
		          << std::setw(getw<double>(greatest_close)       + 12)
		          << df[i].close       << "% | "
		          << std::setw(getw<double>(greatest_volume)      + 12)
		          << df[i].volume      << "% | "
		          << std::setw(getw<double>(greatest_dividends)   + 12)
		          << df[i].dividends   << "% | "
		          << std::setw(getw<double>(greatest_stocksplits) + 12)
		          << df[i].stocksplits
		          << std::defaultfloat << std::noshowpos << "%\n";
	}

	return;
}

void print_df(const std::vector<stockrow> df)
{
	double greatest_open {};
	double greatest_high {};
	double greatest_low {};
	double greatest_close {};
	int    greatest_volume {};
	double greatest_dividends {};
	double greatest_stocksplits {};

	//Find digit length of each member so width of column can be set.
	for (std::vector<stockrow>::size_type i {0}; i < df.size(); ++i ) {
		if (greatest_open        < df[i].open)
			greatest_open        = df[i].open;
		if (greatest_high        < df[i].high)
			greatest_high        = df[i].high;
		if (greatest_low         < df[i].low)
			greatest_low         = df[i].low;
		if (greatest_close       < df[i].close)
			greatest_close       = df[i].close;
		if (greatest_volume      < df[i].volume)
			greatest_volume      = df[i].volume;
		if (greatest_dividends   < df[i].dividends)
			greatest_dividends   = df[i].dividends;
		if (greatest_stocksplits < df[i].stocksplits)
			greatest_stocksplits = df[i].stocksplits;
	}

	//Print
	for (std::vector<stockrow>::size_type i {}; i < df.size(); ++i ) {
		std::cout << df[i].year        << '-'
		          << std::setfill ('0') << std::setw(2)
		          << df[i].month       << '-'
		          << std::setw(2)
		          << df[i].day         << '-'
		          << std::setw(2)
		          << df[i].hour        << '-'
		          << std::setw(2)
		          << df[i].minute      << '-'
		          << std::setw(2)
		          << df[i].second      << " | "
		          << std::setfill (' ')
		          << std::setw(getw<double>(greatest_open)        + 11)
		          << std::fixed << std::setprecision(10)
		          << df[i].open        << " | "
		          << std::setw(getw<double>(greatest_high)        + 11)
		          << df[i].high        << " | "
		          << std::setw(getw<double>(greatest_low)         + 11)
		          << df[i].low         << " | "
		          << std::setw(getw<double>(greatest_close)       + 11)
		          << df[i].close       << " | "
		          << std::setw(getw<int>(greatest_volume))
		          << df[i].volume      << " | "
		          << std::setw(getw<double>(greatest_dividends)   + 11)
		          << df[i].dividends   << " | "
		          << std::setw(getw<double>(greatest_stocksplits) + 11)
		          << df[i].stocksplits
		          << std::defaultfloat << '\n';
	}

	return;
}

void get_change_per(const std::vector<stockrow> df,
                    std::vector<stockrow_p>& dfp)
{
	stockrow_p temprow {};
	int prv_i {};

	for (std::vector<stockrow>::size_type i {1}; i < df.size(); ++i) {
		prv_i = i - 1;
		temprow.year        = df[i].year;
		temprow.month       = df[i].month;
		temprow.day         = df[i].day;
		temprow.hour        = df[i].hour;
		temprow.minute      = df[i].minute;
		temprow.second      = df[i].second;
		temprow.open        = (df[i].open
		                       - df[prv_i].open)
		                      / df[prv_i].open
		                      * 100.0;
		temprow.high        = (df[i].high
		                       - df[prv_i].high)
		                      / df[prv_i].high
		                      * 100.0;
		temprow.low         = (df[i].low
		                       - df[prv_i].low)
		                      / df[prv_i].low
		                      * 100.0;
		temprow.close       = (df[i].close
		                       - df[prv_i].close)
		                      / df[prv_i].close
		                      * 100.0;
		temprow.volume      = (static_cast<double>(df[i].volume)
		                       - static_cast<double>(df[prv_i].volume))
		                      / static_cast<double>(df[prv_i].volume)
		                      * 100.0;
		temprow.dividends   = (df[i].dividends
		                       - df[prv_i].dividends)
		                      / df[prv_i].dividends
		                      * 100.0;
		temprow.stocksplits = (df[i].stocksplits
		                       - df[prv_i].stocksplits)
		                      / df[prv_i].stocksplits
		                      * 100.0;
		dfp.push_back(temprow);
	}

	return;
}

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
		temprow.hour        = std::stoi(date.substr(11, 2));
		temprow.minute      = std::stoi(date.substr(14, 2));
		temprow.second      = std::stoi(date.substr(17, 2));
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

	python_get_df(db[0], "AAPL", "2022-01-01", "2024-12-31");
	python_get_df(db[1], "AMD",  "2022-01-01", "2024-12-31");
	python_get_df(db[2], "NVDA", "2022-01-01", "2024-12-31");

	return;
}

int main() {
	std::array<std::vector<stockrow>,   number_of_df> db;
	std::array<std::vector<stockrow_p>, number_of_df> dbp;

	std::cout.precision(10);
	python_scope_guard(db);
	print_df(db[0]);
	get_change_per(db[0], dbp[0]);
	print_dfp(dbp[0]);

	return 0;
}