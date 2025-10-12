#include "stockrow.h"
#include <pybind11/embed.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

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
		          << df[i].day         << " | "
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

void print_df_bool(const std::vector<stockrow_b> df)
{
	//Print
	for (std::vector<stockrow>::size_type i {}; i < df.size(); ++i ) {
		std::cout << df[i].year        << '-'
		          << std::setfill ('0') << std::setw(2)
		          << df[i].month       << '-'
		          << std::setw(2)
		          << df[i].day         << " | "
		          << std::setfill (' ')
		          << df[i].open        << " | "
		          << df[i].high        << " | "
		          << df[i].low         << " | "
		          << df[i].close       << " | "
		          << df[i].volume      << " | "
		          << df[i].dividends   << " | "
		          << df[i].stocksplits << '\n';
	}

	return;
}

void print_df_float(const std::vector<stockrow_f> df)
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
		          << df[i].day         << " | "
		          << std::setfill (' ')
		          << std::setw(getw<double>(greatest_open)        + 12)
		          << std::fixed << std::setprecision(10)
		          << std::showpos
		          << df[i].open        << " | "
		          << std::setw(getw<double>(greatest_high)        + 12)
		          << df[i].high        << " | "
		          << std::setw(getw<double>(greatest_low)         + 12)
		          << df[i].low         << " | "
		          << std::setw(getw<double>(greatest_close)       + 12)
		          << df[i].close       << " | "
		          << std::setw(getw<double>(greatest_volume)      + 12)
		          << df[i].volume      << " | "
		          << std::setw(getw<double>(greatest_dividends)   + 12)
		          << df[i].dividends   << " | "
		          << std::setw(getw<double>(greatest_stocksplits) + 12)
		          << df[i].stocksplits
		          << std::defaultfloat << std::noshowpos << '\n';
	}

	std::cout << getw<double>(greatest_dividends) << '\n';
	return;
}

void print_df_float_per(const std::vector<stockrow_f> df)
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
		          << df[i].day         << " | "
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