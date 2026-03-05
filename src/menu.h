#ifndef MENU_H
#define MENU_H

#include "struc.h"
#include <vector>

bool menu(database &df, flag &f);

// Prints debug info for python.
inline void option_debug();

// Gets arguments, setups, and prepares for lstm.h/cpp
void run_lstm(std::vector<std::string> &cmd, database &db);

// Tries calling run_lstm and catches any exceptions
void option_lstm(std::vector<std::string> &cmd, database &db);

// Prints general info of all dataframes or prints all values in one dataframe
void option_print (std::vector<std::string> &cmd, database &db, const flag &f);

// Changes the number limit of characters you can input
void option_ilimit(const std::vector<std::string> &cmd, flag &f);

// Chnages the floating-point precision
void option_precision(const std::vector<std::string> &cmd, flag &f);

// Calls menu again with the Python interpreter active
void python_scope_guard(database &db, flag &f);

// Returns true if Python interpreter is active
inline bool check_py();

// The menu
bool menu(database &db, flag &f);

#endif