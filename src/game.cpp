#include "game.hpp"

Game::Game()
{
	draw_puzzle_state(puzzle, curs_state, 0);
}

void Game::send_input_event(InputEvent e)
{
	switch (e)
	{
	case InputEvent::move_left:
		if (curs_state.index == 0) { curs_state.index = puzzle.size() - 1; }
		else { --curs_state.index; }
		break;

	case InputEvent::move_right:
		if (curs_state.index == puzzle.size() - 1) { curs_state.index = 0; }
		else { ++curs_state.index; }
		break;

	case InputEvent::select:
		curs_state.grabbed = !curs_state.grabbed;
		break;

	case InputEvent::reset:
		break;

	case InputEvent::change_mode:
		break;

	case InputEvent::check_solution:
		break;
	}

	draw_puzzle_state(puzzle, curs_state, 0);
}
