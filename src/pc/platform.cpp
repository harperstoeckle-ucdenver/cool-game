#include "platform.hpp"

#include "levels.hpp"

#include <etl/algorithm.h>
#include <etl/flat_map.h>
#include <etl/string.h>
#include <ncurses.h>
#include <unistd.h>

using Utf8Char = etl::string<5>;

// Return a string with the utf-8 representation of the symbol.
static
auto symbol_to_utf8(Symbol s) -> Utf8Char
{
	// Map a type to a symbol.
	static etl::flat_map<SymbolType, char const*, 16> type_map = {
		{SymbolType::num_0, "0"},
		{SymbolType::num_1, "1"},
		{SymbolType::num_2, "2"},
		{SymbolType::num_3, "3"},
		{SymbolType::trophy, "🏆"},
		{SymbolType::flip_left_right, "⦶"},
		{SymbolType::flip_up_down, "⦵"},
		{SymbolType::remove, "×"}
	};

	if (!s.is_block)
	{
		auto const it = type_map.find(s.s.type);
		return Utf8Char(it == type_map.end() ? "?" : it->second);
	}

	Utf8Char c;

	// Insert the corresponding braille character.
	c.push_back(static_cast<char>(0b11100010));
	c.push_back(static_cast<char>(0b10100000));
	c.push_back(static_cast<char>(
		0b10000000
		| (((s.b.bits >> 5) & 1) << 0)
		| (((s.b.bits >> 3) & 1) << 1)
		| (((s.b.bits >> 1) & 1) << 2)
		| (((s.b.bits >> 4) & 1) << 3)
		| (((s.b.bits >> 2) & 1) << 4)
		| (((s.b.bits >> 0) & 1) << 5)));

	return c;
}

void draw_puzzle_state(Puzzle const& p, CursorState c, int level_num)
{
	clear();

	int i = 0;
	for (auto s : p)
	{
		if (!s.is_block)
		{
			char const* arrow_str;
			switch (2 * s.s.left + s.s.right)
			{
			case 0b00: arrow_str = ""; break;
			case 0b01: arrow_str = "→"; break;
			case 0b10: arrow_str = "←"; break;
			case 0b11: arrow_str = "↔"; break;
			}
			mvprintw(0, i * 2, arrow_str);
		}

		if (s.b.is_locked)
		{
			mvaddstr(2, i * 2, "▣");
		}
		else if (i == c.index)
		{
			mvaddstr(2, i * 2, c.grabbed ? "▲" : "△");
		}

		mvaddstr(1, i * 2, symbol_to_utf8(s).c_str());

		++i;
	}

	mvprintw(0, 18, "level %d", level_num);
	mvaddstr(5, 0,
		"quit: q            select/drop: space\n"
		"move: arrow keys   reset level: r\n"
		"level select: tab  try solution: enter\n");
	refresh();
}

void draw_level_select(int cur_level, int max_unlocked_level)
{
	clear();

	int const min_shown = etl::max(0, cur_level - 1);
	int const one_past_max_shown = etl::min(min_shown + 4, static_cast<int>(num_levels));

	int col = 0;
	for (int i = min_shown; i < one_past_max_shown; ++i, col += 4)
	{
		mvprintw(1, col, "%-4d", i);
		if (i == cur_level) { mvaddstr(2, col, "△"); }
		else if (i > max_unlocked_level) { mvaddstr(2, col, "▣"); }
	}

	mvaddstr(5, 0,
		"quit: q            select level: space\n"
		"move: arrow keys\n");

	refresh();
}

void play_jingle(Jingle j)
{
	switch (j)
	{
	case Jingle::move_cursor: break;
	case Jingle::select: break;

	case Jingle::correct:
		mvaddstr(2, 18, "Correct");
		refresh();
		usleep(500000);
		break;

	case Jingle::incorrect:
		mvaddstr(2, 18, "Incorrect");
		refresh();
		usleep(500000);
		break;
	}
}
