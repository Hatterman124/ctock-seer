#ifndef UTIL_H
#define UTIL_H

#include "struc.h"
#include <pybind11/embed.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <cmath>

// Returns true is string is in correct date format for yfinance
bool is_date_format(std::string s);

// Starts yfinance to download pandas dataframe of ticker
// then converts it into custom dataframe
void python_get_df(std::vector<std::string> &cmd, database &db);

#endif