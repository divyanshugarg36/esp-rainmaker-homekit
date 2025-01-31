/* Switch demo implementation using button and RGB LED
   
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sdkconfig.h>

#include <iot_button.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h> 

#include <app_reset.h>
#include <ws2812_led.h>
#include "app_priv.h"

/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          CONFIG_EXAMPLE_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL  0

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

void app_indicator_set(bool state)
{
    if (state) {
        ws2812_led_set_rgb(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE);
    } else {
        ws2812_led_clear();
    }
}

static void app_indicator_init(void)
{
    ws2812_led_init();
    app_indicator_set(g_power_state1);
}
static void push_btn_cb(void *arg)
{
    bool new_state = !g_power_state1;
    app_driver_set_state(new_state);
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device1, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(new_state));
    app_homekit_update_state(new_state);
}

static void set_power_state(int gpioPin, bool target)
{
    gpio_set_level(gpioPin, target);
    app_indicator_set(target);
}

void setGpioPoweModeforOutput(int gpioPin){
    /* Configure power */
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 1,
    };
    io_conf.pin_bit_mask = ((uint64_t)1 << gpioPin);
    gpio_config(&io_conf);
}

void app_driver_init()
{
    button_handle_t btn_handle = iot_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LEVEL);
    if (btn_handle) {
        /* Register a callback for a button tap (short press) event */
        iot_button_set_evt_cb(btn_handle, BUTTON_CB_TAP, push_btn_cb, NULL);
        /* Register Wi-Fi reset and factory reset functionality on same button */
        app_reset_button_register(btn_handle, WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
    }

    setGpioPoweModeforOutput(DEVICE_1_OUTPUT_GPIO);
    setGpioPoweModeforOutput(DEVICE_2_OUTPUT_GPIO);
    setGpioPoweModeforOutput(DEVICE_3_OUTPUT_GPIO);
    setGpioPoweModeforOutput(DEVICE_4_OUTPUT_GPIO);
    setGpioPoweModeforOutput(DEVICE_5_OUTPUT_GPIO);
    setGpioPoweModeforOutput(DEVICE_6_OUTPUT_GPIO);
    setGpioPoweModeforOutput(DEVICE_7_OUTPUT_GPIO);

    app_indicator_init();
}

int IRAM_ATTR app_driver_set_state(bool state)
{
    if(g_power_state1 != state) {
        g_power_state1 = state;
        set_power_state(DEVICE_1_OUTPUT_GPIO, g_power_state1);
    }
    return ESP_OK;
}

// New function to set state of second switch
int IRAM_ATTR app_driver_set_state2(bool state)
{
    if(g_power_state2 != state) {
        g_power_state2 = state;
        set_power_state(DEVICE_2_OUTPUT_GPIO, g_power_state2);
    }
    return ESP_OK;
}

int IRAM_ATTR app_driver_set_state3(bool state)
{
    if(g_power_state3 != state) {
        g_power_state3 = state;
        set_power_state(DEVICE_3_OUTPUT_GPIO, g_power_state3);
    }
    return ESP_OK;
}
int IRAM_ATTR app_driver_set_state4(bool state)
{
    if(g_power_state4 != state) {
        g_power_state4 = state;
        set_power_state(DEVICE_4_OUTPUT_GPIO, g_power_state4);
    }
    return ESP_OK;
}
int IRAM_ATTR app_driver_set_state5(bool state)
{
    if(g_power_state5 != state) {
        g_power_state5 = state;
        set_power_state(DEVICE_5_OUTPUT_GPIO, g_power_state5);
    }
    return ESP_OK;
}
int IRAM_ATTR app_driver_set_state6(bool state)
{
    if(g_power_state6 != state) {
        g_power_state6 = state;
        set_power_state(DEVICE_6_OUTPUT_GPIO, g_power_state6);
    }
    return ESP_OK;
}
int IRAM_ATTR app_driver_set_state7(bool state)
{
    if(g_power_state7 != state) {
        g_power_state7 = state;
        set_power_state(DEVICE_7_OUTPUT_GPIO, g_power_state7);
    }
    return ESP_OK;
}

bool app_driver_get_state(void)
{
    return g_power_state1;
}

// New function to get state of second switch
bool app_driver_get_state2(void)
{
    return g_power_state2;
}