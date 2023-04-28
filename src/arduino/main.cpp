// vim: ft=arduino

#include "game.hpp"

#include <Arduino.h>

Game game;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	if (Serial.available() > 0)
	{
		InputEvent e;
		switch (Serial.read())
		{
		case 'h': e = InputEvent::move_left; break;
		case 'l': e = InputEvent::move_right; break;
		case ' ': e = InputEvent::select; break;
		case 'r': e = InputEvent::reset; break;
		case '\t': e = InputEvent::change_mode; break;
		case 'c': e = InputEvent::check_solution; break;
		default: return;
		}

		game.send_input_event(e);
	}
}
