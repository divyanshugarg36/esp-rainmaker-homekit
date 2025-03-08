/* HomeKit Switch Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <driver/gpio.h>

#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_ota.h>
#include <esp_rmaker_schedule.h>
#include <esp_rmaker_scenes.h>
#include <esp_rmaker_console.h>
#include <esp_rmaker_common_events.h>

#include <app_insights.h>

#include "app_wifi_with_homekit.h"
#include "app_priv.h"

// Custom Components
#include "i2c.h"
#include "aht10.h"
#include "tm1637.h"
#include "pcf8574.h"
#include "rmaker_custom_params.h"

static const char *TAG = "app_main";

esp_rmaker_node_t *node;

esp_rmaker_device_t *device1;
esp_rmaker_device_t *device2;
esp_rmaker_device_t *device3;
esp_rmaker_device_t *device4;
esp_rmaker_device_t *device5;
esp_rmaker_device_t *device6;
esp_rmaker_device_t *device7;
esp_rmaker_device_t *device8;
esp_rmaker_device_t *temperatureDevice;
esp_rmaker_device_t *humidityDevice;

esp_rmaker_param_t *power_param1;
esp_rmaker_param_t *power_param2;
esp_rmaker_param_t *power_param3;
esp_rmaker_param_t *power_param4;
esp_rmaker_param_t *power_param5;
esp_rmaker_param_t *power_param6;
esp_rmaker_param_t *power_param7;
esp_rmaker_param_t *power_param8;
esp_rmaker_param_t *temperature_param;
esp_rmaker_param_t *temperature_brightness_param;
esp_rmaker_param_t *temperature_display_param;
esp_rmaker_param_t *humidity_param;

tm1637_lcd_t *lcd;
float temperature = 0.0, humidity = 0.0;
int brightness = 0;
int displayMode = 0;

bool isAHT10Connected = false;
bool isPCF8574Connected = false;

void setDisplayData() {
    tm1637_set_brightness(lcd, brightness);
    if (displayMode == 0) {
        tm1637_set_temperature(lcd, (uint8_t)temperature);
    } else {
        tm1637_set_humidity(lcd, (uint8_t)humidity);
    }
}

int display_to_percent(int input) {
    if (input < 0) input = 0;
    if (input > 7) input = 7;
    return (input * 100) / 7;
}

int percent_to_display(int input) {
    if (input < 0) input = 0;    // Clamp below 0
    if (input > 100) input = 100; // Clamp above 100
    return (input * 7) / 100;
}

void IRAM_ATTR gpio_input_task(int gpioIn, int val)
{
    switch (gpioIn)
    {
    case DEVICE_1_PCF_GPIO:
        app_driver_set_state(deviceList.device1.id, val);
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device1, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(val));
        app_homekit_update_state(deviceList.device1.id, val);
        break;
    case DEVICE_2_PCF_GPIO:
        app_driver_set_state(deviceList.device2.id, val);
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device2, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(val)
        );
        app_homekit_update_state(deviceList.device2.id, val);
        break;
    case DEVICE_3_PCF_GPIO:
        app_driver_set_state(deviceList.device3.id, val);
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device3, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(val)
        );
        app_homekit_update_state(deviceList.device3.id, val);
        break;
    case DEVICE_4_PCF_GPIO:
        app_driver_set_state(deviceList.device4.id, val);
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device4, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(val)
        );
        app_homekit_update_state(deviceList.device4.id, val);
        break;
    case DEVICE_5_PCF_GPIO:
        app_driver_set_state(deviceList.device5.id, val);
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device5, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(val)
        );
        app_homekit_update_state(deviceList.device5.id, val);
        break;
    case DEVICE_6_PCF_GPIO:
        app_driver_set_state(deviceList.device6.id, val);
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device6, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(val)
        );
        app_homekit_update_state(deviceList.device6.id, val);
        break;
    case DEVICE_7_PCF_GPIO:
        app_driver_set_state(deviceList.device7.id, val);
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device7, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(val)
        );
        app_homekit_update_state(deviceList.device7.id, val);
        break;
    case DEVICE_8_PCF_GPIO:
        app_driver_set_state(deviceList.device8.id, val);
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(device8, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(val)
        );
        app_homekit_update_state(deviceList.device8.id, val);
        break;
    default:
        break;
    }
    // printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());
}

/* Callback to handle commands received from the RainMaker cloud */
static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
            const esp_rmaker_param_val_t val, void *priv_data, esp_rmaker_write_ctx_t *ctx)
{
    if (ctx) {
        ESP_LOGI(TAG, "Received write request via : %s", esp_rmaker_device_cb_src_to_str(ctx->src));
    }
    if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0) {
        ESP_LOGI(TAG, "Received value = %s for %s - %s",
                val.val.b? "true" : "false", esp_rmaker_device_get_name(device),
                esp_rmaker_param_get_name(param));
        // Check which device the command is for
        if (device == device1) {
            app_driver_set_state(deviceList.device1.id, val.val.b);
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(deviceList.device1.id, val.val.b);
        } else if (device == device2) {
            app_driver_set_state(deviceList.device2.id, val.val.b);
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(deviceList.device2.id, val.val.b);
        } else if (device == device3) {
            app_driver_set_state(deviceList.device3.id, val.val.b);
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(deviceList.device3.id, val.val.b);
        } else if (device == device4) {
            app_driver_set_state(deviceList.device4.id, val.val.b);
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(deviceList.device4.id, val.val.b);
        } else if (device == device5) {
            app_driver_set_state(deviceList.device5.id, val.val.b);
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(deviceList.device5.id, val.val.b);
        } else if (device == device6) {
            app_driver_set_state(deviceList.device6.id, val.val.b);
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(deviceList.device6.id, val.val.b);
        } else if (device == device7) {
            app_driver_set_state(deviceList.device7.id, val.val.b);
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(deviceList.device7.id, val.val.b);
        } else if (device == device8) {
            app_driver_set_state(deviceList.device8.id, val.val.b);
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(deviceList.device8.id, val.val.b);
        }
    } else if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_BRIGHTNESS_NAME) == 0) {
        if (device == temperatureDevice) {
            printf("Brightness = %d\n", val.val.i);
            brightness = val.val.i;
            esp_rmaker_param_update(param, val);
            app_homekit_update_brightness_state(DEVICE_TEMPERATURE_BRIGHTNESS, val.val.i);
            setDisplayData();
        }
    } else if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_LCD_MODE_NAME) == 0) {
        if (device == temperatureDevice) {
            printf("LCD Mode = %d\n", val.val.i);
            displayMode = val.val.i;
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(DEVICE_TEMPERATURE_MODE, val.val.i);
            setDisplayData();
        }
    }
    return ESP_OK;
}

