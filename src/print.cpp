#include "print.h"
#include "struc.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

void print_df(const std::vector<stockrow> &df,
              const flag &f,
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

void print_df(const std::vector<stockrow_f> &df,
              const flag &f,
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

void print_df(const std::vector<stockrow_b> &df)
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

void graph_df(const std::vector<stockrow> &df,
              const unsigned long wd,
              const unsigned long ht)
{
	if (df.empty())
		return;

	const unsigned long ht_minus {ht - 1};
	std::string graph {};
	std::vector<unsigned long> graph_num {};
	// Number of prices per chars.
	std::vector<stockrow>::size_type w_n {df.size() / wd};
	float w_nf {static_cast<float>(w_n)};
	float min {df[0].close};
	float max {df[0].close};

	graph_num.reserve(wd);
	for (std::vector<stockrow>::size_type i {1}; i < df.size(); ++i) {
		if (df[i].close < min)
			min = df[i].close;
		else if (df[i].close > max)
			max = df[i].close;
	}

	// Size of char block.
	float h_n { (std::ceilf(max) - std::floorf(min)) / ht};
	float avg {};

	std::cout << "size: " << df.size()
	          << "\nmin: " << min
	          << "\nmax: " << max
	          << "\nw_n: " << w_n
	          << "\nh_n: " << h_n << '\n';;

	/*
	 * i:   Index for assigning graph_num.
	 * i_df:  Index for traversing df.
	 * ii: Index to know when to stop average batch
	 *
	 */
	std::vector<stockrow>::size_type i_df {};
	for (std::vector<unsigned long>::size_type i {}; i < wd; ++i) {
		for (std::vector<stockrow>::size_type ii {};
		     ii < w_n;
		     ++ii) {
			avg += df[i_df].close;
			++i_df;
		}
		avg /= w_nf;
		std::cout << h_n << " / " << avg - std::floorf(min) << '\n';
		graph_num.push_back(
			static_cast<unsigned long>(
				(avg - std::floorf(min)) / h_n
			)
		);
		avg = 0;
	}
	for (unsigned long n : graph_num)
		std::cout << n << '\n';
	for (unsigned int i {}; i < ht; ++i) {
		for (unsigned int ii {}; ii < wd; ++ii) {
			if (graph_num[ii] == ht_minus - i)
				graph.push_back('*');
			else
				graph.push_back(' ');
		}
		graph.push_back('\n');
	}
	std::cout << graph;
}