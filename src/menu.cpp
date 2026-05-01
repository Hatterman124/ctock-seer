#include "menu.h"
#include "tdatabase.h"
#include "lstm.h"
#include <pybind11/embed.h>
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <numbers>
#include <string>
#include <bitset>
#include <sstream>
#include <iostream>

command_pos::command_pos(list_operator op) : op {op}
{
	return;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	// Declare database and menu flags.
	tdatabase db;
	menu_flag flag;
	cmd_history cmd_hist;

	// Sets floating-point precision from menu flag.
	std::cout.precision(flag.precision);
	// Banner message
	std::cout << "        __             __      _____\n  _____/ /_____  _____/ /__   / ___/___  ___  _____\n / ___/ __/ __ \\/ ___/ //_/   \\__ \\/ _ \\/ _ \\/ ___/\n/ /__/ /_/ /_/ / /__/ ,<     ___/ /  __/  __/ /\n\\___/\\__/\\____/\\___/_/|_|   /____/\\___/\\___/_/\nctock Seer version 0.5.0\nPaul Lee Kim\n\nCalling Py_FinalizeEx multiple times can cause problems.\nNumPy will throw an exception.\nPlease try to enter Python-interpreter mode only once.\nhttps://docs.python.org/3/c-api/init.html#c.Py_FinalizeEx\n";
	// Calls menu_exception_catch() repeatedly until quit.
	while (menu_exception_catch(db, flag, cmd_hist)) {
		// Nothing
	}

	return 0;
}


inline bool menu_exception_catch(tdatabase   &db,
                                 menu_flag   &flag,
                                 cmd_history &cmd_hist)
{
	try {
		return menu(db, flag, cmd_hist);
	} catch (const std::invalid_argument &ex) {
		std::cout << ex.what() <<'\n';
	}

	return true;
}

bool menu(tdatabase &db, menu_flag &flag, cmd_history &cmd_hist)
{
	std::string s;

	s.reserve(flag.in_limit);
	if (flag.is_py_running)
		std::cout << "#> ";
	else
		std::cout << "$> ";
	if (!std::getline(std::cin, s)) {
		std::cout << '\n';
		return false;
	}
	if (s.size() > flag.in_limit) {
		std::cout << "The character input limit is "
		          << flag.in_limit << ".\n";
		return true;
	}

	// Separates string into commands separated by list operators
	// while respecting quotes.
	std::vector<command_pos> cmd_list {list_operator::END};
	std::string::size_type cmd_start {std::string::npos};
	char_status c_stat;
	loop_control str_to_cmd_val {loop_control::CONTINUE};

	for (std::string::size_type i {}; i < s.size(); ++i) {
		if (s[i] >= '!' && s[i] <= '~') {
			cmd_start = i;
			break;
		}
	}
	if (cmd_start == std::string::npos)
		return true;
	// The following string is not empty and not only whitespace.

	switch (s[cmd_start]) {
	default:
		c_stat.only_whitespace = false;
		break;
	case '\'':
		c_stat.quote_status = is_quoted::IN_SINGLE_QUOTE;
		c_stat.last_char = char_status::DEFAULT;
		c_stat.only_whitespace = false;
		break;
	case '\"':
		c_stat.quote_status = is_quoted::IN_DOUBLE_QUOTE;
		c_stat.last_char = char_status::DEFAULT;
		c_stat.only_whitespace = false;
		break;
	case ';':
		throw std::invalid_argument(INVALID_ARGUMENT_END);
	case '&':
		c_stat.last_char = char_status::UNESCAPED_AND;
		break;
	case '|':
		c_stat.last_char = char_status::UNESCAPED_OR;
		break;
	}
	while (str_to_cmd_val == loop_control::CONTINUE)
		str_to_cmd_val = seperate_str_to_cmd(flag,
		                                     s,
		                                     cmd_list,
		                                     cmd_start,
		                                     c_stat   );
	if (str_to_cmd_val == loop_control::ERROR)
		return false;
	cmd_hist.list.push_back(s);
	++cmd_hist.index;

	// Uncomment for debug information.
	/*
	std::cout << "--------------------------------------------------------------------------------\n";
	for (command_pos &tmp : cmd_list) {
		switch (tmp.op) {
		case list_operator::END:
			std::cout << "END\n";
			break;
		case list_operator::AND:
			std::cout << "AND\n";
			break;
		case list_operator::OR:
			std::cout << "OR\n";
			break;
		}
		for (std::string &tmp_s : tmp.arg_list) {
			std::cout << '\"' << tmp_s << "\"\n";
		}
		std::cout << "--------------------------------------------------------------------------------\n";
	}
	*/

	// Run commands.

	// This can be uninitialized, ignore warnings.
	int return_val;

	for (command_pos &tmp : cmd_list) {
		if ((tmp.op     == list_operator::AND &&
		     return_val != 0                    ) ||
		    (tmp.op     == list_operator::OR &&
		     return_val == 0                   )    )
			break;
		return_val = run_command(db, flag, cmd_hist, tmp.arg_list);
		if (flag.exit) {
			flag.exit = false;
			return false;
		}
	}

	return true;
}

