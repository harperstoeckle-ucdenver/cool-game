#include "platform.hpp"

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 7, 8, 9, 10);
int const start_lcd = [] {
	lcd.begin(16, 2);
	return 0;
}();

static
auto symbol_to_char(Symbol s) -> char
{
	if (s.is_block) { return 'b'; }
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
	lcd.clear();
	for (Symbol const s : p)
	{
		lcd.print(symbol_to_char(s));
	}

	lcd.setCursor(c.index, 1);
	lcd.print(c.grabbed ? '@' : '^');

	lcd.setCursor(9, 0);
	lcd.print("lvl ");
	lcd.print(level_num);
}

void draw_level_select([[maybe_unused]] int cur_level, [[maybe_unused]] int max_unlocked_level) {}
void play_jingle([[maybe_unused]] Jingle j) {}
