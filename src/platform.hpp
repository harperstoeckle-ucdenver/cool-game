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

/*
 * Draw a level select screen from the current level number and maximum unlocked level. All
 * non-unlocked levels will be drawn with a locked sign below them.
 */
void draw_level_select(int cur_level, int max_unlocked_level);

/*
 * A "jingle" is something that plays when an event happens and indicates something to the player.
 * For example, completing a level might turn on a light and play a sound. In the terminal, these
 * probably mostly do nothing.
 */
enum struct Jingle
{
	move_cursor,
	select,
	correct,
	incorrect
};

// This plays whatever is required for the jingle. It may or may not block.
void play_jingle(Jingle j);

#endif