loop_control seperate_str_to_cmd(const menu_flag &flag,
                               std::string &s,
                               std::vector<command_pos> &cmd_list,
                               std::string::size_type   &cmd_start,
                               char_status &c_stat)
{
	for (std::string::size_type i {cmd_start}; i < s.size(); ++i)
		seperate_str_body_to_cmd(i, s, cmd_list, c_stat);
	if (c_stat.quote_status == is_quoted::IN_SINGLE_QUOTE ||
	    c_stat.quote_status == is_quoted::IN_DOUBLE_QUOTE ||
	    c_stat.last_char    == char_status::ESCAPE          ) {
		std::string buffer {};

		std::cout << "> ";
		if (!std::getline(std::cin, buffer)) {
			std::cout << '\n';
			return loop_control::ERROR;
		}
		cmd_start = s.size();
		s.append(buffer);
		if (s.size() > flag.in_limit) {
			std::cout << "The character input limit is "
			          << flag.in_limit << ".\n";
			return loop_control::ERROR;
		}
		return loop_control::CONTINUE;
	} else if (c_stat.last_char == char_status::UNESCAPED_AND) {
		throw std::invalid_argument(INVALID_ARGUMENT_AND);
	} else if (c_stat.last_char == char_status::DOUBLE_AND) {
		throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_AND);
	} else if (c_stat.last_char == char_status::UNESCAPED_OR) {
		throw std::invalid_argument(INVALID_ARGUMENT_OR);
	} else if (c_stat.last_char == char_status::DOUBLE_OR) {
		throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_OR);
	}

	return loop_control::BREAK;
}

