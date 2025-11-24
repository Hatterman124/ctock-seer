#include <array>
#include <vector>
#include <string>
#include <chrono>
#include <limits>
#pragma once

constexpr int           i_min  {std::numeric_limits<int>::min()};
constexpr int           i_max  {std::numeric_limits<int>::max()};
constexpr unsigned long ul_min {std::numeric_limits<unsigned long>::min()};
constexpr unsigned long ul_max {std::numeric_limits<unsigned long>::max()};
constexpr std::string::size_type str_max {
	std::numeric_limits<std::string::size_type>::max()
};
constexpr std::string            whitespace {" \f\n\r\t\v"};
constexpr std::array<char, 10>   digits {'0', '1', '2', '3', '4',
                                        '5', '6', '7', '8', '9'};
constexpr std::string::size_type in_limit_default {32};
constexpr std::string::size_type in_limit_min {16};
constexpr int                    precision_default {11};

struct stockrow {
	int    index;
	std::chrono::year_month_day date;
	double open;
	double high;
	double low;
	double close;
	int    volume;
	double dividends;
	double stocksplits;
};

struct stockrow_b {
	int    index;
	std::chrono::year_month_day date;
	bool   open;
	bool   high;
	bool   low;
	bool   close;
	bool   volume;
	bool   dividends;
	bool   stocksplits;
};

struct stockrow_f {
	int    index;
	std::chrono::year_month_day date;
	double open;
	double high;
	double low;
	double close;
	double volume;
	double dividends;
	double stocksplits;
};

struct dataframe {
	std::vector<stockrow>   sr;     // Regular
	std::vector<stockrow>   sr_dif; // Change
	std::vector<stockrow_f> sr_per; // Change Percent
	std::vector<stockrow_f> sr_log; // Log Change
	std::vector<stockrow_b> sr_up;  // Up or Down
	std::chrono::year_month_day start_date;
	std::chrono::year_month_day end_date;
};

struct database {
	std::vector<dataframe>   df;
	std::vector<std::string> ticker; // Name of stock (Ticker Symbol)
};

struct flag {
	bool                   py_mode;   // Running state of Python Interpreter
	std::string::size_type in_limit;  // Command input length limit at menu
	int                    precision; // Floating-point number precision
};