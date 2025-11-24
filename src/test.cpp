#include <iostream>
#include <string>

constexpr std::string whitespace {" \f\n\r\t\v"};

bool leap_year(int year)
{
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		return true;

	return false;
}

int main()
{
	std::string s {};

	while (std::cin >> s)
		std::cout << leap_year(std::stoi(s)) << '\n';

	return 0;
}