void seperate_str_body_to_cmd(const std::string::size_type i,
                              const std::string &s,
                              std::vector<command_pos> &cmd_list,
                              char_status &c_stat)
{
	// If in single quotes.
	// All characters are literal except for another ''' which ends quote.
	if (c_stat.quote_status == is_quoted::IN_SINGLE_QUOTE) {
		if (s[i] == '\'') {
			c_stat.quote_status = is_quoted::NOT_QUOTED;
			c_stat.last_char = char_status::DEFAULT;
			c_stat.in_arg = false;
		} else {
			cmd_list[
				c_stat.cmd_index
			].arg_list[
				c_stat.arg_index
			].push_back(s[i]);
		}
	// If this character is escaped.
	} else if (c_stat.last_char == char_status::ESCAPE) {
		cmd_list[
			c_stat.cmd_index
		].arg_list[
			c_stat.arg_index
		].push_back(s[i]);
		c_stat.last_char = char_status::DEFAULT;
	// If found escape character.
	} else if (s[i] == '\\') {
			c_stat.last_char = char_status::ESCAPE;
			c_stat.only_whitespace = false;
	// If in double quotes.
	// Escape works as normal.
	} else if (c_stat.quote_status == is_quoted::IN_DOUBLE_QUOTE) {
		if (s[i] == '\"') {
			c_stat.quote_status = is_quoted::NOT_QUOTED;
			c_stat.last_char = char_status::DEFAULT;
			c_stat.in_arg = false;
		}
		else {
			cmd_list[
				c_stat.cmd_index
			].arg_list[
				c_stat.arg_index
			].push_back(s[i]);
		}

	// The following characters are not in quotes, escaped, or is backslash.

	// If found beginning single quote.
	} else if (s[i] == '\'') {
		if (!c_stat.in_arg) {
			cmd_list[c_stat.cmd_index].arg_list.push_back("");
			c_stat.in_arg = true;
			++c_stat.arg_index;
		}
		switch (c_stat.last_char) {
		case char_status::DEFAULT:
			c_stat.quote_status = is_quoted::IN_SINGLE_QUOTE;
			c_stat.last_char = char_status::DEFAULT;
			c_stat.only_whitespace = false;
			break;
		case char_status::UNESCAPED_AND:
			throw std::invalid_argument(INVALID_ARGUMENT_AND);
		case char_status::UNESCAPED_OR:
			throw std::invalid_argument(INVALID_ARGUMENT_OR);
		case char_status::DOUBLE_AND:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_AND);
		case char_status::DOUBLE_OR:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_OR);
		default:
			// Nothing
			break;
		};
	// If found beginning double quote.
	} else if (s[i] == '\"') {
		if (!c_stat.in_arg) {
			cmd_list[c_stat.cmd_index].arg_list.push_back("");
			c_stat.in_arg = true;
			++c_stat.arg_index;
		}
		switch (c_stat.last_char) {
		case char_status::DEFAULT:
			c_stat.quote_status = is_quoted::IN_DOUBLE_QUOTE;
			c_stat.last_char = char_status::DEFAULT;
			c_stat.only_whitespace = false;
			break;
		case char_status::UNESCAPED_AND:
			throw std::invalid_argument(INVALID_ARGUMENT_AND);
		case char_status::UNESCAPED_OR:
			throw std::invalid_argument(INVALID_ARGUMENT_OR);
		case char_status::DOUBLE_AND:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_AND);
		case char_status::DOUBLE_OR:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_OR);
		default:
			// Nothing
			break;
		};
	// If found command end.
	} else if (s[i] == ';') {
		if (c_stat.only_whitespace)
			throw std::invalid_argument(INVALID_ARGUMENT_END);
		switch (c_stat.last_char) {
		case char_status::DEFAULT:
			cmd_list.push_back(list_operator::END);
			c_stat.last_char = char_status::DEFAULT;
			c_stat.only_whitespace = true;
			c_stat.in_arg    = false;
			++c_stat.cmd_index;
			c_stat.arg_index = 0;
			break;
		case char_status::UNESCAPED_AND:
			throw std::invalid_argument(INVALID_ARGUMENT_AND);
		case char_status::UNESCAPED_OR:
			throw std::invalid_argument(INVALID_ARGUMENT_OR);
		case char_status::DOUBLE_AND:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_AND);
		case char_status::DOUBLE_OR:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_OR);
		default:
			// Nothing
			break;
		};
	// If found possible AND list operator.
	} else if (s[i] == '&') {
		switch (c_stat.last_char) {
		case char_status::DEFAULT:
			c_stat.last_char = char_status::UNESCAPED_AND;
			break;
		case char_status::UNESCAPED_AND:
			cmd_list.push_back(list_operator::AND);
			c_stat.last_char = char_status::DOUBLE_AND;
			c_stat.only_whitespace = true;
			c_stat.in_arg    = false;
			++c_stat.cmd_index;
			c_stat.arg_index = 0;
			break;
		case char_status::UNESCAPED_OR:
			throw std::invalid_argument(INVALID_ARGUMENT_OR);
		case char_status::DOUBLE_AND:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_AND);
		case char_status::DOUBLE_OR:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_OR);
		default:
			// Nothing
			break;
		};
	// If found possible OR list operator.
	} else if (s[i] == '|') {
		switch (c_stat.last_char) {
		case char_status::DEFAULT:
			c_stat.last_char = char_status::UNESCAPED_OR;
			break;
		case char_status::UNESCAPED_OR:
			cmd_list.push_back(list_operator::OR);
			c_stat.last_char = char_status::DOUBLE_OR;
			c_stat.only_whitespace = true;
			c_stat.in_arg    = false;
			++c_stat.cmd_index;
			c_stat.arg_index = 0;
			break;
		case char_status::UNESCAPED_AND:
			throw std::invalid_argument(INVALID_ARGUMENT_AND);
		case char_status::DOUBLE_AND:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_AND);
		case char_status::DOUBLE_OR:
			throw std::invalid_argument(INVALID_ARGUMENT_DOUBLE_OR);
		default:
			// Nothing
			break;
		};

	// All the following characters are regular characters.

	// If previous character was a '&' that was not escaped.
	} else if (c_stat.last_char == char_status::UNESCAPED_AND) {
		throw std::invalid_argument(INVALID_ARGUMENT_AND);
	// If previous character was a '|' that was not escaped.
	} else if (c_stat.last_char == char_status::UNESCAPED_OR) {
		throw std::invalid_argument(INVALID_ARGUMENT_OR);
	} else if (s[i] >= '!' && s[i] <= '~') {
		if (!c_stat.in_arg) {
			cmd_list[c_stat.cmd_index].arg_list.push_back("");
			c_stat.in_arg = true;
		}
		cmd_list[
			c_stat.cmd_index
		].arg_list[
			c_stat.arg_index
		].push_back(s[i]);
		if (c_stat.only_whitespace && s[i] >= '!' && s[i] <= '~')
			c_stat.only_whitespace = false;
		c_stat.last_char = char_status::DEFAULT;
	// If char is whitespace
	} else {
		if (c_stat.in_arg) {
			++c_stat.arg_index;
			c_stat.in_arg    = false;
		}
		c_stat.last_char = char_status::DEFAULT;
	}

	// Uncomment for debug information.
	/*
	std::cout << i << ": \'" << s[i] << "\'\n";
	switch (c_stat.quote_status) {
	case is_quoted::NOT_QUOTED:
		std::cout << "c_stat.quote_status    = is_quoted::NOT_QUOTED\n";
		break;
	case is_quoted::IN_SINGLE_QUOTE:
		std::cout << "c_stat.quote_status    = is_quoted::IN_SINGLE_QUOTE\n";
		break;
	case is_quoted::IN_DOUBLE_QUOTE:
		std::cout << "c_stat.quote_status    = is_quoted::IN_DOUBLE_QUOTE\n";
		break;
	}
	switch (c_stat.last_char) {
	case char_status::DEFAULT:
		std::cout << "c_stat.last_char       = char_status::DEFAULT\n";
		break;
	case char_status::ESCAPE:
		std::cout << "c_stat.last_char       = char_status::ESCAPE\n";
		break;
	case char_status::UNESCAPED_AND:
		std::cout << "c_stat.last_char       = char_status::UNESCAPED_AND\n";
		break;
	case char_status::DOUBLE_AND:
		std::cout << "c_stat.last_char       = char_status::DOUBLE_AND\n";
		break;
	case char_status::UNESCAPED_OR:
		std::cout << "c_stat.last_char       = char_status::UNESCAPED_AND\n";
		break;
	case char_status::DOUBLE_OR:
		std::cout << "c_stat.last_char       = char_status::DOUBLE_AND\n";
		break;
	}
	if (c_stat.only_whitespace)
		std::cout << "c_stat.only_whitespace = true\n";
	else
		std::cout << "c_stat.only_whitespace = false\n";
	if (c_stat.in_arg)
		std::cout << "c_stat.in_arg          = true\n";
	else
		std::cout << "c_stat.in_arg          = false\n";
	std::cout << "c_stat.cmd_index       = " << c_stat.cmd_index << '\n'
	          << "c_stat.arg_index       = " << c_stat.arg_index << '\n'
	          << "arg_list.size()        = "
	          << cmd_list[c_stat.cmd_index].arg_list.size() << '\n';
	*/

	return;
}