/* Event handler for catching RainMaker events */
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_base == RMAKER_EVENT) {
        switch (event_id) {
            case RMAKER_EVENT_INIT_DONE:
                ESP_LOGI(TAG, "RainMaker Initialised.");
                break;
            case RMAKER_EVENT_CLAIM_STARTED:
                ESP_LOGI(TAG, "RainMaker Claim Started.");
                break;
            case RMAKER_EVENT_CLAIM_SUCCESSFUL:
                ESP_LOGI(TAG, "RainMaker Claim Successful.");
                break;
            case RMAKER_EVENT_CLAIM_FAILED:
                ESP_LOGI(TAG, "RainMaker Claim Failed.");
                break;
            default:
                ESP_LOGW(TAG, "Unhandled RainMaker Event: %"PRIi32, event_id);
        }
    } else if (event_base == RMAKER_COMMON_EVENT) {
        switch (event_id) {
            case RMAKER_EVENT_REBOOT:
                ESP_LOGI(TAG, "Rebooting in %d seconds.", *((uint8_t *)event_data));
                break;
            case RMAKER_EVENT_WIFI_RESET:
                ESP_LOGI(TAG, "Wi-Fi credentials reset.");
                break;
            case RMAKER_EVENT_FACTORY_RESET:
                ESP_LOGI(TAG, "Node reset to factory defaults.");
                break;
            case RMAKER_MQTT_EVENT_CONNECTED:
                ESP_LOGI(TAG, "MQTT Connected.");
                break;
            case RMAKER_MQTT_EVENT_DISCONNECTED:
                ESP_LOGI(TAG, "MQTT Disconnected.");
                break;
            case RMAKER_MQTT_EVENT_PUBLISHED:
                ESP_LOGI(TAG, "MQTT Published. Msg id: %d.", *((int *)event_data));
                break;
            default:
                ESP_LOGW(TAG, "Unhandled RainMaker Common Event: %"PRIi32, event_id);
        }
    } else if (event_base == APP_WIFI_EVENT) {
        switch (event_id) {
            case APP_WIFI_EVENT_QR_DISPLAY:
                ESP_LOGI(TAG, "Provisioning QR : %s", (char *)event_data);
                break;
            case APP_WIFI_EVENT_PROV_TIMEOUT:
                ESP_LOGI(TAG, "Provisioning Timed Out. Please reboot.");
                break;
            case APP_WIFI_EVENT_PROV_RESTART:
                ESP_LOGI(TAG, "Provisioning has restarted due to failures.");
                break;
            default:
                ESP_LOGW(TAG, "Unhandled App Wi-Fi Event: %"PRIi32, event_id);
                break;
        }
    } else if (event_base == RMAKER_OTA_EVENT) {
        switch(event_id) {
            case RMAKER_OTA_EVENT_STARTING:
                ESP_LOGI(TAG, "Starting OTA.");
                break;
            case RMAKER_OTA_EVENT_IN_PROGRESS:
                ESP_LOGI(TAG, "OTA is in progress.");
                break;
            case RMAKER_OTA_EVENT_SUCCESSFUL:
                ESP_LOGI(TAG, "OTA successful.");
                break;
            case RMAKER_OTA_EVENT_FAILED:
                ESP_LOGI(TAG, "OTA Failed.");
                break;
            case RMAKER_OTA_EVENT_REJECTED:
                ESP_LOGI(TAG, "OTA Rejected.");
                break;
            case RMAKER_OTA_EVENT_DELAYED:
                ESP_LOGI(TAG, "OTA Delayed.");
                break;
            case RMAKER_OTA_EVENT_REQ_FOR_REBOOT:
                ESP_LOGI(TAG, "Firmware image downloaded. Please reboot your device to apply the upgrade.");
                break;
            default:
                ESP_LOGW(TAG, "Unhandled OTA Event: %"PRIi32, event_id);
                break;
        }
    } else {
        ESP_LOGW(TAG, "Invalid event received!");
    }
}

