#ifndef STRUC_H
#define STRUC_H

#include <array>
#include <vector>
#include <string>
#include <chrono>
#include <limits>

constexpr int           I_LOW  {std::numeric_limits<int>::lowest()};
constexpr int           I_MAX  {std::numeric_limits<int>::max()};
constexpr unsigned long UL_LOW {std::numeric_limits<unsigned long>::lowest()};
constexpr unsigned long UL_MAX {std::numeric_limits<unsigned long>::max()};
constexpr std::string            WHITESPACE {" \f\n\r\t\v"};
constexpr std::string::size_type IN_LIMIT_DEFAULT {80};
constexpr std::string::size_type IN_LIMIT_MIN {16};
constexpr int                    PRECISION_DEFAULT {11};

struct stockrow {
	int    index;
	std::chrono::year_month_day date;
	float open;
	float high;
	float low;
	float close;
	int    volume;
	float dividends;
	float stocksplits;
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
	float open;
	float high;
	float low;
	float close;
	float volume;
	float dividends;
	float stocksplits;
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
	// Running state of Python Interpreter
	bool                   py_mode   {};
	// Command input length limit at menu
	std::string::size_type in_limit  {IN_LIMIT_DEFAULT};
	// Floating-point number precision
	int                    precision {PRECISION_DEFAULT};
};

#endif