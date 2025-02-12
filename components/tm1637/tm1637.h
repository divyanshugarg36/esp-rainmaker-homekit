/**
 * ESP-32 IDF library for control TM1637 LCD 7-Segment display
 *
 * Author: Petro <petro@petro.ws>
 *
 * Project homepage: https://github.com/petrows/esp-32-tm1637
 * Example: https://github.com/petrows/esp-32-tm1637-example
 *
 * License: MIT (see LICENSE file included)
 *
 */

#ifndef TM1637_H
#define TM1637_H

#include <inttypes.h>
#include <driver/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

static const int8_t tm1637_symbols[] = {
			    // XGFEDCBA
		0x3f, // 0b00111111,    // 0
		0x06, // 0b00000110,    // 1
		0x5b, // 0b01011011,    // 2
		0x4f, // 0b01001111,    // 3
		0x66, // 0b01100110,    // 4
		0x6d, // 0b01101101,    // 5
		0x7d, // 0b01111101,    // 6
		0x07, // 0b00000111,    // 7
		0x7f, // 0b01111111,    // 8
		0x6f, // 0b01101111,    // 9
		0x77, // 0b01110111,    // A
		0x7c, // 0b01111100,    // b
		0x39, // 0b00111001,    // C
		0x5e, // 0b01011110,    // d
		0x79, // 0b01111001,    // E
		0x71, // 0b01110001     // F
};

struct tm;

typedef struct {
	gpio_num_t m_pin_clk;
	gpio_num_t m_pin_dta;
	uint8_t m_brightness;
} tm1637_lcd_t;

/**
 * @brief Constructs new LCD TM1637 object
 *
 * @param pin_clk GPIO pin for CLK input of LCD module
 * @param pin_data GPIO pin for DIO input of LCD module
 * @return
 */
tm1637_lcd_t * tm1637_init(gpio_num_t pin_clk, gpio_num_t pin_data);

/**
 * @brief Set brightness level. Note - will be set after next display render
 * @param lcd LCD object
 * @param level Brightness level 0..7 value
 */
void tm1637_set_brightness(tm1637_lcd_t * lcd, uint8_t level);

/**
 * @brief Set one-segment number, also controls dot of this segment
 * @param lcd LCD object
 * @param segment_idx Segment index (0..3)
 * @param num Number to set (0x00..0x0F, 0xFF for clear)
 * @param dot Display dot of this segment
 */
void tm1637_set_segment_number(tm1637_lcd_t * lcd, const uint8_t segment_idx, const uint8_t num, const bool dot);

/**
 * @brief Set one-segment raw segment data
 * @param lcd LCD object
 * @param segment_idx Segment index (0..3)
 * @param data Raw data, bitmask is XGFEDCBA
 */
void tm1637_set_segment_raw(tm1637_lcd_t * lcd, const uint8_t segment_idx, const uint8_t data);

/**
 * @brief Set full display number, in decimal encoding
 * @param lcd LCD object
 * @param number Display number (0...9999)
 */
void tm1637_set_number(tm1637_lcd_t * lcd, uint16_t number);

/**
 * @brief Set full display number, in decimal encoding + control leading zero
 * @param lcd LCD object
 * @param number Display number (0...9999)
 * @param lead_zero Display leading zero(s)
 */
void tm1637_set_number_lead(tm1637_lcd_t * lcd, uint16_t number, const bool lead_zero);

/**
 * @brief Set full display number, in decimal encoding + control leading zero + control dot display
 * @param lcd LCD object
 * @param number Display number (0...9999)
 * @param lead_zero Display leading zero(s)
 * @param dot_mask Dot mask, bits left-to-right
 */
void tm1637_set_number_lead_dot(tm1637_lcd_t * lcd, uint16_t number, const bool lead_zero, const uint8_t dot_mask);

#ifdef __cplusplus
}
#endif

#endif // TM1637_H
