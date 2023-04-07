#include "symbol.hpp"

auto block_bit(Symbol s, int row, int col) -> bool
{
	return (s.b.bits >> (5 - 2 * row - col)) & 1;
}
