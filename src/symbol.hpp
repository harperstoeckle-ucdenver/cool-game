#ifndef SYMBOL_HPP_INCLUDED
#define SYMBOL_HPP_INCLUDED

#include <stdint.h>

enum struct SymbolType : uint8_t
{
	// Regular numbers. They do nothing and are just for showing off the mechanics.
	num_0,
	num_1,
	num_2,
	num_3,

	// Shown in the last level. It means you win.
	trophy,

	remove,
	flip_up_down,
	flip_left_right
};

struct SpecialSymbol
{
	uint8_t is_block : 1;
	uint8_t is_locked : 1;
	uint8_t left : 1;
	uint8_t right : 1;
	SymbolType type : 4;
};

struct BlockSymbol
{
	uint8_t is_block : 1;
	uint8_t is_locked : 1;
	uint8_t bits : 6;
};

union Symbol
{
	uint8_t is_block : 1;
	SpecialSymbol s;
	BlockSymbol b;
};
static_assert(sizeof(Symbol) == 1);

// Get the block bit at the given row and column.
constexpr
auto block_bit(Symbol s, int row, int col) -> bool
{
	return (s.b.bits >> (5 - 2 * row - col)) & 1;
}

// Set the block bit at the given row and column.
constexpr
void set_block_bit(Symbol& s, int row, int col, bool v)
{
	int const offset = 5 - 2 * row - col;

	if (v)
	{
		s.b.bits |= (1 << offset);
	}
	else
	{
		s.b.bits &= ~(1 << offset);
	}
}
// Swap bits (r0, c0) and (r1, c1).
constexpr
void swap_block_bits(Symbol& s, int r0, int c0, int r1, int c1)
{
	bool const t = block_bit(s, r0, c0);
	set_block_bit(s, r0, c0, block_bit(s, r1, c1));
	set_block_bit(s, r1, c1, t);
}

// Construct a block symbol.
constexpr
auto make_block(uint8_t bits, bool locked = false) -> Symbol
{
	return Symbol{.b = {1, locked, bits}};
}

// Construct a special symbol.
constexpr
auto make_special(bool left, bool right, SymbolType type, bool locked = false) -> Symbol
{
	return Symbol{.s = {0, locked, left, right, type}};
}

#endif
