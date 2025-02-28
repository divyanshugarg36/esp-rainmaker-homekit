#ifndef PCF8574_H
#define PCF8574_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define I2C_MASTER_NUM I2C_NUM_0  // I2C port number

#define PCF8574_ADDR 0x20 // Address when A0, A1, A2 = GND

// Function declarations
esp_err_t pcf8574_write(uint8_t data);
esp_err_t pcf8574_read(uint8_t *data);
int get_pcf8574_pin_state(uint8_t gpio);

#endif // PCF8574_H
