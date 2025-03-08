/* Switch demo implementation using button and RGB LED
   
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sdkconfig.h>

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
static bool g_power_state7 = BUTTON_ACTIVE_LOW;
static bool g_power_state8 = BUTTON_ACTIVE_LOW;

void init_power_states() {
    g_power_state1 = read_device_state(deviceList.device1.id);
    g_power_state2 = read_device_state(deviceList.device2.id);
    g_power_state3 = read_device_state(deviceList.device3.id);
    g_power_state4 = read_device_state(deviceList.device4.id);
    g_power_state5 = read_device_state(deviceList.device5.id);
    g_power_state6 = read_device_state(deviceList.device6.id);
    g_power_state7 = read_device_state(deviceList.device7.id);
    g_power_state8 = read_device_state(deviceList.device8.id);
}

static void flash_light_task(void *pvParameters) {
    bool state = false;
    for (int i = 0; i < 5; i++) {
        gpio_set_level(PROCESS_INDICATOR, state);
        vTaskDelay(pdMS_TO_TICKS(50));
        state = !state;
    }
    vTaskDelete(NULL);
}

/* Function to set the output power state of a device */
static void set_power_state(int gpioPin, bool state)
{
    gpio_set_level(gpioPin, !state);
    // app_indicator_set(state);
    // Run flash_light in a separate task to avoid blocking
    xTaskCreate(flash_light_task, "FlashLightTask", 2048, NULL, 5, NULL);
}

/*************************************************************
 * Functions to initialize the Output GPIOs
**************************************************************/

void configure_gpio_output(int gpioPin) {
    /* Configure power */
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 1,
    };
    io_conf.pin_bit_mask = ((uint64_t)1 << gpioPin);
    gpio_config(&io_conf);
}

void app_output_driver_init() {
    configure_gpio_output(deviceList.device1.gpio);
    configure_gpio_output(deviceList.device2.gpio);
    configure_gpio_output(deviceList.device3.gpio);
    configure_gpio_output(deviceList.device4.gpio);
    configure_gpio_output(deviceList.device5.gpio);
    configure_gpio_output(deviceList.device6.gpio);
    configure_gpio_output(deviceList.device7.gpio);
    configure_gpio_output(deviceList.device8.gpio);
    configure_gpio_output(POWER_INDICATOR);
    configure_gpio_output(PROCESS_INDICATOR);

    set_power_state(deviceList.device1.gpio, g_power_state1);
    set_power_state(deviceList.device2.gpio, g_power_state2);
    set_power_state(deviceList.device3.gpio, g_power_state3);
    set_power_state(deviceList.device4.gpio, g_power_state4);
    set_power_state(deviceList.device5.gpio, g_power_state5);
    set_power_state(deviceList.device6.gpio, g_power_state6);
    set_power_state(deviceList.device7.gpio, g_power_state7);
    set_power_state(deviceList.device8.gpio, g_power_state8);
    set_power_state(POWER_INDICATOR, false);
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
        case DEVICE_7_ID:
            if (g_power_state7 != state) {
                g_power_state7 = state;
                set_power_state(deviceList.device7.gpio, g_power_state7);
                save_device_state(deviceId, g_power_state7);
            }
            return ESP_OK;
        case DEVICE_8_ID:
            if (g_power_state8 != state) {
                g_power_state8 = state;
                set_power_state(deviceList.device8.gpio, g_power_state8);
                save_device_state(deviceId, g_power_state8);
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
    case DEVICE_7_ID:
        return g_power_state7;
    case DEVICE_8_ID:
        return g_power_state8;
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
}