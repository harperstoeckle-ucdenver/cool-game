// vim: ft=arduino

#include "game.hpp"

#include "pins.hpp"

#include <Arduino.h>

Game game;

// Update the state of a button and return true if it was just pressed.
auto update_button_state(bool& state, bool new_state) -> bool
{
	bool const just_pressed = !state & new_state;
	state = new_state;
	return just_pressed;
}

void setup()
{
	Serial.begin(9600);

	pinMode(pin::joystick_x, INPUT);
	pinMode(pin::joystick_y, INPUT);
	pinMode(pin::joystick_button, INPUT_PULLUP);

	pinMode(pin::buzzer, OUTPUT);

	pinMode(pin::correct_led, OUTPUT);
	pinMode(pin::incorrect_led, OUTPUT);

	pinMode(pin::level_select_button, INPUT_PULLUP);
	pinMode(pin::reset_button, INPUT_PULLUP);
}

void loop()
{
	// Value in either direction before it's actually considered an input.
	constexpr int tolerance = 200;

	static bool left_held = false;
	static bool right_held = false;
	static bool up_held = false;
	static bool select_held = false;
	static bool level_select_held = false;
	static bool reset_held = false;

	int const x = analogRead(pin::joystick_x);
	int const y = analogRead(pin::joystick_y);

	// Check for new button presses.
	bool const left_pressed = update_button_state(right_held, x < 512 - tolerance);
	bool const right_pressed = update_button_state(left_held, x > 512 + tolerance);
	bool const up_pressed = update_button_state(up_held, y < 512 - tolerance);
	bool const select_pressed =
		update_button_state(select_held, digitalRead(pin::joystick_button) == LOW);
	bool const level_select_pressed =
		update_button_state(level_select_held, digitalRead(pin::level_select_button) == LOW);
	bool const reset_pressed =
		update_button_state(reset_held, digitalRead(pin::reset_button) == LOW);

	if (right_pressed)
	{
		game.send_input_event(InputEvent::move_right);
	}
	else if (left_pressed)
	{
		game.send_input_event(InputEvent::move_left);
	}
	else if (up_pressed)
	{
		game.send_input_event(InputEvent::check_solution);
	}
	else if (select_pressed)
	{
		game.send_input_event(InputEvent::select);
	}
	else if (level_select_pressed)
	{
		game.send_input_event(InputEvent::change_mode);
	}
	else if (reset_pressed)
	{
		game.send_input_event(InputEvent::reset);
	}

	// Prevents weird problems with repeated inputs.
	delay(50);
}
