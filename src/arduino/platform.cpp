// vim: ft=arduino
#include "platform.hpp"

#include "levels.hpp"
#include "notes.hpp"

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

LiquidCrystal lcd(2, 3, 4, 7, 8, 9, 10);
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
	switch (s.s.type)
	{
		case SymbolType::num_0: return '0';
		case SymbolType::num_1: return '1';
		case SymbolType::num_2: return '2';
		case SymbolType::num_3: return '3';
		case SymbolType::trophy: return '!';
		case SymbolType::remove: return 'x';
		case SymbolType::flip_up_down: return '-';
		case SymbolType::flip_left_right: return '|';
	}
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

constexpr uint8_t buzzer_pin = 5;

void play_jingle(Jingle j)
{
	switch (j)
	{
	case Jingle::move_cursor:
		tone(buzzer_pin, note::c6, 100);
		break;

	case Jingle::select:
		break;

	case Jingle::correct:
		tone(buzzer_pin, note::c4);
		delay(200);
		tone(buzzer_pin, note::ds4);
		delay(200);
		tone(buzzer_pin, note::b3);
		delay(400);
		noTone(buzzer_pin);
		break;

	case Jingle::incorrect:
		tone(buzzer_pin, note::f2);
		delay(200);
		noTone(buzzer_pin);
		delay(100);
		tone(buzzer_pin, note::f2);
		delay(200);
		noTone(buzzer_pin);
		break;
	}
}
