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
#include "app_nvs.c"

/* Power state of each device */
static bool g_power_state1 = BUTTON_ACTIVE_LOW;
static bool g_power_state2 = BUTTON_ACTIVE_LOW;
static bool g_power_state3 = BUTTON_ACTIVE_LOW;
static bool g_power_state4 = BUTTON_ACTIVE_LOW;
static bool g_power_state5 = BUTTON_ACTIVE_LOW;
static bool g_power_state6 = BUTTON_ACTIVE_LOW;

void init_power_states() {
    g_power_state1 = read_device_state(deviceList.device1.id);
    g_power_state2 = read_device_state(deviceList.device2.id);
    g_power_state3 = read_device_state(deviceList.device3.id);
    g_power_state4 = read_device_state(deviceList.device4.id);
    g_power_state5 = read_device_state(deviceList.device5.id);
    g_power_state6 = read_device_state(deviceList.device6.id);
}

void app_indicator_set(bool state)
{
    if (state) {
        ws2812_led_set_rgb(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE);
    } else {
        ws2812_led_clear();
    }
}

/* Function to set the output power state of a device */
static void set_power_state(int gpioPin, bool state)
{
    gpio_set_level(gpioPin, !state);
    app_indicator_set(state);
}

/*************************************************************
 * Functions to initialize the Output GPIOs
**************************************************************/

void configure_gpio_output(int deviceId, int gpioPin) {
    /* Configure power */
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 1,
    };
    io_conf.pin_bit_mask = ((uint64_t)1 << gpioPin);
    gpio_config(&io_conf);
}

void app_output_driver_init() {
    configure_gpio_output(deviceList.device1.id, deviceList.device1.gpio);
    configure_gpio_output(deviceList.device1.id, deviceList.device2.gpio);
    configure_gpio_output(deviceList.device1.id, deviceList.device3.gpio);
    configure_gpio_output(deviceList.device1.id, deviceList.device4.gpio);
    configure_gpio_output(deviceList.device1.id, deviceList.device5.gpio);
    configure_gpio_output(deviceList.device1.id, deviceList.device6.gpio);
    configure_gpio_output(deviceList.device1.id, deviceList.device7.gpio);
}

/*************************************************************
 * Functions to initialize the Input GPIOs
**************************************************************/

void IRAM_ATTR button_callback(void* arg)
{
    int gpioIn = (int) arg;
    gpio_input_task(gpioIn);
}

void app_input_driver_init() {
    button_handle_t button_1 = iot_button_create(DEVICE_1_INPUT_GPIO, BUTTON_ACTIVE_LOW);
    iot_button_set_evt_cb(button_1, BUTTON_CB_PUSH, button_callback, (void*) DEVICE_1_INPUT_GPIO);
    iot_button_set_evt_cb(button_1, BUTTON_CB_RELEASE, button_callback, (void*) DEVICE_1_INPUT_GPIO);

    button_handle_t button_2 = iot_button_create(DEVICE_2_INPUT_GPIO, BUTTON_ACTIVE_LOW);
    iot_button_set_evt_cb(button_2, BUTTON_CB_PUSH, button_callback, (void*) DEVICE_2_INPUT_GPIO);
    iot_button_set_evt_cb(button_2, BUTTON_CB_RELEASE, button_callback, (void*) DEVICE_2_INPUT_GPIO);

    button_handle_t button_3 = iot_button_create(DEVICE_3_INPUT_GPIO, BUTTON_ACTIVE_LOW);
    iot_button_set_evt_cb(button_3, BUTTON_CB_PUSH, button_callback, (void*) DEVICE_3_INPUT_GPIO);
    iot_button_set_evt_cb(button_3, BUTTON_CB_RELEASE, button_callback, (void*) DEVICE_3_INPUT_GPIO);

    button_handle_t button_4 = iot_button_create(DEVICE_4_INPUT_GPIO, BUTTON_ACTIVE_LOW);
    iot_button_set_evt_cb(button_4, BUTTON_CB_PUSH, button_callback, (void*) DEVICE_4_INPUT_GPIO);
    iot_button_set_evt_cb(button_4, BUTTON_CB_RELEASE, button_callback, (void*) DEVICE_4_INPUT_GPIO);

    button_handle_t button_5 = iot_button_create(DEVICE_5_INPUT_GPIO, BUTTON_ACTIVE_LOW);
    iot_button_set_evt_cb(button_5, BUTTON_CB_PUSH, button_callback, (void*) DEVICE_5_INPUT_GPIO);
    iot_button_set_evt_cb(button_5, BUTTON_CB_RELEASE, button_callback, (void*) DEVICE_5_INPUT_GPIO);

    button_handle_t button_6 = iot_button_create(DEVICE_6_INPUT_GPIO, BUTTON_ACTIVE_LOW);
    iot_button_set_evt_cb(button_6, BUTTON_CB_PUSH, button_callback, (void*) DEVICE_6_INPUT_GPIO);
    iot_button_set_evt_cb(button_6, BUTTON_CB_RELEASE, button_callback, (void*) DEVICE_6_INPUT_GPIO);
}

