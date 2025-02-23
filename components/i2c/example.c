/**
 * @file app_main.c
 * @brief Example application for the I2C
 */

#include "i2c.h"

#define TAG "app"

#define DEVICE_ADDRESS 0x38  // Change this to the address of your device (e.g., AHT10 = 0x38, PCF8574 = 0x20)

void app_main()
{
    i2c_master_init();
    if (check_i2c_device(DEVICE_ADDRESS)) {
        printf("Device found at 0x%02X\n", DEVICE_ADDRESS);
    } else {
        printf("Device NOT found at 0x%02X\n", DEVICE_ADDRESS);
    }
}
