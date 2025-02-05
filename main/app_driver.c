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

#include "nvs_flash.h"
#include "nvs.h"

#include <app_reset.h>
#include <ws2812_led.h>
#include "app_priv.h"

static bool g_power_state1 = GPIO_LOW;
static bool g_power_state2 = GPIO_LOW;
static bool g_power_state3 = GPIO_LOW;
static bool g_power_state4 = GPIO_LOW;
static bool g_power_state5 = GPIO_LOW;
static bool g_power_state6 = GPIO_LOW;

/***************************************************************
 * NVS Code for Local Storage of previous states
 * The states will persist even after a reset
***************************************************************/
void init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("ERASE! This happened.\n");
        nvs_flash_erase();
        nvs_flash_init();
    }
}

void save_device_state(int device, int state) {
    nvs_handle_t my_handle;
    if (nvs_open("storage", NVS_READWRITE, &my_handle) == ESP_OK) {
        char key[10];
        sprintf(key, "device_%d", device);
        nvs_set_i32(my_handle, key, state);
        nvs_commit(my_handle);
        nvs_close(my_handle);
    }
}

int read_device_state(int device) {
    nvs_handle_t my_handle;
    int32_t state = GPIO_LOW;
    if (nvs_open("storage", NVS_READONLY, &my_handle) == ESP_OK) {
        char key[10];
        sprintf(key, "device_%d", device);
        nvs_get_i32(my_handle, key, &state);
        nvs_close(my_handle);
    }
    return state;
}

void init_power_states() {
    g_power_state1 = read_device_state(deviceList.device1.id);
    g_power_state2 = read_device_state(deviceList.device2.id);
    g_power_state3 = read_device_state(deviceList.device3.id);
    g_power_state4 = read_device_state(deviceList.device4.id);
    g_power_state5 = read_device_state(deviceList.device5.id);
    g_power_state6 = read_device_state(deviceList.device6.id);
    printf("init_power_states %d %d %d %d %d %d\n", g_power_state1, g_power_state2, g_power_state3, g_power_state4, g_power_state5, g_power_state6);
}

void app_indicator_set(bool state)
{
    if (state) {
        ws2812_led_set_rgb(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE);
    } else {
        ws2812_led_clear();
    }
}

static void set_power_state(int gpioPin, bool target)
{
    gpio_set_level(gpioPin, target);
    app_indicator_set(target);
}

void configure_gpio_output(int gpioPin){
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
    /* Setup NVS storage */
    init_nvs();
    /* Read initial Power states from NVS */
    init_power_states();

    button_handle_t btn_handle = iot_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LEVEL);
    if (btn_handle) {
        /* Register Wi-Fi reset and factory reset functionality on same button */
        app_reset_button_register(btn_handle, WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
    }

    configure_gpio_output(deviceList.device1.gpio);
    configure_gpio_output(deviceList.device2.gpio);
    configure_gpio_output(deviceList.device3.gpio);
    configure_gpio_output(deviceList.device4.gpio);
    configure_gpio_output(deviceList.device5.gpio);
    configure_gpio_output(deviceList.device6.gpio);
    configure_gpio_output(deviceList.device7.gpio);

    ws2812_led_init();
    app_indicator_set(true);

}


static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_input_evt_queue, &gpio_num, NULL);
}

/*************************************************************
 * Function to initialize the Input GPIOs
**************************************************************/
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

/*************************************************************
 * Function to set the power state of a device
 * Also writes data to NVS
**************************************************************/
int IRAM_ATTR app_driver_set_state(int deviceId, bool state) {
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