int run_command(tdatabase   &db,
                menu_flag   &flag,
                cmd_history &cmd_hist,
                const std::vector<std::string> &arg)
{
	try {
		if (arg[0] == "help") {
			return option_help(flag);
		} else if (arg[0] == "exit" || arg[0] == "q") {
			flag.exit = true;
			return 0;
		} else if (arg[0] == "true") {
			return 0;
		} else if (arg[0] == "false") {
			return 1;
		} else if (arg[0] == "echo") {
			return option_echo(arg);
		} else if (arg[0] == "python") {
			return option_python(db, flag, cmd_hist);
		} else if (arg[0] == "history") {
			return option_history(cmd_hist, arg);
		} else if (arg[0] == "ilimit") {
			return option_ilimit(flag, arg);
		} else if (arg[0] == "precision") {
			return option_precision(flag, arg);
		} else if (arg[0] == "pi") {
			std::cout << std::numbers::pi_v<long double> << '\n';
			return 0;
		} else if (arg[0] == "print") {
			return option_print(db, flag, arg);
		} else if (arg[0] == "lstm") {
			return option_lstm(db, arg);
		} else if (arg[0] == "debug") {
			return option_debug(flag);
		} else if (arg[0] == "get") {
			return option_get(db, flag, arg);
		} else {
			std::cout << arg[0] << ": command not found\n";
			return 127;
		}
	} catch (const std::invalid_argument &ex) {
		std::cout << ex.what() <<'\n';
		return 0;
	} catch (const std::domain_error &ex) {
		std::cout << ex.what() <<'\n';
		return 0;
	}

	std::unreachable();
}

