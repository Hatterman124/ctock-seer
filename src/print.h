#ifndef PRINT_H
#define PRINT_H

#include "struc.h"
#include <pybind11/embed.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

// Gets number of chars a number is.
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

// Prints dataframe
void print_df(const std::vector<stockrow> df,
              const flag &f,
              const bool neg);

// Prints floating-point dataframe
void print_df(const std::vector<stockrow_f> df,
              const flag &f,
              const bool per);

// Prints boolean dataframe
void print_df(const std::vector<stockrow_b> df);

#endif