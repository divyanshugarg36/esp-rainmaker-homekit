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
extern int percent_to_display(int input);
extern int display_to_percent(int input);
extern void setDisplayData();

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

#define DEVICE_TEMPERATURE_MODE   103
#define DEVICE_TEMPERATURE_BRIGHTNESS   104

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

typedef struct {
   int id;
   char *name;
   int gpio;
   int gpioIn;
} Switch;

typedef struct {
    Switch device1;
    Switch device2;
    Switch device3;
    Switch device4;
    Switch device5;
    Switch device6;
    Switch device7;
    Switch device8;
    Switch temperatureDevice;
    Switch humidityDevice;
} Devices;

static const Devices deviceList = {
    .device1 = {
        .id = DEVICE_1_ID,
        .name = "Inner Socket",
        .gpio = DEVICE_1_OUTPUT_GPIO,
        .gpioIn = DEVICE_1_PCF_GPIO,
    },
    .device2 = {
        .id = DEVICE_2_ID,
        .name = "Table Socket R",
        .gpio = DEVICE_2_OUTPUT_GPIO,
        .gpioIn = DEVICE_2_PCF_GPIO,
    },
    .device3 = {
        .id = DEVICE_3_ID,
        .name = "Big Lamp",
        .gpio = DEVICE_3_OUTPUT_GPIO,
        .gpioIn = DEVICE_3_PCF_GPIO,
    },
    .device4 = {
        .id = DEVICE_4_ID,
        .name = "Table Socket L",
        .gpio = DEVICE_4_OUTPUT_GPIO,
        .gpioIn = DEVICE_4_PCF_GPIO,
    },
    .device5 = {
        .id = DEVICE_5_ID,
        .name = "Outer bulb",
        .gpio = DEVICE_5_OUTPUT_GPIO,
        .gpioIn = DEVICE_5_PCF_GPIO,
    },
    .device6 = {
        .id = DEVICE_6_ID,
        .name = "Small Lamp",
        .gpio = DEVICE_6_OUTPUT_GPIO,
        .gpioIn = DEVICE_6_PCF_GPIO,
    },
    .device7 = {
        .id = DEVICE_7_ID,
        .name = "Ceiling Light",
        .gpio = DEVICE_7_OUTPUT_GPIO,
        .gpioIn = DEVICE_7_PCF_GPIO,
    },
    .device8 = {
        .id = DEVICE_8_ID,
        .name = "Ceiling Light 2",
        .gpio = DEVICE_8_OUTPUT_GPIO,
        .gpioIn = DEVICE_8_PCF_GPIO,
    },
    .temperatureDevice = {
        .id = DEVICE_TEMPERATURE,
        .name = "Temperature",
    },
    .humidityDevice = {
        .id = DEVICE_HUMIDITY,
        .name = "Humidity",
    }
};

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
bool compare_mac_addresses(const uint8_t mac1[6], const uint8_t mac2[6]);