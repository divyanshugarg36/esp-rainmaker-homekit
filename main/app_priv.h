/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define MANUFACTURER "Garg's Home"
#define MODEL "homekit_switch"
#define SERIAL_NUMBER "001122334455"
#define FIRMWARE_REVISION "1.0"

#define UNASSIGNED "UNASSIGNED"

extern esp_rmaker_device_t *device1;
extern esp_rmaker_device_t *device2;
extern esp_rmaker_device_t *device3;
extern esp_rmaker_device_t *device4;
extern esp_rmaker_device_t *device5;
extern esp_rmaker_device_t *device6;
extern esp_rmaker_device_t *device7;
extern esp_rmaker_device_t *device8;
extern esp_rmaker_device_t *temperatureDevice;

extern esp_rmaker_param_t *temperature_param;
extern esp_rmaker_param_t *temperature_brightness_param;
extern esp_rmaker_param_t *temperature_display_param;
extern esp_rmaker_param_t *humidity_param;

extern int brightness;
extern int displayMode;

extern bool isAHT10Connected;
extern bool isPCF8574Connected;

/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          0

// Declare Device it to make it easy to identify
#define DEVICE_1_ID   1
#define DEVICE_2_ID   2
#define DEVICE_3_ID   3
#define DEVICE_4_ID   4
#define DEVICE_5_ID   5
#define DEVICE_6_ID   6
#define DEVICE_7_ID   7
#define DEVICE_8_ID   8
#define DEVICE_TEMPERATURE   101
#define DEVICE_HUMIDITY   102
#define DEVICE_DISPLAY_BRIGHTNESS   103
#define DEVICE_DISPLAY_MODE   104

/* This is the GPIO on which the power will be set */
#define LCD_CLK 18
#define LCD_DTA 19

#define I2C_MASTER_SCL_IO 22      // GPIO for SCL
#define I2C_MASTER_SDA_IO 21      // GPIO for SDA

#define DEVICE_1_OUTPUT_GPIO   23
#define DEVICE_2_OUTPUT_GPIO   5
#define DEVICE_3_OUTPUT_GPIO   17
#define DEVICE_4_OUTPUT_GPIO   16
#define DEVICE_5_OUTPUT_GPIO   4
#define DEVICE_6_OUTPUT_GPIO   15
#define DEVICE_7_OUTPUT_GPIO   32
#define DEVICE_8_OUTPUT_GPIO   33

// AVAILABLE_GPIOS 14, 27

#define DEVICE_1_PCF_GPIO   0
#define DEVICE_2_PCF_GPIO   1
#define DEVICE_3_PCF_GPIO   2
#define DEVICE_4_PCF_GPIO   3
#define DEVICE_5_PCF_GPIO   4
#define DEVICE_6_PCF_GPIO   5
#define DEVICE_7_PCF_GPIO   6
#define DEVICE_8_PCF_GPIO   7

#define PROCESS_INDICATOR   25
#define POWER_INDICATOR     26

#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10

void app_driver_init(void);
int app_driver_set_state(int deviceId, bool state);

bool app_driver_get_state(int deviceId);

esp_err_t app_homekit_start();
esp_err_t app_homekit_update_state(int deviceId, bool state);
esp_err_t app_homekit_update_temperature_state(int deviceId, float state);
esp_err_t app_homekit_update_brightness_state(int deviceId, int state);

void init_power_states();

void init_nvs();
void save_device_state(int device, int state);
int read_device_state(int deviceId);

void gpio_input_task(int gpioIn, int val);

void set_display_data();
