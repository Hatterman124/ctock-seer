#include "struc.h"
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

	do {
		++width;
		n /= 10;
	} while (n != 0);

	return width;
}

/*
void print_df_bool(const std::vector<stockrow_b> df)
{
	//Print
	for (std::vector<stockrow>::size_type i {}; i < df.size(); ++i ) {
		std::cout  << std::setfill (' ')
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
		std::cout << std::setfill (' ')
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

void print_df_float_per(const std::vector<stockrow_f> df,
			const flag& f,
			bool pos)
{
	int shift {2 + f.precision};
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
		std::cout << std::setfill (' ')
		          << std::setw(getw<double>(greatest_open)        + 12)
		          << std::fixed << std::showpos
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
*/

void print_df(const std::vector<stockrow> df,
              const flag& f,
              const bool neg)
{

	int sft {1 + f.precision};
	int b_sft {};
	double greatest_open        {df[0].open};
	double greatest_high        {df[0].high};
	double greatest_low         {df[0].low};
	double greatest_close       {df[0].close};
	int    greatest_volume      {df[0].volume};
	double greatest_dividends   {df[0].dividends};
	double greatest_stocksplits {df[0].stocksplits};

	if (neg) {
		++sft;
		++b_sft;
	}
	//Find digit length of each member so width of column can be set.
	for (std::vector<stockrow>::size_type i {}; i < df.size(); ++i ) {
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
		std::cout << std::setfill(' ') << std::noshowpos
		          << std::setw(getw<int>(df.back().index))
		          << df[i].index       << " | ";
		if (neg)
			std::cout << std::showpos;
		std::cout << df[i].date        << " | "
		          << std::fixed
		          << std::setw(getw<double>(greatest_open)        + sft)
		          << df[i].open        << " | "
		          << std::setw(getw<double>(greatest_high)        + sft)
		          << df[i].high        << " | "
		          << std::setw(getw<double>(greatest_low)         + sft)
		          << df[i].low         << " | "
		          << std::setw(getw<double>(greatest_close)       + sft)
		          << df[i].close       << " | "
		          << std::setw(getw<int>(greatest_volume)       + b_sft)
		          << df[i].volume      << " | "
		          << std::setw(getw<double>(greatest_dividends)   + sft)
		          << df[i].dividends   << " | "
		          << std::setw(getw<double>(greatest_stocksplits) + sft)
		          << df[i].stocksplits << '\n';
	}
	std::cout << std::noshowpos;

	return;
}

void print_df(const std::vector<stockrow_f> df,
              const flag& f,
              const bool per)
{

	int sft {2 + f.precision};
	std::string column {};
	double greatest_open        {df[0].open};
	double greatest_high        {df[0].high};
	double greatest_low         {df[0].low};
	double greatest_close       {df[0].close};
	double greatest_volume      {df[0].volume};
	double greatest_dividends   {df[0].dividends};
	double greatest_stocksplits {df[0].stocksplits};

	if (per)
		column = "% | ";
	else
		column = " | ";

	//Find digit length of each member so width of column can be set.
	for (std::vector<stockrow_f>::size_type i {}; i < df.size(); ++i ) {
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
	for (std::vector<stockrow_f>::size_type i {}; i < df.size(); ++i ) {
		std::cout << std::setfill(' ') << std::noshowpos
		          << std::setw(getw<int>(df.back().index))
		          << df[i].index       << " | "
		          << std::showpos
		          << df[i].date        << " | "
		          << std::fixed
		          << std::setw(getw<double>(greatest_open)        + sft)
		          << df[i].open        << column
		          << std::setw(getw<double>(greatest_high)        + sft)
		          << df[i].high        << column
		          << std::setw(getw<double>(greatest_low)         + sft)
		          << df[i].low         << column
		          << std::setw(getw<double>(greatest_close)       + sft)
		          << df[i].close       << column
		          << std::setw(getw<double>(greatest_volume)      + sft)
		          << df[i].volume      << column
		          << std::setw(getw<double>(greatest_dividends)   + sft)
		          << df[i].dividends   << column
		          << std::setw(getw<double>(greatest_stocksplits) + sft)
		          << df[i].stocksplits;
		if (per)
			std::cout << "%\n";
		else
			std::cout << '\n';
	}
	std::cout << std::noshowpos;

	return;
}

void print_df(const std::vector<stockrow_b> df)
{
	//Print
	for (std::vector<stockrow>::size_type i {}; i < df.size(); ++i ) {
		std::cout << std::setfill(' ')
		          << std::setw(getw<int>(df.back().index))
		          << df[i].index       << " | "
		          << df[i].date        << " | "
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