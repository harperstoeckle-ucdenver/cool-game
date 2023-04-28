// vim: ft=arduino

#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 7, 8, 9, 10);

void setup()
{
	lcd.begin(16, 2);
	lcd.print("hello world");
	lcd.cursor();
}

void loop()
{
	static int c = 0;
	lcd.setCursor(c % 16, c / 16);
	delay(500);
	c = (c + 1) % 32;
}
