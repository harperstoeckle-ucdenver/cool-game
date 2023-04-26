// Platform-dependent stuff, mainly for output.
#ifndef PLATFORM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED

#include "puzzle.hpp"

#include <etl/optional>
#include <etl/span.h>

struct CursorState
{
	size_t index;
	bool grabbed;
};

// Draw the puzzle's current state, cursor, and level number.
void draw_puzzle_state(Puzzle const& p, CursorState c, int level_num);

// A note is a sound that is played for a duration given in milliseconds.
struct Note
{
	etl::optional<unsigned> frequency; // Null means rest.
	unsigned long duration_ms;
};

// Play a note, blocking until it is done playing.
void play_note(Note const& n);

#endif
