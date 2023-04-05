#ifndef SYMBOL_HPP_INCLUDED
#define SYMBOL_HPP_INCLUDED

#include <stdint.h>

struct Symbol
{
	uint8_t is_block : 1;
	uint8_t value : 7;
};
static_assert(sizeof(Symbol) == 1);

#endif
