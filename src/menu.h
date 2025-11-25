#include "struc.h"
#include "util.h"
#include "print.h"
#include <pybind11/embed.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

bool menu(database& df, flag& f);

void option_print(std::vector<std::string>& cmd, database& db, const flag& f)
{
	int width {getw<std::vector<std::string>::size_type>(db.df.size())};
	std::vector<dataframe>::size_type index {};
	bool is_num {true};

	if (cmd.size() == 1) {
		std::cout << "Not enough arguments.\n";
		return;
	}
	try {
		index = static_cast<std::vector<dataframe>::size_type>(
			std::stoull(cmd[1])
		);
	} catch (...) {
		is_num = false;
	}
	if (cmd[1] == "list") {
		for (std::vector<dataframe>::size_type i {};
		     i < db.df.size(); ++i) {
			std::cout << std::setw(width) << i << ": "
			          << db.ticker[i] << '\t'
			          << db.df[i].start_date << " to "
			          << db.df[i].end_date << '\n';
		}
		return;
	}
	if (!is_num) {
		std::cout << "Cannot convert " << cmd[1] << " to an index\n";
		return;
	}
	if (cmd[1][0] == '-') {
		std::cout << "Index cannot be a negative number\n";
	} else if (index >= db.df.size()) {
		std::cout << "There is not a dataframe releated to that index\nPlease run \"print list\"\n";
	} else if (cmd.size() == 2) {
		print_df(db.df[index].sr, f, false);
	} else if (cmd[2] == "dif") {
		print_df(db.df[index].sr_dif, f, true);
	} else if (cmd[2] == "per") {
		print_df(db.df[index].sr_per, f, true);
	} else if (cmd[2] == "up") {
		print_df(db.df[index].sr_up);
	}

	return;
}

void option_ilimit(const std::vector<std::string>& cmd, flag& f)
{
	std::string::size_type n {};

	if (cmd.size() == 1) {
		std::cout << "Not enough arguments.\n";
	} else if (cmd[1] == "print") {
		std::cout << f.in_limit << '\n';
	} else if (cmd[1] == "default") {
		f.in_limit = in_limit_default;
	} else if (cmd[1][0] == '-') {
		std::cout << "Cannot set input limit to negative numbers\n";
	} else {
		try {
			n = static_cast<std::string::size_type>(
				std::stoul(cmd[1])
			);
		} catch (std::invalid_argument const& ex) {
			std::cout << "ERROR: std::invalid_argument\nCannot convert \""
			          << cmd[1]
			          << "\" into an integer.\n";
			return;
		} catch (std::out_of_range const& ex) {
			std::cout << "ERROR: std::out_of_range\n\""
			          << cmd[1]
			          << "\" cannot fit into an unsigned long.\nIt must be from "
			          << ul_min << " to " << ul_max << '\n';
			return;
		}
		if (n <= in_limit_min) {
			std::cout << "Cannot set input limit to "
			          << in_limit_min
			          << " or lower.\n";
		} else {
			f.in_limit = n;
		}
	}

	return;
}

void option_precision(const std::vector<std::string>& cmd, flag& f)
{
	int n {};

	if (cmd.size() == 1) {
		std::cout << "Not enough arguments.\n";
	} else if (cmd[1] == "print") {
		std::cout << f.precision << '\n';
	} else if (cmd[1] == "default") {
		std::cout.precision(precision_default);
		f.precision = precision_default;
	} else {
		try {
			n = std::stoi(cmd[1]);
		} catch (std::invalid_argument const& ex) {
			std::cout << "ERROR: std::invalid_argument\nCannot convert \""
			          << cmd[1]
			          << "\" into an integer.\n";
			return;
		} catch (std::out_of_range const& ex) {
			std::cout << "ERROR: std::out_of_range\n\""
			          << cmd[1]
			          << "\" cannot fit into an integer.\nIt must be from "
			          << i_min << " to " << i_max << '\n';
			return;
		}
		if (n <= 0) {
			std::cout << "Cannot set precision to zero or lower.\n";
		} else {
			std::cout.precision(n);
			f.precision = n;
		}
	}

	return;
}

void python_scope_guard(database& db, flag& f)
{
	/*
	 * Python interpreter starts with
	 * pybind11::scoped_interpreter guard{};
	 * and it ends at the end of this scope (at the end of this function).
	 */
	pybind11::scoped_interpreter guard {};
	while (true) {
		if (menu(db, f))
			break;
	}

	return;
}

inline bool check_py(bool b)
{
	if (!b)
		std::cout << "Can only be run in Python-interpreter mode\n";

	return b;
}

