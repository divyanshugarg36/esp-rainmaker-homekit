/**
 * @file app_main.c
 * @brief Example application for the PCF8574
 */

#include "pcf8574.h"

#define TAG "app"

void app_main() {
    i2c_master_init();
    printf("PCF8574 I2C Initialized!\n");

    while (1) {
        printf("Turning ON all pins (0xFF)...\n");
        pcf8574_write(0xFF);
        vTaskDelay(pdMS_TO_TICKS(5000));

        printf("Turning OFF all pins (0x00)...\n");
        pcf8574_write(0x00);
        vTaskDelay(pdMS_TO_TICKS(5000));

        uint8_t input_data;
        if (pcf8574_read(&input_data) == ESP_OK) {
            printf("PCF8574 Input Data: 0x%02X\n", input_data);
        } else {
            printf("Failed to read from PCF8574!\n");
        }
    }
}




