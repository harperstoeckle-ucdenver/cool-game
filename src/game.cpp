#include "game.hpp"

#include <etl/algorithm.h>

Game::Game()
{
	puzzle = levels[cur_level];
	draw_puzzle_state(puzzle, curs_state, cur_level);
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
		if (in_level_select_mode)
		{
			if (cur_level > 0) { play_jingle(Jingle::move_cursor); }
			cur_level = etl::max(0, cur_level - 1);
		}
		else
		{
			auto const prev_unlocked_it = find_prev_unlocked(puzzle, curs_state.index);
			if (prev_unlocked_it != puzzle.end())
			{
				play_jingle(Jingle::move_cursor);
				curs_state.index = static_cast<int>(prev_unlocked_it - puzzle.begin());
				if (curs_state.grabbed) { etl::iter_swap(cur_it, prev_unlocked_it); }
			}
		}
		break;

	case InputEvent::move_right:
		if (in_level_select_mode)
		{
			if (cur_level < last_unlocked_level) { play_jingle(Jingle::move_cursor); }
			cur_level = etl::min(cur_level + 1, last_unlocked_level);
		}
		else
		{
			auto const next_unlocked_it = find_next_unlocked(puzzle, curs_state.index);
			if (next_unlocked_it != puzzle.end())
			{
				play_jingle(Jingle::move_cursor);
				curs_state.index = static_cast<int>(next_unlocked_it - puzzle.begin());
				if (curs_state.grabbed) { etl::iter_swap(cur_it, next_unlocked_it); }
			}
		}
		break;

	case InputEvent::select:
		play_jingle(Jingle::select);

		if (in_level_select_mode)
		{
			puzzle = levels[cur_level];
			curs_state = {0, false};
			in_level_select_mode = false;
		}
		else
		{
			curs_state.grabbed = !curs_state.grabbed;
		}
		break;

	// This was already handled before, so we ignore it.
	case InputEvent::reset:
		if (!in_level_select_mode)
		{
			puzzle = levels[cur_level];
		}
		break;

	case InputEvent::change_mode:
		in_level_select_mode = true;
		break;

	case InputEvent::check_solution:
		if (!in_level_select_mode)
		{
			if (is_solved(puzzle))
			{
				play_jingle(Jingle::correct);

				if (cur_level < static_cast<int>(num_levels) - 1)
				{
					++cur_level;
					last_unlocked_level = etl::max(cur_level, last_unlocked_level);
					puzzle = levels[cur_level];
					curs_state = {0, false};
				}
			}
			else
			{
				play_jingle(Jingle::incorrect);
			}
		}
		break;
	}

	if (in_level_select_mode)
	{
		draw_level_select(cur_level, last_unlocked_level);
	}
	else
	{
		draw_puzzle_state(puzzle, curs_state, cur_level);
	}
}