bool menu(database& db, flag& f)
{
	std::string s {};
	std::vector<std::string> cmd {};
	std::string::size_type str_cur_beg {};
	std::string::size_type str_cur_end {};

	s.reserve(32);
	if (f.py_mode)
		std::cout << "#> ";
	else
		std::cout << "$> ";
	if (!std::getline(std::cin, s)) {
		f.py_mode = false;
		std::cout << '\n';
		return true;
	}
	if (s.size() > f.in_limit) {
		std::cout << "The character input limit is "
		          << f.in_limit << '\n';
		return false;
	}
	s.erase(0, s.find_first_not_of(whitespace));
	s.erase(s.find_last_not_of(whitespace) + 1);
	if (s.empty())
		return false;
	str_cur_end = s.find_first_of(whitespace);
	if (str_cur_end == std::string::npos)
		str_cur_end = s.size();
	while (str_cur_beg != std::string::npos) {
		cmd.push_back(s.substr(str_cur_beg, str_cur_end - str_cur_beg));
		str_cur_end = s.find_first_of(whitespace, str_cur_end);
		str_cur_beg = s.find_first_not_of(whitespace, str_cur_end);
		str_cur_end = s.find_first_of(whitespace, str_cur_beg);
	}
	if (cmd[0] == "exit" || cmd[0] == "q" || cmd[0] == "Q") {
		f.py_mode = false;
		return true;
	}
	if (cmd[0] == "help") {
		std::cout << "The Python interpreter is currently ";
		if (!f.py_mode)
			std::cout << "not ";
		std::cout << "running\n\n\"#>\" Python-interpreter mode running\n\"$>\" Python-interpreter mode NOT running\n\nCan be run in either mode:\nhelp                Print out this message\nexit|q              Exits Python-interpreter mode or progam\n                    depending on what mode you are in\npython              Enters Python-interpreter mode\nilimit <option>     Sets the maximum number input characters\n                    <option>\n                    (number)  Sets limit to that number\n                    default   Sets limit to the default limit\n                    print     Prints current input limit\nprecision <option>  Sets the precision of floating-point numbers\n                    <option>\n                    (number)  Sets precision to that number\n                    default   Sets precision to the default number\n                    print     Prints current set floating-point precision\nprint list          Prints list of dataframes with their index.\nprint <index> <type>  Prints dataframe of a type.\n                    <type>\n                    <empty>   Prints default dataframe\n                    dif       Prints difference dataframe\n                    per       Prints percentage dataframe\n                    log       Prints log dataframe\n                    up        Prints up or down dataframe\n\nCan only be run in Python-interpreter mode:\ndebug               List python debug information\nget <ticker> <start_date> <end_date>  Get stock dataframe of TICKER\n                    Format:   get TICKER YYYY-MM-DD YYYY-MM-DD\n                    Example:  get AAPL 2023-01-01 2024-12-31\n\n";
		return false;
	}
	if (cmd[0] == "python") {
		if (f.py_mode) {
			std::cout << "Python interpreter is already running.\n";
		} else {
			f.py_mode = true;
			python_scope_guard(db, f);
		}
		return false;
	}
	if (cmd[0] == "precision") {
		option_precision(cmd, f);
		return false;
	}
	if (cmd[0] == "ilimit") {
		option_ilimit(cmd, f);
		return false;
	}
	if (cmd[0] == "print") {
		option_print(cmd, db, f);
		return false;
	}
	if (cmd[0] == "debug" && check_py(f.py_mode)) {
		pybind11::module_ sys {pybind11::module_::import("sys")};
		pybind11::module_ pandas {pybind11::module_::import("pandas")};
		pybind11::module_ yfinance {pybind11::module_::import("yfinance")};
		pybind11::print(sys.attr("path"));
		std::cout << "python version:   " << std::flush;
		pybind11::print(sys.attr("version"));
		std::cout << "pandas version:   " << std::flush;
		pybind11::print(pandas.attr("__version__"));
		std::cout << "yfinance version: " << std::flush;
		pybind11::print(yfinance.attr("__version__"));
		return false;
	}
	if (cmd[0] == "get" && check_py(f.py_mode)) {
		try {
			python_get_df(cmd, db);
		} catch (pybind11::error_already_set const& ex) {
			std::cout << ex.what()
			          << "\nERROR: pybind11::error_already_set\nUnable to grab stock data!\n";
		}
		return false;
	}

	return false;
}




/*
	python_get_df(db[0], "AAPL", "2023-01-01", "2024-12-31");
	python_get_df(db[1], "TSLA", "2023-01-01", "2024-12-31");
	python_get_df(db[2], "AMZN", "2023-01-01", "2024-12-31");
	python_get_df(db[3], "V",    "2023-01-01", "2024-12-31");
	python_get_df(db[4], "MSFT", "2023-01-01", "2024-12-31");
	*/