#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

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
public:
	/*
	 * Send an event to the game, causing change in the game's state and possibly some sort of
	 * output. For example, sending InputEvent::reset while in a level will reset the level.
	 */
	void send_input_event(InputEvent e);
};

#endif
