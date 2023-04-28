// Platform-dependent stuff, mainly for output.
#ifndef PLATFORM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED

#include "puzzle.hpp"

#include <etl/optional.h>
#include <etl/span.h>

struct CursorState
{
	int index;
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

// Play a sequence of notes, blocking until finished.
void play_melody(etl::span<Note const> melody);

/*
 * Draw a level select screen from the current level number and maximum unlocked level. All
 * non-unlocked levels will be drawn with a locked sign below them.
 */
void draw_level_select(int cur_level, int max_unlocked_level);

#endif
