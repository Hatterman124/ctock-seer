#include "struc.h"
#include "util.h"
#include "print.h"
#include "lstm.h"
#include <pybind11/embed.h>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

bool menu(database& df, flag& f);

inline void option_debug()
{
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

	return;
}

/*

FOR LAGGED DATA

bool find_if_and_erase(std::vector<std::string>& v, std::string s)
{
	for (std::string::size_type i {}; i < v.size(); ++i) {
		if (v[i] == s) {
			v.erase(v.begin() + i);
			return true;
		}
	}

	return false;
}
*/

void run_lstm(std::vector<std::string>& cmd, database& db)
{
	std::vector<std::string>::size_type index_m     {};
	std::vector<std::string>::size_type index_test  {};
	std::vector<stockrow_b>::size_type  buffer      {};
        std::vector<stockrow_b>::size_type  gap         {};
        std::vector<stockrow_b>::size_type  target      {};
	int64_t                             hidden_size {};
	int64_t                             epochs      {};
	std::vector<std::string>::size_type index_s0    {};
	std::vector<std::string>::size_type index_s1    {};
	std::vector<std::string>::size_type index_s2    {};


	if (cmd.size() == 8) {
		index_m     = static_cast<std::vector<std::string>::size_type>(
			std::stoul(cmd[1])
		);
		index_test  = static_cast<std::vector<std::string>::size_type>(
			std::stoul(cmd[2])
		);
		buffer      = static_cast<std::vector<stockrow_b>::size_type>(
			std::stoul(cmd[3])
		);
		gap         = static_cast<std::vector<stockrow_b>::size_type>(
			std::stoul(cmd[4])
		);
		target      = static_cast<std::vector<stockrow_b>::size_type>(
			std::stoul(cmd[5])
		);
		hidden_size = static_cast<int64_t>(
			std::stol(cmd[6])
		);
		epochs      = static_cast<int64_t>(
			std::stol(cmd[7])
		);
		if (buffer == 0) {
			std::cout << "Buffer cannot be zero\n";
			throw std::out_of_range("");
		}
		if (target == 0) {
			std::cout << "Target cannot be zero\n";
			throw std::out_of_range("");
		}
		if (hidden_size <= 0) {
			std::cout << "Hidden size cannot be zero or lower\n";
			throw std::out_of_range("");
		}
		if (epochs <= 0) {
			std::cout << "Epochs cannot be zero or lower\n";
			throw std::out_of_range("");
		}
		run_lstm(db.df.at(index_m),
		         db.df.at(index_test),
		         buffer,
		         gap,
		         target,
		         hidden_size,
		         epochs);
	} else if (cmd.size() == 11) {
		index_m     = static_cast<std::vector<std::string>::size_type>(
			std::stoul(cmd[1])
		);
		index_test  = static_cast<std::vector<std::string>::size_type>(
			std::stoul(cmd[2])
		);
		buffer      = static_cast<std::vector<stockrow_b>::size_type>(
			std::stoul(cmd[3])
		);
		gap         = static_cast<std::vector<stockrow_b>::size_type>(
			std::stoul(cmd[4])
		);
		target      = static_cast<std::vector<stockrow_b>::size_type>(
			std::stoul(cmd[5])
		);
		hidden_size = static_cast<int64_t>(
			std::stol(cmd[6])
		);
		epochs      = static_cast<int64_t>(
			std::stol(cmd[7])
		);
		index_s0    = static_cast<std::vector<std::string>::size_type>(
			std::stoul(cmd[8])
		);
		index_s1    = static_cast<std::vector<std::string>::size_type>(
			std::stoul(cmd[9])
		);
		index_s2    = static_cast<std::vector<std::string>::size_type>(
			std::stoul(cmd[10])
		);
		if (buffer == 0) {
			std::cout << "Buffer cannot be zero\n";
			throw std::out_of_range("");
		}
		if (target == 0) {
			std::cout << "Target cannot be zero\n";
			throw std::out_of_range("");
		}
		if (hidden_size <= 0) {
			std::cout << "Hidden size cannot be zero or lower\n";
			throw std::out_of_range("");
		}
		if (epochs <= 0) {
			std::cout << "Epochs cannot be zero or lower\n";
			throw std::out_of_range("");
		}
		run_lstm(db.df.at(index_m),
		         db.df.at(index_test),
		         buffer,
		         gap,
		         target,
		         hidden_size,
		         epochs,
		         db.df.at(index_s0),
		         db.df.at(index_s1),
		         db.df.at(index_s2));
	} else if (cmd.size() < 9) {
		std::cout << "Not enough arguments.\n";
	} else if (cmd.size() > 9) {
		std::cout << "Too many arguments.\n";
	}
}

void option_lstm(std::vector<std::string>& cmd, database& db)
{
	// Try-catch wrapper for running lstm
	try {
		run_lstm(cmd, db);
	} catch (const std::invalid_argument& ex) {
		std::cout << "ERROR: std::invalid_argument\nOne or more arguments cannot be convered into an integer\n";
	} catch (std::out_of_range const& ex) {
		std::cout << "ERROR: std::out_of_range\nThe number you entered cannot fit into the required intiger type.\n";
	}

	/*
	 catch (...) {
		std::cout << "ERROR:\nLSTM ran into an error and stopped.\nFor some reason, LSTM randomly crashes, try again\n";
	}
	*/
	return;
}