int option_help(const menu_flag &flag)
{
	std::cout << "The Python interpreter is currently ";
	if (!flag.is_py_running)
		std::cout << "not ";
	std::cout << "running\n\n\"#>\" Python-interpreter mode running\n\"$>\" Python-interpreter mode NOT running\n\nOPERATORS:\n;                   Runs command after\n                    Example: echo foo; echo bar\n                             Will run both echo commands\n&&                  Runs command if last-executed command was successful.\n                    Example: true && echo foo; false && echo bar\n                             Will run echo foo but not echo bar\n||                  Runs command if last-executed command was not successful.\n                    Example: true || echo foo; false || echo bar\n                             Will NOT run echo foo but will run echo bar\n                    Example: true || echo foo || echo bar\n                             Will NOT run any echo commands as technically\n                             the last-executed command for both is\n                             the first one\n\nShort options such as -c and -i can only be in the first argument after\nthe command. They can be combined such as -ci. Having -- in the first argument\nafter the command will make the command ignore it.\nExample: echo -q -n foo\n         Will output \"-n\" \"foo\" since only -q is accepted as an option\nExample: echo -qn foo\n         Will outout \"foo\" without tailing newline since both -q and -n\n         are accepted as options\nExample: echo -- -qn foo\n         Will outout -qn foo since no options are taken\n\nCOMMANDS:\nCan be run in either mode:\nhelp                Prints out this message\nexit|q              Exits Python-interpreter mode or program\n                    depending on what mode you are in\ntrue                Does nothing and returns true\nfalse               Does nothing and returns false\necho <option>       Prints its arguments\n                    <option>\n                    -n           Do not output the trailing newline\n                    -q           Put quotation marks around each argument\npython              Enters Python-interpreter mode\nhistory <option>    Prints entered commands\n                    <option>\n                    (number)     Number of commands to list\n                    clear        Deletes all history\nilimit <option>     Sets the maximum number of input characters\n                    <option>\n                    (number)     Sets limit to that number\n                    default      Sets limit to the default limit\n                    print        Prints current input limit\nprecision <option>  Sets the precision of floating-point numbers\n                    <option>\n                    (number)     Sets precision to that number\n                    default      Sets precision to the default number\n                    print        Prints current set floating-point precision\npi                  Prints the irrational number pi\n                    Used to test precision\nprint list          Prints list of dataframes with their index.\nprint <index>       Prints dataframe of a type.\ngraph <index>       Print a line graph of a dataframe\n      <width>       width        Number of characters the width the graph is\n      <height>      height       Number of characters the height the graph is\nlstm <option>       Runs LSTM on dataframe\n     <index>        <option>\n     <index_test>   -s           Run silently, only output final MSE\n     <batch>        <type>\n     <buffer>       index        Database use to train LSTM\n     <gap>          index_test   Database use to test LSTM once done training\n     <target>       batch        Days to use as training\n     <hidden_size>  buffer       Days between starts of training batches\n     <epochs>       gap          Days between training batch and target\n                    target       Number of target days to be averaged\n                    hidden_size  Number of neurons\n                    epochs       Number of training loops\n\nCan only be run in Python-interpreter mode:\ndebug               List python debug information\nget <ticker> <start_date> <end_date>  Get stock dataframe of TICKER\n                    Format:   get TICKER YYYY-MM-DD YYYY-MM-DD\n                    Example:  get AAPL 2023-01-01 2024-12-31\n\n";

	return 0;
}

