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

extern QueueHandle_t gpio_input_evt_queue;

extern esp_rmaker_device_t *device1;
extern esp_rmaker_device_t *device2;
extern esp_rmaker_device_t *device3;
extern esp_rmaker_device_t *device4;
extern esp_rmaker_device_t *device5;
extern esp_rmaker_device_t *device6;
extern esp_rmaker_param_t *temperature_param;
extern esp_rmaker_param_t *humidity_param;

extern bool isAHT10Connected;


/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          0

// Declare Device it to make it easy to identify
#define DEVICE_1_ID   1
#define DEVICE_2_ID   2
#define DEVICE_3_ID   3
#define DEVICE_4_ID   4
#define DEVICE_5_ID   5
#define DEVICE_6_ID   6
#define DEVICE_TEMPERATURE   101
#define DEVICE_HUMIDITY   102

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

#define DEVICE_1_INPUT_GPIO   14
#define DEVICE_2_INPUT_GPIO   25
#define DEVICE_3_INPUT_GPIO   26
#define DEVICE_4_INPUT_GPIO   27
#define DEVICE_5_INPUT_GPIO   32
#define DEVICE_6_INPUT_GPIO   33

/* These values correspoind to H,S,V = 120,100,10 */
#define DEFAULT_RED     0
#define DEFAULT_GREEN   25
#define DEFAULT_BLUE    0

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
    Switch temperatureDevice;
    Switch humidityDevice;
} Devices;

static const Devices deviceList = {
    .device1 = {
        .id = DEVICE_1_ID,
        .name = "Main Light",
        .gpio = DEVICE_1_OUTPUT_GPIO,
        .gpioIn = DEVICE_1_INPUT_GPIO,
    },
    .device2 = {
        .id = DEVICE_2_ID,
        .name = "Small Light 1",
        .gpio = DEVICE_2_OUTPUT_GPIO,
        .gpioIn = DEVICE_2_INPUT_GPIO,
    },
    .device3 = {
        .id = DEVICE_3_ID,
        .name = "Small Light 2",
        .gpio = DEVICE_3_OUTPUT_GPIO,
        .gpioIn = DEVICE_3_INPUT_GPIO,
    },
    .device4 = {
        .id = DEVICE_4_ID,
        .name = "Fan 1",
        .gpio = DEVICE_4_OUTPUT_GPIO,
        .gpioIn = DEVICE_4_INPUT_GPIO,
    },
    .device5 = {
        .id = DEVICE_5_ID,
        .name = "Fan 2",
        .gpio = DEVICE_5_OUTPUT_GPIO,
        .gpioIn = DEVICE_5_INPUT_GPIO,
    },
    .device6 = {
        .id = DEVICE_6_ID,
        .name = "Socket",
        .gpio = DEVICE_6_OUTPUT_GPIO,
        .gpioIn = DEVICE_6_INPUT_GPIO,
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


void app_indicator_set(bool state);
void app_driver_init(void);
int app_driver_set_state(int deviceId, bool state);

bool app_driver_get_state(int deviceId);

esp_err_t app_homekit_start();
esp_err_t app_homekit_update_state(int deviceId, bool state);

void init_power_states();

void init_nvs();
void save_device_state(int device, int state);
int read_device_state(int deviceId);

void gpio_input_task(int gpioIn);
