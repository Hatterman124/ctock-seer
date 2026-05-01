#ifndef TDATABASE_H
#define TDATABASE_H

#include <pybind11/embed.h>
#include <cstddef>
#include <stdfloat>
#include <vector>
#include <string>
#include <chrono>

const pybind11::int_ PY_ZERO {0};
const pybind11::int_ PY_ONE  {1};

struct stockrow {
	std::size_t    index;
	std::chrono::year_month_day date;

	std::float16_t close;

	// Ran out of time.
	/*
	// MA (Moving Average)
	std::float16_t close_ma;

	// EMA (Exponential Moving Average)
	std::float16_t close_ema;

	// MACD (Moving Average Convergence/Divergence)
	std::float16_t close_macd;

	// RSI (Relative Strength Index)
	std::float16_t close_rsi;
	*/
};


// I COULD turn these into class types, but I hate class.
struct tdataframe {
// public:
	tdataframe(const std::string &ticker,
	           std::istringstream date_beg,
	           std::istringstream date_end );
	void print(const int precision) const;
// private
	std::string ticker;
	std::chrono::year_month_day date_beg;
	std::chrono::year_month_day date_end;
	std::vector<stockrow> sr {};
};

struct tdatabase {
// public:
	~tdatabase();
	int get(const std::string &ticker,
	        const std::string &date_beg,
	        const std::string &date_end );
	void print() const;
// private:
	bool is_date_format(const std::string &s) const;
	std::vector<tdataframe*>::size_type size {};
	std::vector<tdataframe*> df {};
};

#endif