/*************************************************************
 * Function to set the power state of a device
 * Also writes data to NVS
**************************************************************/
int IRAM_ATTR app_driver_set_state(int deviceId, bool state) {
    printf("Setting state of device %d to %d\n", deviceId, state);
    switch (deviceId) {
        case DEVICE_1_ID:
            if (g_power_state1 != state) {
                g_power_state1 = state;
                set_power_state(deviceList.device1.gpio, g_power_state1);
                save_device_state(deviceId, g_power_state1);
            }
            return ESP_OK;
        case DEVICE_2_ID:
            if (g_power_state2 != state) {
                g_power_state2 = state;
                set_power_state(deviceList.device2.gpio, g_power_state2);
                save_device_state(deviceId, g_power_state2);
            }
            return ESP_OK;
        case DEVICE_3_ID:
            if (g_power_state3 != state) {
                g_power_state3 = state;
                set_power_state(deviceList.device3.gpio, g_power_state3);
                save_device_state(deviceId, g_power_state3);
            }
            return ESP_OK;
        case DEVICE_4_ID:
            if (g_power_state4 != state) {
                g_power_state4 = state;
                set_power_state(deviceList.device4.gpio, g_power_state4);
                save_device_state(deviceId, g_power_state4);
            }
            return ESP_OK;
        case DEVICE_5_ID:
            if (g_power_state5 != state) {
                g_power_state5 = state;
                set_power_state(deviceList.device5.gpio, g_power_state5);
                save_device_state(deviceId, g_power_state5);
            }
            return ESP_OK;
        case DEVICE_6_ID:
            if (g_power_state6 != state) {
                g_power_state6 = state;
                set_power_state(deviceList.device6.gpio, g_power_state6);
                save_device_state(deviceId, g_power_state6);
            }
            return ESP_OK;
        default:
            return ESP_FAIL;
    }
}

/*************************************************************
 * Function to get the power state of a device
**************************************************************/
bool IRAM_ATTR app_driver_get_state(int deviceId) {
    switch (deviceId)
    {
    case DEVICE_1_ID:
        return g_power_state1;
    case DEVICE_2_ID:
        return g_power_state2;
    case DEVICE_3_ID:
        return g_power_state3;
    case DEVICE_4_ID:
        return g_power_state4;
    case DEVICE_5_ID:
        return g_power_state5;
    case DEVICE_6_ID:
        return g_power_state6;
    default:
        return false;
    }
}

/*************************************************************
 * Function to initialize the app_driver
**************************************************************/
void app_driver_init() {
    /* Setup NVS storage */
    init_nvs();
    /* Read initial Power states from NVS */
    init_power_states();

    button_handle_t btn_handle = iot_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LOW);
    if (btn_handle) {
        /* Register Wi-Fi reset and factory reset functionality on same button */
        app_reset_button_register(btn_handle, WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
    }

    // Initialize the output GPIOs
    app_output_driver_init();

    ws2812_led_init();
    app_indicator_set(true);

    // This method will read initial state of the devices from Input GPIOs
    // it won't call rmaker and hma methods since they havent been initialized
    // But they will set the default state which further activates the devices
    app_input_driver_init();
}