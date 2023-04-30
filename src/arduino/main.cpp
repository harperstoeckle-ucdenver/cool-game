// vim: ft=arduino

#include "game.hpp"

#include "pins.hpp"

#include <etl/queue.h>

#include <Arduino.h>

Game game;

/*
 * Queue of all input events. If the queue is full, all input events are ignored until at least one
 * event in the queue has been processed.
 */
etl::queue<InputEvent, 10> input_events;

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

	attachInterrupt(digitalPinToInterrupt(pin::level_select_button),
		[] { if (!input_events.full()) { input_events.push(InputEvent::change_mode); } },
		FALLING);
	attachInterrupt(digitalPinToInterrupt(pin::reset_button),
		[] { if (!input_events.full()) { input_events.push(InputEvent::reset); } },
		FALLING);
}

void loop()
{
	// Value in either direction before it's actually considered an input.
	constexpr int tolerance = 200;

	static bool left_held = false;
	static bool right_held = false;
	static bool up_held = false;
	static bool select_held = false;

	int const x = analogRead(pin::joystick_x);
	int const y = analogRead(pin::joystick_y);

	// Check for new button presses.
	bool const left_pressed = update_button_state(right_held, x < 512 - tolerance);
	bool const right_pressed = update_button_state(left_held, x > 512 + tolerance);
	bool const up_pressed = update_button_state(up_held, y < 512 - tolerance);
	bool const select_pressed =
		update_button_state(select_held, digitalRead(pin::joystick_button) == LOW);

	if (left_pressed)
	{
		input_events.push(InputEvent::move_left);
	}
	else if (right_pressed)
	{
		input_events.push(InputEvent::move_right);
	}
	else if (up_pressed)
	{
		input_events.push(InputEvent::check_solution);
	}
	else if (select_pressed)
	{
		input_events.push(InputEvent::select);
	}

	// Send pending input events.
	if (!input_events.empty())
	{
		game.send_input_event(input_events.front());
		input_events.pop();
	}

	// Prevents weird problems with repeated inputs.
	delay(50);
}
