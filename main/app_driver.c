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
    app_driver_set_state(deviceList.device1.id, new_state);
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device1, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(new_state));
    app_homekit_update_state(deviceList.device1.id, new_state);
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

    setGpioPoweModeforOutput(deviceList.device1.gpio);
    setGpioPoweModeforOutput(deviceList.device2.gpio);
    setGpioPoweModeforOutput(deviceList.device3.gpio);
    setGpioPoweModeforOutput(deviceList.device4.gpio);
    setGpioPoweModeforOutput(deviceList.device5.gpio);
    setGpioPoweModeforOutput(deviceList.device6.gpio);
    setGpioPoweModeforOutput(deviceList.device7.gpio);

    app_indicator_init();
}


static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_input_evt_queue, &gpio_num, NULL);
}

void app_input_driver_init(){
     //zero-initialize the config structure.
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_ANYEDGE,
        .pin_bit_mask = GPIO_INPUT_PIN_SEL,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    gpio_input_evt_queue = xQueueCreate(1, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_input_task, "gpio_input_task", 2048, NULL, 10, NULL);

    gpio_isr_handler_add(DEVICE_1_INPUT_GPIO, gpio_isr_handler, (void*) DEVICE_1_INPUT_GPIO);
    gpio_isr_handler_add(DEVICE_2_INPUT_GPIO, gpio_isr_handler, (void*) DEVICE_2_INPUT_GPIO);
    gpio_isr_handler_add(DEVICE_3_INPUT_GPIO, gpio_isr_handler, (void*) DEVICE_3_INPUT_GPIO);
    gpio_isr_handler_add(DEVICE_4_INPUT_GPIO, gpio_isr_handler, (void*) DEVICE_4_INPUT_GPIO);
    gpio_isr_handler_add(DEVICE_5_INPUT_GPIO, gpio_isr_handler, (void*) DEVICE_5_INPUT_GPIO);
    gpio_isr_handler_add(DEVICE_6_INPUT_GPIO, gpio_isr_handler, (void*) DEVICE_6_INPUT_GPIO);

    printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());

    g_power_state1=!gpio_get_level(deviceList.device1.gpioIn);
    g_power_state2=!gpio_get_level(deviceList.device2.gpioIn);
    g_power_state3=!gpio_get_level(deviceList.device3.gpioIn);
    g_power_state4=!gpio_get_level(deviceList.device4.gpioIn);
    g_power_state5=!gpio_get_level(deviceList.device5.gpioIn);
    g_power_state6=!gpio_get_level(deviceList.device6.gpioIn);
}

int IRAM_ATTR app_driver_set_state(int deviceId, bool state) {
    switch (deviceId) {
        case DEVICE_1_OUTPUT_GPIO:
            if(g_power_state1 != state) {
                g_power_state1 = state;
                set_power_state(DEVICE_1_OUTPUT_GPIO, g_power_state1);
            }
            return ESP_OK;
        case DEVICE_2_OUTPUT_GPIO:
            if(g_power_state2 != state) {
                g_power_state2 = state;
                set_power_state(DEVICE_2_OUTPUT_GPIO, g_power_state2);
            }
            return ESP_OK;
        case DEVICE_3_OUTPUT_GPIO:
            if(g_power_state3 != state) {
                g_power_state3 = state;
                set_power_state(DEVICE_3_OUTPUT_GPIO, g_power_state3);
            }
            return ESP_OK;
        case DEVICE_4_OUTPUT_GPIO:
            if(g_power_state4 != state) {
                g_power_state4 = state;
                set_power_state(DEVICE_4_OUTPUT_GPIO, g_power_state4);
            }
            return ESP_OK;
        case DEVICE_5_OUTPUT_GPIO:
            if(g_power_state5 != state) {
                g_power_state5 = state;
                set_power_state(DEVICE_5_OUTPUT_GPIO, g_power_state5);
            }
            return ESP_OK;
        case DEVICE_6_OUTPUT_GPIO:
            if(g_power_state6 != state) {
                g_power_state6 = state;
                set_power_state(DEVICE_6_OUTPUT_GPIO, g_power_state6);
            }
            return ESP_OK;
        default:
            return ESP_FAIL;
    }
}


bool app_driver_get_state(int deviceId) {
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