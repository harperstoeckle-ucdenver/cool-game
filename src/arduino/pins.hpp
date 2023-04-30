// Contains pin number constants.
#ifndef PINS_HPP_INCLUDED
#define PINS_HPP_INCLUDED

#include <pins_arduino.h>

namespace pin
{
inline constexpr uint8_t buzzer = 5;
inline constexpr uint8_t joystick_x = PIN_A0;
inline constexpr uint8_t joystick_y = PIN_A1;
inline constexpr uint8_t joystick_button = PIN_A2;

inline constexpr uint8_t lcd_rs = 6;
inline constexpr uint8_t lcd_rw = 7;
inline constexpr uint8_t lcd_e = 8;
inline constexpr uint8_t lcd_d4 = 9;
inline constexpr uint8_t lcd_d5 = 10;
inline constexpr uint8_t lcd_d6 = 11;
inline constexpr uint8_t lcd_d7 = 12;

inline constexpr uint8_t correct_led = PIN_A3;
inline constexpr uint8_t incorrect_led = PIN_A4;

inline constexpr uint8_t level_select_button = 2;
inline constexpr uint8_t reset_button = 3;
} // namespace pin

#endif
