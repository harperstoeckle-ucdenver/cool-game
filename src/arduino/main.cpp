// vim: ft=arduino

#include "game.hpp"

#include <Arduino.h>

Game game;

// Pins for the joystick.
constexpr uint8_t joystick_x_pin = PIN_A0;
constexpr uint8_t joystick_y_pin = PIN_A1;
constexpr uint8_t joystick_button_pin = PIN_A2;

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

	pinMode(joystick_x_pin, INPUT);
	pinMode(joystick_y_pin, INPUT);
	pinMode(joystick_button_pin, INPUT_PULLUP);

	pinMode(5, OUTPUT);
}

void loop()
{
	// Value in either direction before it's actually considered an input.
	constexpr int tolerance = 200;

	static bool left_held = false;
	static bool right_held = false;
	static bool up_held = false;
	static bool down_held = false;
	static bool select_held = false;

	int const x = analogRead(joystick_x_pin);
	int const y = analogRead(joystick_y_pin);

	// Check for new button presses.
	bool const left_pressed = update_button_state(right_held, x < 512 - tolerance);
	bool const right_pressed = update_button_state(left_held, x > 512 + tolerance);
	bool const up_pressed = update_button_state(up_held, y < 512 - tolerance);
	bool const down_pressed = update_button_state(down_held, y > 512 + tolerance);
	bool const select_pressed =
		update_button_state(select_held, digitalRead(joystick_button_pin) == LOW);

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
	else if (down_pressed)
	{
		game.send_input_event(InputEvent::change_mode);
	}
	else if (select_pressed)
	{
		game.send_input_event(InputEvent::select);
	}

	// Prevents weird problems with repeated inputs.
	delay(50);
}
