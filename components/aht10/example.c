/**
 * @file app_main.c
 * @brief Example application for the AHT10
 */

#include "aht10.h"

#define TAG "app"

void app_main()
{
    float temperature = 0.0, humidity = 0.0;

    i2c_master_init();
    aht10_init();

    while (1)
    {
        aht10_get_temp_humidity(&temperature, &humidity);
        printf("Temperature: %.2f °C, Humidity: %.2f %%\n", temperature, humidity);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
