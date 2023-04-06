#include "symbol.hpp"

#include <stddef.h>
#include <stdio.h>

#include <etl/string.h>

using Utf8Char = etl::string<5>;

// Return a string with the utf-8 representation of the symbol.
auto symbol_to_utf8(Symbol s) -> Utf8Char
{
	if (!s.is_block)
	{
		return Utf8Char(1, static_cast<char>(s.value));
	}

	Utf8Char c;

	// Insert the corresponding braille character.
	c.push_back(static_cast<char>(0b11100010));
	c.push_back(static_cast<char>(0b10100000));
	c.push_back(static_cast<char>(
		0b10000000
		| (((s.value >> 5) & 1) << 0)
		| (((s.value >> 3) & 1) << 1)
		| (((s.value >> 1) & 1) << 2)
		| (((s.value >> 4) & 1) << 3)
		| (((s.value >> 2) & 1) << 4)
		| (((s.value >> 0) & 1) << 5)));

	return c;
}

auto main() -> int
{
	puts(symbol_to_utf8({1, 0b100110}).c_str());
	return 0;
}