void option_print (std::vector<std::string>& cmd, database& db, const flag& f)
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
	} else if (cmd[2] == "log") {
		print_df(db.df[index].sr_log, f, false);
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
		f.in_limit = IN_LIMIT_DEFAULT;;
	} else if (cmd[1][0] == '-') {
		std::cout << "Cannot set input limit to negative numbers\n";
	} else {
		try {
			n = static_cast<std::string::size_type>(
				std::stoul(cmd[1])
			);
		} catch (const std::invalid_argument& ex) {
			std::cout << "ERROR: std::invalid_argument\nCannot convert \""
			          << cmd[1]
			          << "\" into an integer.\n";
			return;
		} catch (const std::out_of_range& ex) {
			std::cout << "ERROR: std::out_of_range\n\""
			          << cmd[1]
			          << "\" cannot fit into an unsigned long.\nIt must be from "
			          << UL_MIN << " to " << UL_MAX << '\n';
			return;
		}
		if (n <= IN_LIMIT_MIN) {
			std::cout << "Cannot set input limit to "
			          << IN_LIMIT_MIN
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
		std::cout.precision(PRECISION_DEFAULT);
		f.precision = PRECISION_DEFAULT;
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
			          << I_MIN << " to " << I_MAX << '\n';
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
	s.erase(0, s.find_first_not_of(WHITESPACE));
	s.erase(s.find_last_not_of(WHITESPACE) + 1);
	if (s.empty())
		return false;
	str_cur_end = s.find_first_of(WHITESPACE);
	if (str_cur_end == std::string::npos)
		str_cur_end = s.size();
	while (str_cur_beg != std::string::npos) {
		cmd.push_back(s.substr(str_cur_beg, str_cur_end - str_cur_beg));
		str_cur_end = s.find_first_of(WHITESPACE, str_cur_end);
		str_cur_beg = s.find_first_not_of(WHITESPACE, str_cur_end);
		str_cur_end = s.find_first_of(WHITESPACE, str_cur_beg);
	}
	if (cmd[0] == "exit" || cmd[0] == "q" || cmd[0] == "Q") {
		f.py_mode = false;
		return true;
	}
	if (cmd[0] == "help") {
		std::cout << "The Python interpreter is currently ";
		if (!f.py_mode)
			std::cout << "not ";
		std::cout << "running\n\n\"#>\" Python-interpreter mode running\n\"$>\" Python-interpreter mode NOT running\n\nCan be run in either mode:\nhelp                Print out this message\nexit|q              Exits Python-interpreter mode or progam\n                    depending on what mode you are in\npython              Enters Python-interpreter mode\nilimit <option>     Sets the maximum number input characters\n                    <option>\n                    (number)     Sets limit to that number\n                    default      Sets limit to the default limit\n                    print        Prints current input limit\nprecision <option>  Sets the precision of floating-point numbers\n                    <option>\n                    (number)     Sets precision to that number\n                    default      Sets precision to the default number\n                    print        Prints current set floating-point precision\nprint list          Prints list of dataframes with their index.\nprint <index> <type>  Prints dataframe of a type.\n                    <type>\n                    <empty>      Prints default dataframe\n                    dif          Prints difference dataframe\n                    per          Prints percentage dataframe\n                    log          Prints log dataframe\n                    up           Prints up or down dataframe\nlstm <index>        Runs LSTM on dataframe\n     <index_test>   <type>\n     <buffer>       index_test   Database use to test LSTM once done training\n     <gap>          buffer       Days between starts of training batches\n     <target>       gap          Days between trainging batch and target\n     <hidden_size>  target       Number of target days to be averaged\n     <epochs>       hidden_size  Number of neurons\n                    epochs       Number of training loops\nlstm <index>        Runs LSTM on dataframe including the month and the averages\n     <index_test>   of the three sub-dataframes as parameters.\n     <buffer>\n     <gap>\n     <target>\n     <hidden size>\n     <epochs>\n     <subindex> <subindex> <subindex>\n\nCan only be run in Python-interpreter mode:\ndebug               List python debug information\nget <ticker> <start_date> <end_date>  Get stock dataframe of TICKER\n                    Format:   get TICKER YYYY-MM-DD YYYY-MM-DD\n                    Example:  get AAPL 2023-01-01 2024-12-31\n\n";
	} else if (cmd[0] == "python") {
		if (f.py_mode) {
			std::cout << "Python interpreter is already running.\n";
		} else {
			f.py_mode = true;
			python_scope_guard(db, f);
		}
	} else if (cmd[0] == "precision") {
		option_precision(cmd, f);
	} else if (cmd[0] == "ilimit") {
		option_ilimit(cmd, f);
	} else if (cmd[0] == "print") {
		option_print(cmd, db, f);
	} else if (cmd[0] == "lstm") {
		option_lstm(cmd, db);
	} else if (cmd[0] == "debug" && check_py(f.py_mode)) {
		option_debug();
	} else if (cmd[0] == "get" && check_py(f.py_mode)) {
		try {
			python_get_df(cmd, db);
		} catch (pybind11::error_already_set const& ex) {
			std::cout << ex.what()
			          << "\nERROR: pybind11::error_already_set\nUnable to grab stock data!\n";
		}
	}

	return false;
}

/*
 * python_get_df(db[0], "AAPL", "2023-01-01", "2024-12-31");
 * python_get_df(db[1], "TSLA", "2023-01-01", "2024-12-31");
 * python_get_df(db[2], "AMZN", "2023-01-01", "2024-12-31");
 * python_get_df(db[3], "V",    "2023-01-01", "2024-12-31");
 * python_get_df(db[4], "MSFT", "2023-01-01", "2024-12-31");
 */