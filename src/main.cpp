#include "struc.h"
#include "menu.h"
#include <iostream>

int main() {
	database db;
	flag     f;

	f.py_mode   = false;
	f.in_limit  = in_limit_default;
	f.precision = precision_default;
	std::cout.precision(f.precision);
	while (true) {
		if (menu(db, f))
			break;
	}

	return 0;
}