#ifndef PCF8554_H
#define PCF8554_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO 22      // GPIO for SCL
#define I2C_MASTER_SDA_IO 21      // GPIO for SDA
#define I2C_MASTER_NUM I2C_NUM_0  // I2C port number
#define I2C_MASTER_FREQ_HZ 100000 // I2C clock speed
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0

#define PCF8574_ADDR 0x20 // Address when A0, A1, A2 = GND
// Function declarations
void i2c_master_init();
esp_err_t pcf8574_write(uint8_t data);
esp_err_t pcf8574_read(uint8_t *data);

#endif // PCF8554_H
