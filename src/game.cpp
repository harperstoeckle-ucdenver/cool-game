#include "game.hpp"

Game::Game()
{
	draw_puzzle_state(puzzle, curs_state, 0);
}

void Game::send_input_event(InputEvent e)
{
	switch (e)
	{
	case InputEvent::move_left: break;
	case InputEvent::move_right: break;
	case InputEvent::select: break;
	case InputEvent::reset: break;
	case InputEvent::change_mode: break;
	case InputEvent::check_solution: break;
	}

	draw_puzzle_state(puzzle, curs_state, 0);
}
