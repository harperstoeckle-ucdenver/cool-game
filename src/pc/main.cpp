#include "symbol.hpp"

#include <stddef.h>
#include <stdio.h>

/*
 * Write the utf-8 for a symbol into the range starting at o. Return the number of bytes that were
 * written.
 */
template <typename O>
size_t symbol_to_utf8(Symbol s, O o)
{
	if (!s.is_block)
	{
		*o++ = static_cast<char>(s.value);
		return 1;
	}

	// Insert the corresponding braille character.
	*o++ = static_cast<char>(0b11100010);
	*o++ = static_cast<char>(0b10100000);
	*o++ = static_cast<char>(
		0b10000000
		| (((s.value >> 5) & 1) << 0)
		| (((s.value >> 3) & 1) << 1)
		| (((s.value >> 1) & 1) << 2)
		| (((s.value >> 4) & 1) << 3)
		| (((s.value >> 2) & 1) << 4)
		| (((s.value >> 0) & 1) << 5));
	return 3;
}

auto main() -> int
{
	return 0;
}
