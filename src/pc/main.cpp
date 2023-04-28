#include "game.hpp"
#include "puzzle.hpp"
#include "symbol.hpp"

#include <locale.h>
#include <stddef.h>
#include <stdio.h>

#include <ncurses.h>

auto main() -> int
{
	setlocale(LC_ALL, "");

	initscr();
	noecho();
	keypad(stdscr, true);
	cbreak();
	curs_set(0);

	Game game;

	int c;
	while ((c = getch()) != 'q')
	{
		InputEvent e;
		switch (c)
		{
		case KEY_LEFT: e = InputEvent::move_left; break;
		case KEY_RIGHT: e = InputEvent::move_right; break;
		case ' ': e = InputEvent::select; break;
		case 'r': e = InputEvent::reset; break;
		case '\t': e = InputEvent::change_mode; break;
		case '\n': e = InputEvent::check_solution; break;
		default: continue;
		}

		game.send_input_event(e);
	}

	endwin();

	return 0;
}
