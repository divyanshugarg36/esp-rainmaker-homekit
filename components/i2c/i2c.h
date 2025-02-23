#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define I2C_MASTER_NUM I2C_NUM_0  // I2C port number
#define I2C_MASTER_SDA_IO 21      // GPIO for SDA
#define I2C_MASTER_SCL_IO 22      // GPIO for SCL
#define I2C_MASTER_FREQ_HZ 100000 // I2C clock frequency
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TIMEOUT_MS 1000  // Timeout in milliseconds

// Function declarations
void i2c_master_init();
bool check_i2c_device(uint8_t address);

#endif // I2C_H
