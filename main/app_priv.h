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

#define DEFAULT_POWER false

extern esp_rmaker_device_t *device1;
extern esp_rmaker_device_t *device2;
extern esp_rmaker_device_t *device3;
extern esp_rmaker_device_t *device4;
extern esp_rmaker_device_t *device5;
extern esp_rmaker_device_t *device6;
extern esp_rmaker_device_t *device7;

/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          CONFIG_EXAMPLE_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL  0

// Declare Device it to make it easy to identify
#define DEVICE_1_ID   1
#define DEVICE_2_ID   2
#define DEVICE_3_ID   3
#define DEVICE_4_ID   4
#define DEVICE_5_ID   5
#define DEVICE_6_ID   6
#define DEVICE_7_ID   7

/* This is the GPIO on which the power will be set */
#define DEVICE_1_OUTPUT_GPIO   17
#define DEVICE_2_OUTPUT_GPIO   16
#define DEVICE_3_OUTPUT_GPIO   15
#define DEVICE_4_OUTPUT_GPIO   14
#define DEVICE_5_OUTPUT_GPIO   13
#define DEVICE_6_OUTPUT_GPIO   12
#define DEVICE_7_OUTPUT_GPIO   10

static bool g_power_state1 = DEFAULT_POWER;
static bool g_power_state2 = DEFAULT_POWER;
static bool g_power_state3 = DEFAULT_POWER;
static bool g_power_state4 = DEFAULT_POWER;
static bool g_power_state5 = DEFAULT_POWER;
static bool g_power_state6 = DEFAULT_POWER;
static bool g_power_state7 = DEFAULT_POWER;

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
} Switch;

typedef struct {
    Switch device1;
    Switch device2;
    Switch device3;
    Switch device4;
    Switch device5;
    Switch device6;
    Switch device7;
} Devices;

static const Devices deviceList = {
    .device1 = {
        .id = DEVICE_1_ID,
        .name = "Main Light",
        .gpio = DEVICE_1_OUTPUT_GPIO
    },
    .device2 = {
        .id = DEVICE_2_ID,
        .name = "Small Light 1",
        .gpio = DEVICE_2_OUTPUT_GPIO
    },
    .device3 = {
        .id = DEVICE_3_ID,
        .name = "Small Light 2",
        .gpio = DEVICE_3_OUTPUT_GPIO
    },
    .device4 = {
        .id = DEVICE_4_ID,
        .name = "Fan 1",
        .gpio = DEVICE_4_OUTPUT_GPIO
    },
    .device5 = {
        .id = DEVICE_5_ID,
        .name = "Fan 2",
        .gpio = DEVICE_5_OUTPUT_GPIO
    },
    .device6 = {
        .id = DEVICE_6_ID,
        .name = "Socket",
        .gpio = DEVICE_6_OUTPUT_GPIO
    },
    .device7 = {
        .id = DEVICE_7_ID,
        .name = "Ceiling Light",
        .gpio = DEVICE_7_OUTPUT_GPIO
    }
};


void app_indicator_set(bool state);
void app_driver_init(void);
int app_driver_set_state(int deviceId, bool state);

bool app_driver_get_state(int deviceId);

esp_err_t app_homekit_start(bool init_state);
esp_err_t app_homekit_update_state(bool state);
esp_err_t app_homekit_update_state2(bool state);
