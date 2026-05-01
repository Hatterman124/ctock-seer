#ifndef MENU_H
#define MENU_H

#include "tdatabase.h"
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <bitset>

constexpr std::string::size_type IN_LIMIT_DEFAULT {2'097'152};
constexpr std::string::size_type IN_LIMIT_MIN {16};
constexpr int                    PRECISION_DEFAULT {11};
constexpr const char *INVALID_ARGUMENT_END {
	"std::invalid_argument\nsyntax error near unexpected token \';\'"
};
constexpr const char *INVALID_ARGUMENT_AND {
	"std::invalid_argument\nsyntax error near unexpected token \'&\'"
};
constexpr const char *INVALID_ARGUMENT_DOUBLE_AND {
	"std::invalid_argument\nsyntax error near unexpected token \"&&\""
};
constexpr const char *INVALID_ARGUMENT_OR {
	"std::invalid_argument\nsyntax error near unexpected token \'|\'"
};
constexpr const char *INVALID_ARGUMENT_DOUBLE_OR {
	"std::invalid_argument\nsyntax error near unexpected token \'||\'"
};
enum class list_operator {END, AND, OR};
enum class is_quoted     {NOT_QUOTED, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE};
enum class loop_control  {BREAK, CONTINUE, ERROR};
/*
enum class option_char : unsigned long long {
	U_A = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000001,
	U_B = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000010,
	U_C = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000100,
	U_D = 0b00000000'00000000'00000000'00000000'00000000'00000000'00001000,
	U_E = 0b00000000'00000000'00000000'00000000'00000000'00000000'00010000,
	U_F = 0b00000000'00000000'00000000'00000000'00000000'00000000'00100000,
	U_G = 0b00000000'00000000'00000000'00000000'00000000'00000000'01000000,
	U_H = 0b00000000'00000000'00000000'00000000'00000000'00000000'10000000,
	U_I = 0b00000000'00000000'00000000'00000000'00000000'00000001'00000000,
	U_J = 0b00000000'00000000'00000000'00000000'00000000'00000010'00000000,
	U_K = 0b00000000'00000000'00000000'00000000'00000000'00000100'00000000,
	U_L = 0b00000000'00000000'00000000'00000000'00000000'00001000'00000000,
	U_M = 0b00000000'00000000'00000000'00000000'00000000'00010000'00000000,
	U_N = 0b00000000'00000000'00000000'00000000'00000000'00100000'00000000,
	U_O = 0b00000000'00000000'00000000'00000000'00000000'01000000'00000000,
	U_P = 0b00000000'00000000'00000000'00000000'00000000'10000000'00000000,
	U_Q = 0b00000000'00000000'00000000'00000000'00000001'00000000'00000000,
	U_R = 0b00000000'00000000'00000000'00000000'00000010'00000000'00000000,
	U_S = 0b00000000'00000000'00000000'00000000'00000100'00000000'00000000,
	U_T = 0b00000000'00000000'00000000'00000000'00001000'00000000'00000000,
	U_U = 0b00000000'00000000'00000000'00000000'00010000'00000000'00000000,
	U_V = 0b00000000'00000000'00000000'00000000'00100000'00000000'00000000,
	U_W = 0b00000000'00000000'00000000'00000000'01000000'00000000'00000000,
	U_X = 0b00000000'00000000'00000000'00000000'10000000'00000000'00000000,
	U_Y = 0b00000000'00000000'00000000'00000001'00000000'00000000'00000000,
	U_Z = 0b00000000'00000000'00000000'00000010'00000000'00000000'00000000,
	L_A = 0b00000000'00000000'00000000'00000100'00000000'00000000'00000000,
	L_B = 0b00000000'00000000'00000000'00001000'00000000'00000000'00000000,
	L_C = 0b00000000'00000000'00000000'00010000'00000000'00000000'00000000,
	L_D = 0b00000000'00000000'00000000'00100000'00000000'00000000'00000000,
	L_E = 0b00000000'00000000'00000000'01000000'00000000'00000000'00000000,
	L_F = 0b00000000'00000000'00000000'10000000'00000000'00000000'00000000,
	L_G = 0b00000000'00000000'00000001'00000000'00000000'00000000'00000000,
	L_H = 0b00000000'00000000'00000010'00000000'00000000'00000000'00000000,
	L_I = 0b00000000'00000000'00000100'00000000'00000000'00000000'00000000,
	L_J = 0b00000000'00000000'00001000'00000000'00000000'00000000'00000000,
	L_K = 0b00000000'00000000'00010000'00000000'00000000'00000000'00000000,
	L_L = 0b00000000'00000000'00100000'00000000'00000000'00000000'00000000,
	L_M = 0b00000000'00000000'01000000'00000000'00000000'00000000'00000000,
	L_N = 0b00000000'00000000'10000000'00000000'00000000'00000000'00000000,
	L_O = 0b00000000'00000001'00000000'00000000'00000000'00000000'00000000,
	L_P = 0b00000000'00000010'00000000'00000000'00000000'00000000'00000000,
	L_Q = 0b00000000'00000100'00000000'00000000'00000000'00000000'00000000,
	L_R = 0b00000000'00001000'00000000'00000000'00000000'00000000'00000000,
	L_S = 0b00000000'00010000'00000000'00000000'00000000'00000000'00000000,
	L_T = 0b00000000'00100000'00000000'00000000'00000000'00000000'00000000,
	L_U = 0b00000000'01000000'00000000'00000000'00000000'00000000'00000000,
	L_V = 0b00000000'10000000'00000000'00000000'00000000'00000000'00000000,
	L_W = 0b00000001'00000000'00000000'00000000'00000000'00000000'00000000,
	L_X = 0b00000010'00000000'00000000'00000000'00000000'00000000'00000000,
	L_Y = 0b00000100'00000000'00000000'00000000'00000000'00000000'00000000,
	L_Z = 0b00001000'00000000'00000000'00000000'00000000'00000000'00000000,
};
*/

