#include "game.hpp"

#include <etl/algorithm.h>

Game::Game()
{
	draw_puzzle_state(puzzle, curs_state, 0);
}

// Find the next unlocked symbol in a puzzle after the cursor. If not found, return p.end().
static
auto find_next_unlocked(Puzzle& p, int cursor_index) -> Puzzle::iterator
{
	if (cursor_index >= static_cast<int>(p.size()) || cursor_index < 0) { return p.end(); }
	return etl::find_if(p.begin() + cursor_index + 1, p.end(),
		[](auto s) { return !s.b.is_locked; });
}

// Find the next unlocked symbol in a puzzle before the cursor. If not found, return p.end().
static
auto find_prev_unlocked(Puzzle& p, int cursor_index) -> Puzzle::iterator
{
	if (cursor_index >= static_cast<int>(p.size()) || cursor_index < 0) { return p.end(); }
	auto const prev_unlocked_it = etl::find_if(p.rend() - cursor_index, p.rend(),
		[](auto s) { return !s.b.is_locked; });
	return prev_unlocked_it == p.rend() ? p.end() : prev_unlocked_it.base() - 1;
}

void Game::send_input_event(InputEvent e)
{
	auto const cur_it = puzzle.begin() + curs_state.index;

	switch (e)
	{
	case InputEvent::move_left:
		{
			auto const prev_unlocked_it = find_prev_unlocked(puzzle, curs_state.index);
			if (prev_unlocked_it != puzzle.end())
			{
				curs_state.index = static_cast<int>(prev_unlocked_it - puzzle.begin());
				if (curs_state.grabbed) { etl::iter_swap(cur_it, prev_unlocked_it); }
			}
		}
		break;

	case InputEvent::move_right:
		{
			auto const next_unlocked_it = find_next_unlocked(puzzle, curs_state.index);
			if (next_unlocked_it != puzzle.end())
			{
				curs_state.index = static_cast<int>(next_unlocked_it - puzzle.begin());
				if (curs_state.grabbed) { etl::iter_swap(cur_it, next_unlocked_it); }
			}
		}
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