int option_echo(const std::vector<std::string> &arg)
{
	if (arg.size() == 1) {
		std::cout << '\n';
		return 0;
	}

	std::bitset<53>                     option {get_options(arg[1])};
	std::vector<std::string>::size_type start  {1};

	if (arg[1] == "--" ||
	    (((option.to_ullong() &
	       0b1111'11111111'11111111'11111111'11111111'11111111'11111111ull)
	      > 0ull) &&
	     ((option.to_ullong() &
	       0b1111'11111011'01111111'11111111'11111111'11111111'11111111ull)
	      == 0ull)                                                        ))
		++start;
	else
		option.reset();

	if (option[42] && arg.size() > 2) {
		std::cout << '\"' << arg[start++];
		for (std::vector<std::string>::size_type i {start};
		     i < arg.size();
		     ++i)
			std::cout << "\" \"" << arg[i];
		std::cout << '\"';
	} else {
		std::cout << arg[start++];
		for (std::vector<std::string>::size_type i {start};
		     i < arg.size();
		     ++i)
			std::cout << ' ' << arg[i];
	}
	if (!option[39])
		std::cout << '\n';

	return 0;
}

int option_python(tdatabase &db,
                  menu_flag &flag,
                  cmd_history &cmd_hist)
{
	if (flag.is_py_running) {
		std::cout << "The Python interpreter is already running\n";
		return 1;
	}
	pybind11::scoped_interpreter guard {};
	flag.is_py_running = true;
	while (menu_exception_catch(db, flag, cmd_hist)) {
		// Nothing
	}
	flag.is_py_running = false;

	return 0;
}

int option_history(cmd_history &cmd_hist, const std::vector<std::string> &arg)
{
	std::vector<std::string>::size_type start {1};

	if (arg.size() >= 2 && arg[1] == "--")
		++start;
	if (arg.size() > start + 1) {
		std::cout << "Too many arguments\n";
		return 2;
	}

	std::vector<std::string>::size_type limit {};

	if (arg.size() == start + 1) {
		if (arg[1] == "clear") {
			cmd_hist.list.clear();
			cmd_hist.index = 0;
			return 0;
		}

		std::vector<std::string>::size_type n {
			arg_to_n<std::vector<std::string>::size_type>(arg[1])
		};

		if (cmd_hist.list.size() > n)
			limit = cmd_hist.list.size() - n;
	}
	for (std::vector<std::string>::size_type i {limit};
	     i < cmd_hist.list.size();
	     ++i)
		std::cout << ' ' << i << ": " << cmd_hist.list[i] << '\n';

	return 0;
}