// Holds the menu state flags.
struct menu_flag {
	// If user wants to exit.
	bool exit {};

	// Running state of Python Interpreter.
	bool is_py_running {};

	// Command input length limit at menu.
	std::string::size_type in_limit {IN_LIMIT_DEFAULT};

	// Floating-point number precision.
	int precision {PRECISION_DEFAULT};
};

// Holds past commands imputed.
struct cmd_history {
	std::vector<std::string> list {};
	// If user wants to up arrow through commands.
	std::vector<std::string>::size_type index {};
};

// Holds start and end position of one command and the list operator.
struct command_pos {
	// Constructor
	command_pos(list_operator op);
	// List of positions for each argument in command.
	std::vector<std::string> arg_list {};
	// The list operator before the command.
	list_operator op;
};

// Holds if a char is escaped or in quotes.
struct char_status {
	enum last_character {DEFAULT,
	                     ESCAPE,
	                     UNESCAPED_AND, DOUBLE_AND,
	                     UNESCAPED_OR, DOUBLE_OR};
	// If char is in quotes.
	is_quoted quote_status {is_quoted::NOT_QUOTED};
	// What the last char was.
	last_character last_char {char_status::DEFAULT};
	// If there is a at least one non whitespace character since cmd_beg.
	bool only_whitespace {true};
	// If char is part of argument.
	bool in_arg {};
	// Holds index of current command.
	std::vector<command_pos>::size_type cmd_index {};
	// Holds index of current argument.
	std::vector<command_pos>::size_type arg_index {};
};

// Where ctock seer starts.
// Calls menu_exception_catch() repeatedly.
int main(int argc, char *argv[]);

// Calls menu() and catches any exceptions it throws.
inline bool menu_exception_catch(tdatabase   &db,
                                 menu_flag   &flag,
                                 cmd_history &cmd_hist);

// The start of the CLI menu for stock seer.
bool menu(tdatabase &db, menu_flag &flag, cmd_history &cmd_hist);

// Separates string into commands separated by list operators
// while respecting single, double quotes, and escape slash.
// Must guarantee that s.size() != 0, i != 0, and c_stat is preset.
loop_control seperate_str_to_cmd(const menu_flag &flag,
                                 std::string &s,
                                 std::vector<command_pos> &cmd_list,
                                 std::string::size_type   &cmd_start,
                                 char_status &c_stat);;

void seperate_str_body_to_cmd(const std::string::size_type i,
                              const std::string &s,
                              std::vector<command_pos> &cmd_list,
                              char_status &c_stat
);

// Finds and runs command listed in arg with its arguments.
int run_command(tdatabase   &db,
                menu_flag   &flag,
                cmd_history &cmd_hist,
                const std::vector<std::string> &arg);

// Prints help message.
int option_help(const menu_flag &flag);

// Prints out its arguments.
int option_echo(const std::vector<std::string> &arg);

int option_python(tdatabase   &db,
                  menu_flag   &flag,
                  cmd_history &cmd_hist);

// Show history of last commands.
int option_history(cmd_history &cmd_hist, const std::vector<std::string> &arg);

// Sets the maximum number of input characters.
int option_ilimit(menu_flag &flag, const std::vector<std::string> &arg);

// Sets the precision of floating-point numbers.
int option_precision(menu_flag &flag, const std::vector<std::string> &arg);

// Prints list of dataframes or dataframe itself.
int option_print(const tdatabase &db,
                 const menu_flag &flag,
                 const std::vector<std::string> &arg);

// Runs LSTM on dataframe
int option_lstm(const tdatabase &db, const std::vector<std::string> &arg);

// List python debug information.
int option_debug(menu_flag &flag);

// Get stock dataframe of TICKER.
int option_get(tdatabase &db,
               const menu_flag &flag,
               const std::vector<std::string> &arg);

// Converts arg into bitset depending on what letter option.
std::bitset<53> get_options(const std::string &s);

// Converts a std::string argument to number T.
template<typename T>
T arg_to_n(const std::string &s);

#endif
