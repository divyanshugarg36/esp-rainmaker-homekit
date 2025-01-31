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

#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_ota.h>
#include <esp_rmaker_console.h>
#include <esp_rmaker_common_events.h>

#include <app_insights.h>

#include "app_wifi_with_homekit.h"
#include "app_priv.h"

#include "../components/rmaker_custom/custom_params.h"

static const char *TAG = "app_main";

esp_rmaker_device_t *device1;
esp_rmaker_device_t *device2;
esp_rmaker_device_t *device3;
esp_rmaker_device_t *device4;
esp_rmaker_device_t *device5;
esp_rmaker_device_t *device6;
esp_rmaker_device_t *device7;


typedef struct {
    char *name;
    int gpioNumber;
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

Devices deviceList = {
    .device1 = {
        .name = "Main Light",
        .gpioNumber = 1
    },
    .device2 = {
        .name = "Small Light 1",
        .gpioNumber = 1
    },
    .device3 = {
        .name = "Small Light 2",
        .gpioNumber = 1
    },
    .device4 = {
        .name = "Fan 1",
        .gpioNumber = 1
    },
    .device5 = {
        .name = "Fan 2",
        .gpioNumber = 1
    },
    .device6 = {
        .name = "Socket",
        .gpioNumber = 1
    },
    .device7 = {
        .name = "Ceiling Light",
        .gpioNumber = 1
    }
};

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
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update(param, val);
            app_homekit_update_state(val.val.b);
        } else if (device == device2) {
            app_driver_set_state2(val.val.b);
            esp_rmaker_param_update(param, val);
        }
         else if (device == device3) {
            app_driver_set_state3(val.val.b);
            esp_rmaker_param_update(param, val);
        } else if (device == device4) {
            app_driver_set_state4(val.val.b);
            esp_rmaker_param_update(param, val);
        } else if (device == device5) {
            app_driver_set_state5(val.val.b);
            esp_rmaker_param_update(param, val);
        } else if (device == device6) {
            app_driver_set_state6(val.val.b);
            esp_rmaker_param_update(param, val);
        } else if (device == device7) {
            app_driver_set_state7(val.val.b);
            esp_rmaker_param_update(param, val);
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

void app_main()
{
    /* Initialize Application specific hardware drivers and
     * set initial state.
     */
    esp_rmaker_console_init();
    app_driver_init();
    app_driver_set_state(DEFAULT_POWER);

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
        .enable_time_sync = false,
    };
    esp_rmaker_node_t *node = esp_rmaker_node_init(&rainmaker_cfg, "Garg's Home", "Main Hall");
    if (!node) {
        ESP_LOGE(TAG, "Could not initialise node. Aborting!!!");
        vTaskDelay(5000/portTICK_PERIOD_MS);
        abort();
    }

    /* Create a Switch device.
     * You can optionally use the helper API esp_rmaker_switch_device_create() to
     * avoid writing code for adding the name and power parameters.
     */
    device1 = esp_rmaker_device_create(deviceList.device1.name, ESP_RMAKER_DEVICE_LIGHT, NULL);
    device2 = esp_rmaker_device_create(deviceList.device2.name, ESP_RMAKER_DEVICE_LIGHTBULB, NULL);
    device3 = esp_rmaker_device_create(deviceList.device3.name, ESP_RMAKER_DEVICE_LIGHTBULB, NULL);
    device4 = esp_rmaker_device_create(deviceList.device4.name, ESP_RMAKER_DEVICE_FAN, NULL);
    device5 = esp_rmaker_device_create(deviceList.device5.name, ESP_RMAKER_DEVICE_FAN, NULL);
    device6 = esp_rmaker_device_create(deviceList.device6.name, ESP_RMAKER_DEVICE_SOCKET, NULL);
    device7 = esp_rmaker_device_create(deviceList.device7.name, ESP_RMAKER_DEVICE_LIGHT, NULL);

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

    /* Add the standard power parameter (type: esp.param.power), which adds a boolean param
     * with a toggle switch ui-type.
     */
    esp_rmaker_param_t *power_param1 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME, DEFAULT_POWER);
    esp_rmaker_param_t *power_param2 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME, DEFAULT_POWER);
    esp_rmaker_param_t *power_param3 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME, DEFAULT_POWER);
    esp_rmaker_param_t *power_param4 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME, DEFAULT_POWER);
    esp_rmaker_param_t *power_param4_1 = esp_rmaker_custom_speed_param_create(ESP_RMAKER_DEF_SPEED_NAME, DEFAULT_POWER);
    esp_rmaker_param_t *power_param5 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME, DEFAULT_POWER);
    esp_rmaker_param_t *power_param5_1 = esp_rmaker_custom_speed_param_create(ESP_RMAKER_DEF_SPEED_NAME, DEFAULT_POWER);
    esp_rmaker_param_t *power_param6 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME, DEFAULT_POWER);
    esp_rmaker_param_t *power_param7 = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME, DEFAULT_POWER);
    
    esp_rmaker_device_add_param(device1, power_param1);
    esp_rmaker_device_add_param(device2, power_param2);
    esp_rmaker_device_add_param(device3, power_param3);
    esp_rmaker_device_add_param(device4, power_param4);
    esp_rmaker_device_add_param(device4, power_param4_1);
    esp_rmaker_device_add_param(device5, power_param5);
    esp_rmaker_device_add_param(device5, power_param5_1);
    esp_rmaker_device_add_param(device6, power_param6);
    esp_rmaker_device_add_param(device7, power_param7);

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

    /* Add this switch device to the node */
    esp_rmaker_node_add_device(node, device1);
    esp_rmaker_node_add_device(node, device2);
    esp_rmaker_node_add_device(node, device3);
    esp_rmaker_node_add_device(node, device4);
    esp_rmaker_node_add_device(node, device5);
    esp_rmaker_node_add_device(node, device6);
    esp_rmaker_node_add_device(node, device7);

    /* Enable OTA */
    esp_rmaker_ota_enable_default();

    /* Enable Insights. Requires CONFIG_ESP_INSIGHTS_ENABLED=y */
    app_insights_enable();

    /* Start the ESP RainMaker Agent */
    esp_rmaker_start();

    /* Start the HomeKit module */
    app_homekit_start(DEFAULT_POWER);

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
}