int option_ilimit(menu_flag &flag, const std::vector<std::string> &arg)
{
	if (arg.size() == 1) {
		std::cout << "Not enough arguments\n";
		return 2;
	}

	std::vector<std::string>::size_type start {1};

	if (arg[1] == "--")
		++start;
	if (arg.size() > start + 1) {
		std::cout << "Too many arguments\n";
		return 2;
	}
	if (arg[1] == "default") {
		flag.in_limit = IN_LIMIT_DEFAULT;
	} else if (arg[1] == "print") {
		std::cout << flag.in_limit << '\n';
	} else {
		std::string::size_type n {
			arg_to_n<std::string::size_type>(arg[1])
		};
		if (n >= IN_LIMIT_MIN) {
			flag.in_limit = n;
		} else {
			std::cout << "Input limit must be equal or greater than "
			          << IN_LIMIT_MIN << " characters.\n";
			return 2;
		}
	}

	return 0;
}

int option_precision(menu_flag &flag, const std::vector<std::string> &arg)
{
	if (arg.size() == 1) {
		std::cout << "Not enough arguments\n";
		return 2;
	}

	std::vector<std::string>::size_type start {1};

	if (arg[1] == "--")
		++start;
	if (arg.size() > start + 1) {
		std::cout << "Too many arguments\n";
		return 2;
	}
	if (arg[1] == "default") {
		std::cout.precision(PRECISION_DEFAULT);
		flag.precision = PRECISION_DEFAULT;
	} else if (arg[1] == "print") {
		std::cout << flag.precision << '\n';
	} else {
		int n {arg_to_n<int>(arg[1])};
		std::cout.precision(n);
		flag.precision = n;
	}

	return 0;
}

int option_print(const tdatabase &db,
                 const menu_flag &flag,
                 const std::vector<std::string> &arg)
{
	if (arg.size() == 1) {
		std::cout << "Not enough arguments\n";
		return 2;
	}

	std::vector<std::string>::size_type start {1};

	if (arg[1] == "--")
		++start;
	if (arg.size() > start + 1) {
		std::cout << "Too many arguments\n";
		return 2;
	}

	if (arg[start] == "list") {
		db.print();
	} else {
		std::vector<tdataframe*>::size_type index {
			arg_to_n<std::vector<tdataframe*>::size_type>(arg[start])
		};

		if (index < db.size) {
			db.df[index]->print(flag.precision);
		} else {
			std::cout << "Dataframe " << index << " not found\n";
			return 1;
		}
	}

	return 0;
}

