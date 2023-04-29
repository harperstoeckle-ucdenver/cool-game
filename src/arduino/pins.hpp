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

inline constexpr uint8_t lcd_rs = 2;
inline constexpr uint8_t lcd_rw = 3;
inline constexpr uint8_t lcd_e = 4;
inline constexpr uint8_t lcd_d4 = 7;
inline constexpr uint8_t lcd_d5 = 8;
inline constexpr uint8_t lcd_d6 = 9;
inline constexpr uint8_t lcd_d7 = 10;
} // namespace pin

#endif
