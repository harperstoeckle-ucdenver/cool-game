#include "platform.hpp"

#include <etl/flat_map.h>
#include <etl/string.h>
#include <ncurses.h>

using Utf8Char = etl::string<5>;

// Return a string with the utf-8 representation of the symbol.
static
auto symbol_to_utf8(Symbol s) -> Utf8Char
{
	// Map a type to a symbol.
	static etl::flat_map<SymbolType, char, 16> type_map = {
		{SymbolType::num_0, '0'},
		{SymbolType::num_1, '1'},
		{SymbolType::num_2, '2'},
		{SymbolType::num_3, '3'},
		{SymbolType::flip_left_right, '|'},
		{SymbolType::flip_up_down, '-'},
		{SymbolType::remove, 'x'}
	};

	if (!s.is_block)
	{
		auto const it = type_map.find(s.s.type);
		return Utf8Char(1, it == type_map.end() ? '?' : it->second);
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
		else if (static_cast<size_t>(i) == c.index)
		{
			mvaddstr(2, i * 2, c.grabbed ? "▲" : "△");
		}

		mvaddstr(1, i * 2, symbol_to_utf8(s).c_str());

		++i;
	}

	mvprintw(0, 18, "level %d", level_num);
	mvprintw(5, 0,
		"quit: q            select/drop: space\n"
		"move: arrow keys   reset level: r\n"
		"change mode: m     try solution: enter\n");
	refresh();
}
