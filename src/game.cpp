#include "game.hpp"

#include <etl/algorithm.h>

Game::Game()
{
	load_level(cur_level_);
	draw_puzzle_state(puzzle_, curs_state_, cur_level_);
}

// Find the next unlocked symbol in a puzzle_ after the cursor. If not found, return p.end().
static
auto find_next_unlocked(Puzzle& p, int cursor_index) -> Puzzle::iterator
{
	if (cursor_index >= static_cast<int>(p.size()) || cursor_index < 0) { return p.end(); }
	return etl::find_if(p.begin() + cursor_index + 1, p.end(),
		[](auto s) { return !s.b.is_locked; });
}

// Find the next unlocked symbol in a puzzle_ before the cursor. If not found, return p.end().
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
	auto const cur_it = puzzle_.begin() + curs_state_.index;

	switch (e)
	{
	case InputEvent::move_left:
		if (in_level_select_mode_)
		{
			if (cur_level_ > 0) { play_jingle(Jingle::move_cursor); }
			cur_level_ = etl::max(0, cur_level_ - 1);
		}
		else
		{
			auto const prev_unlocked_it = find_prev_unlocked(puzzle_, curs_state_.index);
			if (prev_unlocked_it != puzzle_.end())
			{
				play_jingle(Jingle::move_cursor);
				curs_state_.index = static_cast<int>(prev_unlocked_it - puzzle_.begin());
				if (curs_state_.grabbed) { etl::iter_swap(cur_it, prev_unlocked_it); }
			}
		}
		break;

	case InputEvent::move_right:
		if (in_level_select_mode_)
		{
			if (cur_level_ < last_unlocked_level_) { play_jingle(Jingle::move_cursor); }
			cur_level_ = etl::min(cur_level_ + 1, last_unlocked_level_);
		}
		else
		{
			auto const next_unlocked_it = find_next_unlocked(puzzle_, curs_state_.index);
			if (next_unlocked_it != puzzle_.end())
			{
				play_jingle(Jingle::move_cursor);
				curs_state_.index = static_cast<int>(next_unlocked_it - puzzle_.begin());
				if (curs_state_.grabbed) { etl::iter_swap(cur_it, next_unlocked_it); }
			}
		}
		break;

	case InputEvent::select:

		if (in_level_select_mode_)
		{
			load_level(cur_level_);
			in_level_select_mode_ = false;
		}
		else
		{
			if (curs_state_.grabbed)
			{
				play_jingle(Jingle::drop);
				curs_state_.grabbed = false;
			}
			else
			{
				play_jingle(Jingle::select);
				curs_state_.grabbed = true;
			}
		}
		break;

	// This was already handled before, so we ignore it.
	case InputEvent::reset:
		if (!in_level_select_mode_)
		{
			puzzle_ = levels[cur_level_];
		}
		break;

	case InputEvent::change_mode:
		in_level_select_mode_ = true;
		break;

	case InputEvent::check_solution:
		// Cheat to make things easier.
		if (in_level_select_mode_)
		{
			play_jingle(Jingle::correct);
			last_unlocked_level_ = static_cast<int>(num_levels) - 1;
		}
		else
		{
			if (is_solved(puzzle_))
			{
				play_jingle(Jingle::correct);

				if (cur_level_ < static_cast<int>(num_levels) - 1)
				{
					++cur_level_;
					last_unlocked_level_ = etl::max(cur_level_, last_unlocked_level_);
					load_level(cur_level_);
				}
			}
			else
			{
				play_jingle(Jingle::incorrect);
			}
		}
		break;
	}

	if (in_level_select_mode_)
	{
		draw_level_select(cur_level_, last_unlocked_level_);
	}
	else
	{
		draw_puzzle_state(puzzle_, curs_state_, cur_level_);
	}
}

void Game::load_level(int level_num)
{
	puzzle_ = levels[level_num];
	curs_state_ = {
		static_cast<int>(etl::find_if(puzzle_.cbegin(), puzzle_.cend(),
				[](auto s) { return !s.b.is_locked; }) - puzzle_.begin()),
		false
	};
}