void my_task1(void *pvParameters) {
    while (1) {
        aht10_get_temp_humidity(&temperature, &humidity);
        if(node) {
            esp_rmaker_param_update_and_report(
                temperature_param,
                esp_rmaker_float(temperature));
            esp_rmaker_param_update_and_report(
                humidity_param,
                esp_rmaker_float(humidity));
            app_homekit_update_temperature_state(DEVICE_TEMPERATURE, temperature);
            app_homekit_update_temperature_state(DEVICE_HUMIDITY, humidity);
        }
        setDisplayData();
        vTaskDelay(pdMS_TO_TICKS(60000)); // Updates after 60 seconds
    }
}

void check_inputs(uint8_t *last_state) {
    uint8_t current_state;
    pcf8574_read(&current_state);  // Read from PCF8574

    // printf("Current State: 0x%02X, Last State: 0x%02X\n", current_state, *last_state);

    uint8_t changed_pins = current_state ^ *last_state; // XOR to detect changes

    if (changed_pins == 0) {
        return;  // No changes, exit function
    }

    for (int i = 0; i < 8; i++) {
        if (changed_pins & (1 << i)) { // Check which bit changed
            // printf("GPIO %d changed to %d\n", i, (current_state >> i) & 1);
            gpio_input_task(i, !((current_state >> i) & 1));
            // Perform operation here based on GPIO state
        }
    }

    *last_state = current_state; // Update last known state
}

void my_task2(void *pvParameters) {
    uint8_t last_state;
    pcf8574_read(&last_state);  // Read initial state
    printf("Initial State: 0x%02X\n", last_state);

    while (1) {
        check_inputs(&last_state);  // Check for changes
        vTaskDelay(pdMS_TO_TICKS(100));  // Reduced delay to 500ms for better response
    }
}

