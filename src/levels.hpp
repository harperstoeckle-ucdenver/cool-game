// Contains the list of levels.
#ifndef LEVELS_HPP_INCLUDED
#define LEVELS_HPP_INCLUDED

#include "puzzle.hpp"

inline Puzzle const levels[] = {
	// 0
	{
		make_special(false, false, SymbolType::num_1),
		make_special(false, false, SymbolType::num_3),
		make_special(false, false, SymbolType::num_2),
		make_special(false, false, SymbolType::num_0)
	},

	// 1
	{
		make_block(0b011101),
		make_block(0b111011)
	},

	// 2
	{
		make_block(0b110101),
		make_block(0b101101),
		make_block(0b101011)
	},

	// 3
	{
		make_block(0b001101),
		make_block(0b010010),
		make_block(0b111010)
	},

	// 4
	{
		make_block(0b111011, true),
		make_block(0b101110),
		make_special(false, true, SymbolType::flip_left_right),
		make_block(0b011100),
		make_special(true, false, SymbolType::remove)
	},

	// Last
	{
		make_special(false, false, SymbolType::trophy)
	}
};

// Just for convenience.
inline constexpr size_t num_levels = sizeof(levels) / sizeof(Puzzle);

#endif
