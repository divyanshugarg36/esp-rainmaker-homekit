/**
 * ESP-32 IDF library for control TM1637 LCD 7-Segment display
 *
 * Author: Petro <petro@petro.ws>
 *
 * Project homepage: https://github.com/petrows/esp-32-tm1637
 * Example: https://github.com/petrows/esp-32-tm1637-example
 *
 */

#include "tm1637.h"

#include <stdlib.h>
#include <string.h>
#include <rom/ets_sys.h>

#define TM1637_ADDR_AUTO  0x40
#define TM1637_ADDR_FIXED 0x44

static void tm1637_start(tm1637_lcd_t * lcd);
static void tm1637_stop(tm1637_lcd_t * lcd);
static void tm1637_send_byte(tm1637_lcd_t * lcd, uint8_t byte);
static void tm1637_delay();

void tm1637_start(tm1637_lcd_t * lcd)
{
	// Send start bit to TM1637
	gpio_set_level(lcd->m_pin_clk, 1);
	gpio_set_level(lcd->m_pin_dta, 1);
	tm1637_delay();
	gpio_set_level(lcd->m_pin_dta, 0);
	tm1637_delay();
	gpio_set_level(lcd->m_pin_clk, 0);
	tm1637_delay();
}

void tm1637_stop(tm1637_lcd_t * lcd)
{
	gpio_set_level(lcd->m_pin_clk, 0);
	gpio_set_level(lcd->m_pin_dta, 0);
	tm1637_delay();
	gpio_set_level(lcd->m_pin_clk, 1);
	gpio_set_level(lcd->m_pin_dta, 1);
	tm1637_delay();
}

void tm1637_send_byte(tm1637_lcd_t * lcd, uint8_t byte)
{
	for (uint8_t i=0; i<8; ++i)
	{
		gpio_set_level(lcd->m_pin_clk, 0);
		tm1637_delay();
		gpio_set_level(lcd->m_pin_dta, byte & 0x01); // Send current bit
		gpio_set_level(lcd->m_pin_clk, 1);
		byte >>= 1;
		tm1637_delay();
	}

	gpio_set_level(lcd->m_pin_clk, 0); //wait for the ACK
	gpio_set_level(lcd->m_pin_dta, 1);
	tm1637_delay();
	gpio_set_level(lcd->m_pin_clk, 1);
	gpio_set_direction(lcd->m_pin_dta, GPIO_MODE_INPUT);
	tm1637_delay();
	uint8_t ack = gpio_get_level(lcd->m_pin_dta);
	if (ack == 0) {
		gpio_set_direction(lcd->m_pin_dta, GPIO_MODE_OUTPUT);
		gpio_set_level(lcd->m_pin_dta, 0);
	}
	tm1637_delay();
	gpio_set_direction(lcd->m_pin_dta, GPIO_MODE_OUTPUT);
	tm1637_delay();
}

void tm1637_delay()
{
	ets_delay_us(50);
}

// PUBLIC PART:

tm1637_lcd_t * tm1637_init(gpio_num_t pin_clk, gpio_num_t pin_data) {
	tm1637_lcd_t * lcd = malloc(sizeof(tm1637_lcd_t));
	lcd->m_pin_clk = pin_clk;
	lcd->m_pin_dta = pin_data;
	lcd->m_brightness = 0x07;
	gpio_set_direction(pin_clk, GPIO_MODE_OUTPUT);
	gpio_set_direction(pin_data, GPIO_MODE_OUTPUT);
	gpio_set_level(pin_clk, 0);
	gpio_set_level(pin_data, 0);
	return lcd;
}

void tm1637_set_brightness(tm1637_lcd_t * lcd, uint8_t level)
{
	if (level > 0x07) { level = 0x07; } // Check max level
	lcd->m_brightness = level;
}

void tm1637_set_segment_number(tm1637_lcd_t * lcd, const uint8_t segment_idx, const uint8_t num, const bool dot)
{
	uint8_t seg_data = 0x00;

	if (num < (sizeof(tm1637_symbols)/sizeof(tm1637_symbols[0]))) {
		seg_data = tm1637_symbols[num]; // Select proper segment image
	}

	if (dot) {
		seg_data |= 0x80; // Set DOT segment flag
	}

	tm1637_set_segment_raw(lcd, segment_idx, seg_data);
}

