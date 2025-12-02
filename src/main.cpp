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

	// Banner message
	std::cout << "        __             __      _____\n  _____/ /_____  _____/ /__   / ___/___  ___  _____\n / ___/ __/ __ \\/ ___/ //_/   \\__ \\/ _ \\/ _ \\/ ___/\n/ /__/ /_/ /_/ / /__/ ,<     ___/ /  __/  __/ /\n\\___/\\__/\\____/\\___/_/|_|   /____/\\___/\\___/_/\nctock Seer version 0.2.0\nPaul Lee Kim\n\nCalling Py_FinalizeEx multiple times can cause problems.\nNumPy will throw an exception.\nPlease try to enter Python-interpreter mode only once.\nhttps://docs.python.org/3/c-api/init.html#c.Py_FinalizeEx\n";

	while (true) {
		if (menu(db, f))
			break;
	}

	return 0;
}