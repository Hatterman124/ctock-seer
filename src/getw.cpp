#include "getw.h"
#include <cstddef>
#include <stdfloat>
#include <stdexcept>

template<typename T>
int getw(T n)
{
	size_t n_sizet {static_cast<std::size_t>(n)};
	int width {};

	do {
		++width;
		n_sizet /= 10uz;
		if (width <= 0)
			throw std::overflow_error("std::overflow_error\nIn getw(), width overflowed int");
	} while (n_sizet != 0uz);

	return width;
}

template int getw<std::size_t>(std::size_t);
template int getw<std::float16_t>(std::float16_t);