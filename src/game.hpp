#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "levels.hpp"
#include "platform.hpp"
#include "puzzle.hpp"

// Types of events that can cause the state of the game to change.
enum struct InputEvent
{
	move_left,
	move_right,
	select,
	reset,
	change_mode,
	check_solution
};

/*
 * General manager of the state and output of the game. The caller must continually relay events
 * (inputs, usually) to the Game instance, and the state will be appropriately changed and the
 * correct output will be made. Implementation details of output are handled with platform-specific
 * code.
 */
class Game
{
private:
	CursorState curs_state_ = {0, false};

	bool in_level_select_mode_ = false;
	int last_unlocked_level_ = 0;
	int cur_level_ = last_unlocked_level_;

	Puzzle puzzle_;

public:
	// Starts displaying the game.
	Game();

	/*
	 * Send an event to the game, causing change in the game's state and possibly some sort of
	 * output. For example, sending InputEvent::reset while in a level will reset the level.
	 */
	void send_input_event(InputEvent e);

private:
	// Unconditionally load the level with the given number. The cursor is put in the right spot.
	void load_level(int level_num);
};

#endif
