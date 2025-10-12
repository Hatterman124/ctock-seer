#include "stockrow.h"
#include <pybind11/embed.h>
#include <vector>

void get_up_or_down(const std::vector<stockrow_f> dff,
                    std::vector<stockrow_b>& dfb)
{
	stockrow_b temprow {};

	for (std::vector<stockrow>::size_type i {1}; i < dff.size(); ++i) {
		temprow.year        = dff[i].year;
		temprow.month       = dff[i].month;
		temprow.day         = dff[i].day;
		if (dff[i].open        > 0.0)
			temprow.open        = true;
		else
			temprow.open        = false;
		if (dff[i].high        > 0.0)
			temprow.high        = true;
		else
			temprow.high        = false;
		if (dff[i].low         > 0.0)
			temprow.low         = true;
		else
			temprow.low         = false;
		if (dff[i].close       > 0.0)
			temprow.close       = true;
		else
			temprow.close       = false;
		if (dff[i].volume      > 0.0)
			temprow.volume      = true;
		else
			temprow.volume      = false;
		if (dff[i].dividends   > 0.0)
			temprow.dividends   = true;
		else
			temprow.dividends   = false;
		if (dff[i].stocksplits > 0.0)
			temprow.stocksplits = true;
		else
			temprow.stocksplits = false;
		dfb.push_back(temprow);
	}

	return;
}

void get_change(const std::vector<stockrow> df,
                    std::vector<stockrow_f>& dff)
{
	stockrow_f temprow {};
	int prv_i {};

	for (std::vector<stockrow>::size_type i {1}; i < df.size(); ++i) {
		prv_i = i - 1;
		temprow.year        = df[i].year;
		temprow.month       = df[i].month;
		temprow.day         = df[i].day;
		temprow.open        = df[i].open
		                      - df[prv_i].open;
		temprow.high        = df[i].high
		                      - df[prv_i].high;
		temprow.low         = df[i].low
		                      - df[prv_i].low;
		temprow.close       = df[i].close
		                      - df[prv_i].close;
		temprow.volume      = static_cast<double>(df[i].volume)
		                      - static_cast<double>(df[prv_i].volume);
		temprow.dividends   = df[i].dividends
		                      - df[prv_i].dividends;
		temprow.stocksplits = df[i].stocksplits
		                      - df[prv_i].stocksplits;
		dff.push_back(temprow);
	}

	return;
}

void get_change_per(const std::vector<stockrow> df,
                    std::vector<stockrow_f>& dffd)
{
	stockrow_f temprow {};
	int prv_i {};

	for (std::vector<stockrow>::size_type i {1}; i < df.size(); ++i) {
		prv_i = i - 1;
		temprow.year        = df[i].year;
		temprow.month       = df[i].month;
		temprow.day         = df[i].day;
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
		dffd.push_back(temprow);
	}

	return;
}