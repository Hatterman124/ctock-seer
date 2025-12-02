#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

constexpr std::string whitespace {" \f\n\r\t\v"};

bool leap_year(int year)
{
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		return true;

	return false;
}

int main()
{
	double d0 {183.9032135009765625};
	double d1 {182.5262298583984375};

	std::cout << std::setprecision(20)
	          << std::log(d1 / d0) << '\n'
	          << std::log(d1) - std::log(d0) << '\n';

	return 0;
}