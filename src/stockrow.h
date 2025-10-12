#pragma once

constexpr int number_of_df {5};

/*
 * Structure of Database
 * Array of Vector of Struct
 *
 * Array:  db
 * Vector: df
 * struct: stockrow
 */

struct stockrow {
	int    year;
	int    month;
	int    day;
	double open;
	double high;
	double low;
	double close;
	int    volume;
	double dividends;
	double stocksplits;
};

struct stockrow_b {
	int    year;
	int    month;
	int    day;
	bool open;
	bool high;
	bool low;
	bool close;
	bool volume;
	bool dividends;
	bool stocksplits;
};

struct stockrow_f {
	int    year;
	int    month;
	int    day;
	double open;
	double high;
	double low;
	double close;
	double volume;
	double dividends;
	double stocksplits;
};