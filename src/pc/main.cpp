#include "symbol.hpp"

#include <stddef.h>
#include <stdio.h>

#include <etl/flat_map.h>
#include <etl/string.h>

using Utf8Char = etl::string<5>;

// Return a string with the utf-8 representation of the symbol.
auto symbol_to_utf8(Symbol s) -> Utf8Char
{
	// Map a type to a symbol.
	static etl::flat_map<SymbolType, char, 16> type_map = {
		{SymbolType::flip_left_right, '|'},
		{SymbolType::flip_up_down, '-'}
	};

	if (!s.is_block)
	{
		auto const it = type_map.find(s.s.type);
		return Utf8Char(1, it == type_map.end() ? '?' : it->second);
	}

	Utf8Char c;

	// Insert the corresponding braille character.
	c.push_back(static_cast<char>(0b11100010));
	c.push_back(static_cast<char>(0b10100000));
	c.push_back(static_cast<char>(
		0b10000000
		| (((s.b.bits >> 5) & 1) << 0)
		| (((s.b.bits >> 3) & 1) << 1)
		| (((s.b.bits >> 1) & 1) << 2)
		| (((s.b.bits >> 4) & 1) << 3)
		| (((s.b.bits >> 2) & 1) << 4)
		| (((s.b.bits >> 0) & 1) << 5)));

	return c;
}

auto main() -> int
{
	auto const s = Symbol{.b = {1, 0, 0b110110}};

	for (int i : {0, 1, 2})
	{
		for (int j : {0, 1})
		{
			putchar(block_bit(s, i, j) ? '#' : ' ');
		}
		putchar('\n');
	}
	return 0;
}