int option_lstm(const tdatabase &db, const std::vector<std::string> &arg)
{
	if (arg.size() < 9) {
		std::cout << "Not enough arguments\n";
		return 2;
	}
	if (arg.size() > 10) {
		std::cout << "Too many arguments\n";
		return 2;
	}

	std::bitset<53>  option {get_options(arg[1])};
	bool option_exist {};

	if (arg[1] == "--" ||
	    (((option.to_ullong() &
	       0b1111'11111111'11111111'11111111'11111111'11111111'11111111ull)
	      > 0ull) &&
	     ((option.to_ullong() &
	       0b1111'11111011'11111111'11111111'11111111'11111111'11111111ull)
	      == 0ull)                                                        ))
	{
		option_exist = true;
	} else if (arg[1][0] == '-') {
		std::cout << "Unrecognized option: " << arg[1] << '\n';
		return 2;
	}

	if (option_exist) {
		if (arg.size() < 10) {
			std::cout << "Not enough arguments\n";
			return 2;
		}
		if (option[42])
			return run_lstm_q(
				// tdataframe &df
				*(db.df[arg_to_n<
					std::vector<tdataframe*>::size_type
				>(arg[2])]),
				// tdataframe &df_test
				*(db.df[arg_to_n<
					std::vector<tdataframe*>::size_type
				>(arg[3])]),
				// std::vector<stockrow>::size_type batch
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[4]),
				// std::vector<stockrow>::size_type buffer
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[5]),
				// std::vector<stockrow>::size_type gap
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[6]),
				// std::vector<stockrow>::size_type target
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[7]),
				// std::int64_t hidden_size
				arg_to_n<std::int64_t>(arg[8]),
				// std::int64_t epochs
				arg_to_n<std::int64_t>(arg[9])
			);
		else
			return run_lstm(
				// tdataframe &df
				*(db.df[arg_to_n<
					std::vector<tdataframe*>::size_type
				>(arg[2])]),
				// tdataframe &df_test
				*(db.df[arg_to_n<
					std::vector<tdataframe*>::size_type
				>(arg[3])]),
				// std::vector<stockrow>::size_type batch
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[4]),
				// std::vector<stockrow>::size_type buffer
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[5]),
				// std::vector<stockrow>::size_type gap
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[6]),
				// std::vector<stockrow>::size_type target
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[7]),
				// std::int64_t hidden_size
				arg_to_n<std::int64_t>(arg[8]),
				// std::int64_t epochs
				arg_to_n<std::int64_t>(arg[9])
			);
	} else {
		if (arg.size() > 9) {
			std::cout << "Too many arguments\n";
			return 2;
		}
		if (option[42])
			return run_lstm_q(
				// tdataframe &df
				*(db.df[arg_to_n<
					std::vector<tdataframe*>::size_type
				>(arg[1])]),
				// tdataframe &df_test
				*(db.df[arg_to_n<
					std::vector<tdataframe*>::size_type
				>(arg[2])]),
				// std::vector<stockrow>::size_type batch
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[3]),
				// std::vector<stockrow>::size_type buffer
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[4]),
				// std::vector<stockrow>::size_type gap
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[5]),
				// std::vector<stockrow>::size_type target
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[6]),
				// std::int64_t hidden_size
				arg_to_n<std::int64_t>(arg[7]),
				// std::int64_t epochs
				arg_to_n<std::int64_t>(arg[8])
			);
		else
			return run_lstm(
				// tdataframe &df
				*(db.df[arg_to_n<
					std::vector<tdataframe*>::size_type
				>(arg[1])]),
				// tdataframe &df_test
				*(db.df[arg_to_n<
					std::vector<tdataframe*>::size_type
				>(arg[2])]),
				// std::vector<stockrow>::size_type batch
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[3]),
				// std::vector<stockrow>::size_type buffer
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[4]),
				// std::vector<stockrow>::size_type gap
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[5]),
				// std::vector<stockrow>::size_type target
				arg_to_n<
					std::vector<stockrow>::size_type
				>(arg[6]),
				// std::int64_t hidden_size
				arg_to_n<std::int64_t>(arg[7]),
				// std::int64_t epochs
				arg_to_n<std::int64_t>(arg[8])
			);
	}
}

int option_debug(menu_flag &flag)
{
	if (!flag.is_py_running) {
		std::cout << "The Python interpreter is not running\n";
		return 1;
	}
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

	return 0;
}

int option_get(tdatabase &db,
               const menu_flag &flag,
               const std::vector<std::string> &arg)
{
	if (!flag.is_py_running) {
		std::cout << "The Python interpreter is not running\n";
		return 1;
	}

	std::vector<std::string>::size_type start {1};

	if (arg[1] == "--")
		++start;
	if (arg.size() > start + 3) {
		std::cout << "Too many arguments\n";
		return 1;
	}

	return db.get(arg[1], arg[2], arg[3]);
}

std::bitset<53> get_options(const std::string &s)
{
	if (s[0] != '-')
		return std::bitset<53> {0};

	std::bitset<53> arg_opt {};
	std::size_t     index;

	for (std::string::size_type i {1}; i < s.size(); ++i) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			index = s[i] - 65;
			arg_opt[index] = true;
		} else if (s[i] >= 'a' && s[i] <= 'z') {
			index = s[i] - 71;
			arg_opt[index] = true;
		} else {
			arg_opt[52] = true;
			break;
		}
	}

	return arg_opt;
}

template<typename T>
T arg_to_n(const std::string &s)
{
	for (std::vector<std::string>::size_type i {}; i < s.size(); ++i)
		if (s[i] < '0' || s[i] > '9')
			throw std::domain_error("std::domain_error\nNot a positive number");

	std::stringstream ss {s};
	T n;

	ss >> n;

	return n;
}