void tm1637_set_segment_raw(tm1637_lcd_t * lcd, const uint8_t segment_idx, const uint8_t data)
{
	tm1637_start(lcd);
	tm1637_send_byte(lcd, TM1637_ADDR_FIXED);
	tm1637_stop(lcd);
	tm1637_start(lcd);
	tm1637_send_byte(lcd, segment_idx | 0xc0);
	tm1637_send_byte(lcd, data);
	tm1637_stop(lcd);
	tm1637_start(lcd);
	tm1637_send_byte(lcd, lcd->m_brightness | 0x88);
	tm1637_stop(lcd);
}

void tm1637_set_number(tm1637_lcd_t * lcd, uint16_t number)
{
	tm1637_set_number_lead_dot(lcd, number, false, 0x00);
}

void tm1637_set_number_lead(tm1637_lcd_t * lcd, uint16_t number, const bool lead_zero)
{
	tm1637_set_number_lead_dot(lcd, number, lead_zero, 0x00);
}

void tm1637_set_number_lead_dot(tm1637_lcd_t * lcd, uint16_t number, bool lead_zero, const uint8_t dot_mask)
{
	uint8_t lead_number = lead_zero ? 0xFF : tm1637_symbols[0];

	if (number < 10) {
		tm1637_set_segment_number(lcd, 3, number, dot_mask & 0x01);
		tm1637_set_segment_number(lcd, 2, lead_number, dot_mask & 0x02);
		tm1637_set_segment_number(lcd, 1, lead_number, dot_mask & 0x04);
		tm1637_set_segment_number(lcd, 0, lead_number, dot_mask & 0x08);
	} else if (number < 100) {
		tm1637_set_segment_number(lcd, 3, number % 10, dot_mask & 0x01);
		tm1637_set_segment_number(lcd, 2, (number / 10) % 10, dot_mask & 0x02);
		tm1637_set_segment_number(lcd, 1, lead_number, dot_mask & 0x04);
		tm1637_set_segment_number(lcd, 0, lead_number, dot_mask & 0x08);
	} else if (number < 1000) {
		tm1637_set_segment_number(lcd, 3, number % 10, dot_mask & 0x01);
		tm1637_set_segment_number(lcd, 2, (number / 10) % 10, dot_mask & 0x02);
		tm1637_set_segment_number(lcd, 1, (number / 100) % 10, dot_mask & 0x04);
		tm1637_set_segment_number(lcd, 0, lead_number, dot_mask & 0x08);
	} else {
		tm1637_set_segment_number(lcd, 3, number % 10, dot_mask & 0x01);
		tm1637_set_segment_number(lcd, 2, (number / 10) % 10, dot_mask & 0x02);
		tm1637_set_segment_number(lcd, 1, (number / 100) % 10, dot_mask & 0x04);
		tm1637_set_segment_number(lcd, 0, (number / 1000) % 10, dot_mask & 0x08);
	}
}

void tm1637_set_temperature(tm1637_lcd_t * lcd, uint8_t temperature) {
    tm1637_set_segment_number(lcd, 0, (temperature / 10) % 10, false);  // Tens place
    tm1637_set_segment_number(lcd, 1, temperature % 10, false);    // Ones place
    tm1637_set_segment_raw(lcd, 2, 0x63);  // Degree symbol
    tm1637_set_segment_raw(lcd, 3, tm1637_symbols[12]);  // 'C' character
}

void tm1637_set_humidity(tm1637_lcd_t * lcd, uint8_t humidity) {
    tm1637_set_segment_raw(lcd, 0, 0x76);  // H character
    tm1637_set_segment_raw(lcd, 1, 0x40);  // Dash symbol
    tm1637_set_segment_number(lcd, 2, (humidity / 10) % 10, false);  // Tens place
    tm1637_set_segment_number(lcd, 3, humidity % 10, false);    // Ones place
}