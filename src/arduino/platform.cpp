// vim: ft=arduino
#include "platform.hpp"

#include "levels.hpp"
#include "notes.hpp"
#include "pins.hpp"

#include <LiquidCrystal.h>
#include <etl/algorithm.h>
#include <etl/array.h>
#include <etl/functional.h>
#include <etl/string.h>
#include <etl/variant.h>

#include <Arduino.h>

// Needed because arduino for some reason defines these as macros.
#undef max
#undef min

#include <stdio.h>

LiquidCrystal lcd(pin::lcd_rs, pin::lcd_rw, pin::lcd_e, pin::lcd_d4, pin::lcd_d5, pin::lcd_d6,
	pin::lcd_d7);
int const start_lcd = [] {
	lcd.begin(16, 2);
	return 0;
}();

using Sprite = etl::array<uint8_t, 8>;

// Either a unique sprite or a normal character.
using DisplayChar = etl::variant<char, Sprite>;

// Add pixels from b to a.
void add_to_sprite(Sprite& a, Sprite const& b)
{
	etl::transform_n(a.cbegin(), b.cbegin(), 8, a.begin(), etl::bit_or<uint8_t>());
}

Sprite const block_sprites[3][2] = {
	// Row 0.
	{
		{0b11000, 0b11000, 0, 0, 0, 0, 0, 0},
		{0b00011, 0b00011, 0, 0, 0, 0, 0, 0}
	},

	// Row 1.
	{
		{0, 0, 0, 0b11000, 0b11000, 0, 0, 0},
		{0, 0, 0, 0b00011, 0b00011, 0, 0, 0}
	},

	// Row 2.
	{
		{0, 0, 0, 0, 0, 0, 0b11000, 0b11000},
		{0, 0, 0, 0, 0, 0, 0b00011, 0b00011}
	}
};

Sprite const left_sprite = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b01000,
	0b11000,
	0b01000
};
Sprite const right_sprite = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00100,
	0b00110,
	0b00100
};

Sprite const trophy_sprite = {
	0b11111,
	0b10101,
	0b11111,
	0b01110,
	0b00100,
	0b00100,
	0b01110,
	0b01110
};

Sprite const remove_sprite = {
	0b10001,
	0b01010,
	0b00100,
	0b01010,
	0b10001,
	0b00000,
	0b00000,
	0b00000
};

Sprite const flip_left_right_sprite = {
	0b11001,
	0b10001,
	0b10001,
	0b10011,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

Sprite const flip_up_down_sprite = {
	0b11111,
	0b10000,
	0b00001,
	0b01111,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

static
auto symbol_to_char(Symbol s) -> DisplayChar
{
	if (s.is_block)
	{
		Sprite block = {0, 0, 0, 0, 0, 0, 0, 0};
		for (int const r : {0, 1, 2})
		{
			for (int const c : {0, 1})
			{
				if (block_bit(s, r, c))
				{
					add_to_sprite(block, block_sprites[r][c]);
				}
			}
		}

		return block;
	}

	// Used for symbols with directions.
	Sprite symbol;
	switch (s.s.type)
	{
		case SymbolType::num_0: return '0';
		case SymbolType::num_1: return '1';
		case SymbolType::num_2: return '2';
		case SymbolType::num_3: return '3';
		case SymbolType::trophy: return trophy_sprite;

		case SymbolType::remove: symbol = remove_sprite; break;
		case SymbolType::flip_up_down: symbol = flip_up_down_sprite; break;
		case SymbolType::flip_left_right: symbol = flip_left_right_sprite; break;
	}

	if (s.s.left) { add_to_sprite(symbol, left_sprite); }
	if (s.s.right) { add_to_sprite(symbol, right_sprite); }

	return symbol;
}

void draw_puzzle_state(Puzzle const& p, CursorState c, int level_num)
{
	// The actual sprites have to be created *before* clearing to work.
	for (uint8_t i = 0; i < p.size(); ++i)
	{
		auto const dc = symbol_to_char(p[i]);
		if (Sprite* s = etl::get_if<Sprite>(&dc))
		{
			lcd.createChar(i, s->data());
		}
	}

	lcd.clear();

	for (uint8_t i = 0; i < p.size(); ++i)
	{
		lcd.setCursor(i * 2, 0);

		auto const dc = symbol_to_char(p[i]);
		if (char* c = etl::get_if<char>(&dc))
		{
			lcd.print(*c);
		}
		else
		{
			lcd.write(i);
		}

		lcd.setCursor(i * 2, 1);

		if (p[i].b.is_locked)
		{
			lcd.print('#');
		}
		else if (i == c.index)
		{
			lcd.print(c.grabbed ? '@' : '^');
		}
	}

	lcd.setCursor(c.index * 2, 1);

	//lcd.setCursor(9, 0);
	//lcd.print("lvl ");
	//lcd.print(level_num);
}

void draw_level_select(int cur_level, int max_unlocked_level)
{
	lcd.clear();

	int const min_shown = etl::max(0, cur_level - 1);
	int const one_past_max_shown = etl::min(min_shown + 4, static_cast<int>(num_levels));

	int col = 0;
	for (int i = min_shown; i < one_past_max_shown; ++i, col += 4)
	{
		// Buffer for the formatted string.
		etl::string<5> buf(4, '\0');
		snprintf(&buf[0], 4, "%-4d", i);
		
		lcd.setCursor(col, 0);
		lcd.print(buf.c_str());

		lcd.setCursor(col, 1);
		if (i == cur_level)
		{
			lcd.print('^');
		}
		else if (i > max_unlocked_level)
		{
			lcd.print('#');
		}
	}
}

void play_jingle(Jingle j)
{
	switch (j)
	{
	case Jingle::move_cursor:
		tone(pin::buzzer, note::c5, 30);
		break;

	case Jingle::select:
		tone(pin::buzzer, note::g3);
		delay(50);
		tone(pin::buzzer, note::e4);
		delay(50);
		noTone(pin::buzzer);
		break;

	case Jingle::drop:
		tone(pin::buzzer, note::e4);
		delay(50);
		tone(pin::buzzer, note::g3);
		delay(50);
		noTone(pin::buzzer);
		break;

	case Jingle::correct:
		digitalWrite(pin::correct_led, HIGH);
		tone(pin::buzzer, note::d4);
		delay(100);
		tone(pin::buzzer, note::a3);
		delay(100);
		tone(pin::buzzer, note::f4);
		delay(200);
		noTone(pin::buzzer);
		digitalWrite(pin::correct_led, LOW);
		break;

	case Jingle::incorrect:
		digitalWrite(pin::incorrect_led, HIGH);
		tone(pin::buzzer, note::f2);
		delay(200);
		noTone(pin::buzzer);
		delay(100);
		tone(pin::buzzer, note::f2);
		delay(200);
		noTone(pin::buzzer);
		digitalWrite(pin::incorrect_led, LOW);
		break;
	}
}
