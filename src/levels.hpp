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

	// Last
	{
		make_special(false, false, SymbolType::trophy)
	}
};

// Just for convenience.
inline constexpr size_t num_levels = sizeof(levels) / sizeof(Puzzle);

#endif
