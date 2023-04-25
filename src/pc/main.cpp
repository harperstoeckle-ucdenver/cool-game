#include "game.hpp"
#include "puzzle.hpp"
#include "symbol.hpp"

#include <locale.h>
#include <stddef.h>
#include <stdio.h>

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
		{SymbolType::flip_left_right, '|'},
		{SymbolType::flip_up_down, '-'}
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

static
void draw_puzzle(Puzzle const& p)
{
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
			mvprintw(0, i, arrow_str);
		}

		if (s.b.is_locked)
		{
			mvprintw(2, i, "⬢");
		}

		mvprintw(1, i, symbol_to_utf8(s).c_str());

		i += 2;
	}
}

auto main() -> int
{
	setlocale(LC_ALL, "");

	initscr();
	noecho();
	keypad(stdscr, true);
	cbreak();
	curs_set(0);

	Puzzle p = {
		make_block(0b111011),
		make_block(0b101110),
		make_special(true, false, SymbolType::flip_left_right)
	};

	clear();
	draw_puzzle(p);
	refresh();
	getch();

	p = apply_special_symbols(p);

	clear();
	draw_puzzle(p);
	refresh();
	getch();

	endwin();

	return is_solved(p);
}
