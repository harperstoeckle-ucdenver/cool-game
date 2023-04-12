#ifndef SYMBOL_HPP_INCLUDED
#define SYMBOL_HPP_INCLUDED

#include <stdint.h>

enum struct SymbolType : uint8_t
{
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
auto block_bit(Symbol s, int row, int col) -> bool;

#endif