void app_main()
{
    /* Initialize Application specific hardware drivers and
     * set initial state.
     */
    esp_rmaker_console_init();

    /* Initialize I2C. */
    i2c_master_init();

    lcd = tm1637_init(LCD_CLK, LCD_DTA);

    isAHT10Connected = check_i2c_device(AHT10_ADDR);
    isPCF8574Connected = check_i2c_device(PCF8574_ADDR);

    app_driver_init();
    
    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    /* Initialize Wi-Fi. Note that, this should be called before esp_rmaker_node_init()
     */
    app_wifi_with_homekit_init();

    /* Register an event handler to catch RainMaker events */
    ESP_ERROR_CHECK(esp_event_handler_register(RMAKER_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(RMAKER_COMMON_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(APP_WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(RMAKER_OTA_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    /* Initialize the ESP RainMaker Agent.
     * Note that this should be called after app_wifi_with_homekit_init() but before app_wifi_with_homekit_start()
     * */
    esp_rmaker_config_t rainmaker_cfg = {
        .enable_time_sync = true,
    };

    node = esp_rmaker_node_init(&rainmaker_cfg, MANUFACTURER, "Main Hall");
    if (!node) {
        ESP_LOGE(TAG, "Could not initialise node. Aborting!!!");
        vTaskDelay(5000/portTICK_PERIOD_MS);
        abort();
    }

    /* Create a Switch device.
     * You can optionally use the helper API esp_rmaker_switch_device_create() to
     * avoid writing code for adding the name and power parameters.
     */
    device1 = esp_rmaker_device_create(deviceList.device1.name, ESP_RMAKER_DEVICE_SOCKET, NULL);
    device2 = esp_rmaker_device_create(deviceList.device2.name, ESP_RMAKER_DEVICE_SOCKET, NULL);
    device3 = esp_rmaker_device_create(deviceList.device3.name, ESP_RMAKER_DEVICE_LIGHT, NULL);
    device4 = esp_rmaker_device_create(deviceList.device4.name, ESP_RMAKER_DEVICE_SOCKET, NULL);
    device5 = esp_rmaker_device_create(deviceList.device5.name, ESP_RMAKER_DEVICE_LIGHTBULB, NULL);
    device6 = esp_rmaker_device_create(deviceList.device6.name, ESP_RMAKER_DEVICE_LIGHT, NULL);
    device7 = esp_rmaker_device_create(deviceList.device7.name, ESP_RMAKER_DEVICE_LIGHT, NULL);
    device8 = esp_rmaker_device_create(deviceList.device8.name, ESP_RMAKER_DEVICE_LIGHT, NULL);
    temperatureDevice = esp_rmaker_device_create(deviceList.temperatureDevice.name, ESP_RMAKER_DEVICE_TEMP_SENSOR, NULL);
    humidityDevice = esp_rmaker_device_create(deviceList.humidityDevice.name, ESP_RMAKER_DEVICE_HUMIDITY_SENSOR, NULL);

    /* Add the write callback for the device. We aren't registering any read callback yet as
     * it is for future use.
     */
    esp_rmaker_device_add_cb(device1, write_cb, NULL);
    esp_rmaker_device_add_cb(device2, write_cb, NULL);
    esp_rmaker_device_add_cb(device3, write_cb, NULL);
    esp_rmaker_device_add_cb(device4, write_cb, NULL);
    esp_rmaker_device_add_cb(device5, write_cb, NULL);
    esp_rmaker_device_add_cb(device6, write_cb, NULL);
    esp_rmaker_device_add_cb(device7, write_cb, NULL);
    esp_rmaker_device_add_cb(device8, write_cb, NULL);
    esp_rmaker_device_add_cb(temperatureDevice, write_cb, NULL);

    /* Add the standard name parameter (type: esp.param.name), which allows setting a persistent,
     * user friendly custom name from the phone apps. All devices are recommended to have this
     * parameter.
     */
    esp_rmaker_device_add_param(device1, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.device1.name));
    esp_rmaker_device_add_param(device2, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.device2.name));
    esp_rmaker_device_add_param(device3, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.device3.name));
    esp_rmaker_device_add_param(device4, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.device4.name));
    esp_rmaker_device_add_param(device5, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.device5.name));
    esp_rmaker_device_add_param(device6, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.device6.name));
    esp_rmaker_device_add_param(device7, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.device7.name));
    esp_rmaker_device_add_param(device8, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.device8.name));
    esp_rmaker_device_add_param(temperatureDevice, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.temperatureDevice.name));
    esp_rmaker_device_add_param(humidityDevice, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, deviceList.humidityDevice.name));

    /* Add the standard power parameter (type: esp.param.power), which adds a boolean param
     * with a toggle switch ui-type.
     */
    power_param1 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME,app_driver_get_state(deviceList.device1.id));
    power_param2 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME,app_driver_get_state(deviceList.device2.id));
    power_param3 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME,app_driver_get_state(deviceList.device3.id));
    power_param4 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME,app_driver_get_state(deviceList.device4.id));
    power_param5 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME,app_driver_get_state(deviceList.device5.id));
    power_param6 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME,app_driver_get_state(deviceList.device6.id));
    power_param7 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME,app_driver_get_state(deviceList.device7.id));
    power_param8 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME,app_driver_get_state(deviceList.device8.id));
    temperature_param = esp_rmaker_param_create(ESP_RMAKER_DEF_TEMPERATURE_NAME, ESP_RMAKER_PARAM_TEMPERATURE, esp_rmaker_float(0), PROP_FLAG_READ);
    temperature_brightness_param = esp_rmaker_lcd_brightness_param_create(ESP_RMAKER_DEF_BRIGHTNESS_NAME, 0);
    temperature_display_param = esp_rmaker_lcd_mode_param_create(ESP_RMAKER_DEF_LCD_MODE_NAME, 0);
    humidity_param = esp_rmaker_param_create(ESP_RMAKER_DEF_HUMIDITY_NAME, ESP_RMAKER_PARAM_HUMIDITY, esp_rmaker_float(0), PROP_FLAG_READ);

    esp_rmaker_device_add_param(device1, power_param1);
    esp_rmaker_device_add_param(device2, power_param2);
    esp_rmaker_device_add_param(device3, power_param3);
    esp_rmaker_device_add_param(device4, power_param4);
    esp_rmaker_device_add_param(device5, power_param5);
    esp_rmaker_device_add_param(device6, power_param6);
    esp_rmaker_device_add_param(device7, power_param7);
    esp_rmaker_device_add_param(device8, power_param8);
    esp_rmaker_device_add_param(temperatureDevice, temperature_param);
    esp_rmaker_device_add_param(temperatureDevice, temperature_brightness_param);
    esp_rmaker_device_add_param(temperatureDevice, temperature_display_param);
    esp_rmaker_device_add_param(humidityDevice, humidity_param);

    /* Assign the power parameter as the primary, so that it can be controlled from the
     * home screen of the phone apps.
     */
    esp_rmaker_device_assign_primary_param(device1, power_param1);
    esp_rmaker_device_assign_primary_param(device2, power_param2);
    esp_rmaker_device_assign_primary_param(device3, power_param3);
    esp_rmaker_device_assign_primary_param(device4, power_param4);
    esp_rmaker_device_assign_primary_param(device5, power_param5);
    esp_rmaker_device_assign_primary_param(device6, power_param6);
    esp_rmaker_device_assign_primary_param(device7, power_param7);
    esp_rmaker_device_assign_primary_param(device8, power_param8);
    esp_rmaker_device_assign_primary_param(temperatureDevice, temperature_param);
    esp_rmaker_device_assign_primary_param(humidityDevice, humidity_param);

    /* Add this switch device to the node */
    esp_rmaker_node_add_device(node, device1);
    esp_rmaker_node_add_device(node, device2);
    esp_rmaker_node_add_device(node, device3);
    esp_rmaker_node_add_device(node, device4);
    esp_rmaker_node_add_device(node, device5);
    esp_rmaker_node_add_device(node, device6);
    esp_rmaker_node_add_device(node, device7);
    esp_rmaker_node_add_device(node, device8);

    if (isAHT10Connected) {
        esp_rmaker_node_add_device(node, temperatureDevice);
        esp_rmaker_node_add_device(node, humidityDevice);
    }

    /* Enable OTA */
    esp_rmaker_ota_enable_default();

    /* Enable timezone service which will be require for setting appropriate timezone
     * from the phone apps for scheduling to work correctly.
     * For more information on the various ways of setting timezone, please check
     * https://rainmaker.espressif.com/docs/time-service.html.
     */
    esp_rmaker_timezone_service_enable();

    /* Enable scheduling. */
    esp_rmaker_schedule_enable();

    /* Enable Scenes */
    esp_rmaker_scenes_enable();

    /* Enable Insights. Requires CONFIG_ESP_INSIGHTS_ENABLED=y */
    app_insights_enable();

    /* Start the ESP RainMaker Agent */
    esp_rmaker_start();

    /* Start the HomeKit module */
    app_homekit_start();

    /* Start the Wi-Fi.
     * If the node is provisioned, it will start connection attempts,
     * else, it will start Wi-Fi provisioning. The function will return
     * after a connection has been successfully established
     */
    err = app_wifi_with_homekit_start(POP_TYPE_RANDOM);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Could not start Wifi. Aborting!!!");
        vTaskDelay(5000/portTICK_PERIOD_MS);
        abort();
    }

    if (isAHT10Connected) {
        aht10_init();
        xTaskCreate(my_task1, "DelayedTask1", 4096, NULL, 1, NULL);
    }
    if (isPCF8574Connected) {
        xTaskCreate(my_task2, "DelayedTask2", 2048, NULL, 1, NULL);
    }
}
