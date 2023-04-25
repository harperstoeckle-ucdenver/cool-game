#include "symbol.hpp"

auto block_bit(Symbol s, int row, int col) -> bool
{
	return (s.b.bits >> (5 - 2 * row - col)) & 1;
}

void set_block_bit(Symbol& s, int row, int col, bool v)
{
	int const offset = 5 - 2 * row - col;

	if (v)
	{
		s.b.bits |= (1 << offset);
	}
	else
	{
		s.b.bits ^= (1 << offset);
	}
}

void swap_block_bits(Symbol& s, int r0, int c0, int r1, int c1)
{
	bool const t = block_bit(s, r0, c0);
	set_block_bit(s, r0, c0, block_bit(s, r1, c1));
	set_block_bit(s, r1, c1, t);
}
