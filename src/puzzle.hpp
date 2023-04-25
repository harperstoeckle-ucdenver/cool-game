#ifndef PUZZLE_HPP_INCLUDED
#define PUZZLE_HPP_INCLUDED

#include "symbol.hpp"

#include <etl/vector.h>

// A puzzle is just a sequence of up to 8 symbols.
using Puzzle = etl::vector<Symbol, 8>;

auto apply_special_symbols(Puzzle const& p) -> Puzzle;

// Check whether the puzzle is in a solved position.
auto is_solved(Puzzle const& p) -> bool;

#endif
