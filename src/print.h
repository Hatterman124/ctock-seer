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

void print_df(const std::vector<stockrow> df,
              const flag& f,
              const bool neg)
{

	int sft {1 + f.precision};
	int b_sft {};
	float greatest_open        {df[0].open};
	float greatest_high        {df[0].high};
	float greatest_low         {df[0].low};
	float greatest_close       {df[0].close};
	int    greatest_volume      {df[0].volume};
	float greatest_dividends   {df[0].dividends};
	float greatest_stocksplits {df[0].stocksplits};

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
		          << std::setw(getw<float>(greatest_open)        + sft)
		          << df[i].open        << " | "
		          << std::setw(getw<float>(greatest_high)        + sft)
		          << df[i].high        << " | "
		          << std::setw(getw<float>(greatest_low)         + sft)
		          << df[i].low         << " | "
		          << std::setw(getw<float>(greatest_close)       + sft)
		          << df[i].close       << " | "
		          << std::setw(getw<int>(greatest_volume)       + b_sft)
		          << df[i].volume      << " | "
		          << std::setw(getw<float>(greatest_dividends)   + sft)
		          << df[i].dividends   << " | "
		          << std::setw(getw<float>(greatest_stocksplits) + sft)
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
	float greatest_open        {df[0].open};
	float greatest_high        {df[0].high};
	float greatest_low         {df[0].low};
	float greatest_close       {df[0].close};
	float greatest_volume      {df[0].volume};
	float greatest_dividends   {df[0].dividends};
	float greatest_stocksplits {df[0].stocksplits};

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
		          << std::setw(getw<float>(greatest_open)        + sft)
		          << df[i].open        << column
		          << std::setw(getw<float>(greatest_high)        + sft)
		          << df[i].high        << column
		          << std::setw(getw<float>(greatest_low)         + sft)
		          << df[i].low         << column
		          << std::setw(getw<float>(greatest_close)       + sft)
		          << df[i].close       << column
		          << std::setw(getw<float>(greatest_volume)      + sft)
		          << df[i].volume      << column
		          << std::setw(getw<float>(greatest_dividends)   + sft)
		          << df[i].dividends   << column
		          << std::setw(getw<float>(greatest_stocksplits) + sft)
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