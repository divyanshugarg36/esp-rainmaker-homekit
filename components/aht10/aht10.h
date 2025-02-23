#ifndef AHT10_H
#define AHT10_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define I2C_MASTER_NUM I2C_NUM_0  // I2C port number

#define AHT10_ADDR 0x38          // I2C address of the AHT10 sensor
#define AHT10_CMD_TRIGGER 0xAC   // Command to trigger measurement
#define AHT10_CMD_SOFTRESET 0xBA // Command to reset the sensor
#define AHT10_CMD_INIT 0xE1      // Command to initialize the sensor

// Function declarations
esp_err_t aht10_write_command(uint8_t *cmd, size_t len);
esp_err_t aht10_read_data(uint8_t *data, size_t length);
void aht10_init();
void aht10_get_temp_humidity(float *temperature, float *humidity);

#endif // AHT10_H
