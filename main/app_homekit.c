/* HomeKit Switch control
   
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h>

#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>

#include "homekit_custom.h"
#include "rmaker_custom_params.h"

#include <qrcode.h>

#include "app_priv.h"

static const char *TAG = "app_homekit";

static hap_char_t *device1_char;
static hap_char_t *device2_char;
static hap_char_t *device3_char;
static hap_char_t *device4_char;
static hap_char_t *device5_char;
static hap_char_t *device6_char;

static hap_char_t *temperatureDevice_char;
static hap_char_t *temperatureDevice_display_brightness_char;
static hap_char_t *temperatureDevice_display_brightness_power_char;
static hap_char_t *temperatureDevice_display_mode_char;

static hap_char_t *humidityDevice_char;

char *display_brightness = "Brightness";
char *display_mode = "Display Mode";

#define QRCODE_BASE_URL  "https://espressif.github.io/esp-homekit-sdk/qrcode.html"

static void app_homekit_show_qr(void) {
    #ifdef CONFIG_EXAMPLE_USE_HARDCODED_SETUP_CODE
        char *setup_payload =  esp_hap_get_setup_payload(CONFIG_EXAMPLE_SETUP_CODE,
                CONFIG_EXAMPLE_SETUP_ID, false, HAP_CID_SWITCH);
        if (setup_payload) {
            printf("-----QR Code for HomeKit-----\n");
            printf("Scan this QR code from the Home app on iOS\n");
            qrcode_display(setup_payload);
            printf("If QR code is not visible, copy paste the below URL in a browser.\n%s?data=%s\n",
                    QRCODE_BASE_URL, setup_payload);
            free(setup_payload);
        }
    #else
        ESP_LOGW(TAG, "Cannot show QR code for HomeKit pairing as the raw setup code is not available.");
    #endif
}

/* Mandatory identify routine for the accessory (bridge)
 * In a real accessory, something like LED blink should be implemented
 * got visual identification
 */
static int bridge_identify(hap_acc_t *ha)
{
    ESP_LOGI(TAG, "Bridge identified");
    return HAP_SUCCESS;
}

/* Mandatory identify routine for the bridged accessory
 * In a real bridge, the actual accessory must be sent some request to
 * identify itself visually
 */
static int accessory_identify(hap_acc_t *ha) {
    hap_serv_t *hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_ACCESSORY_INFORMATION);
    hap_char_t *hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_NAME);
    const hap_val_t *val = hap_char_get_val(hc);
    char *name = val->s;

    ESP_LOGI(TAG, "Bridged Accessory %s identified", name);
    return HAP_SUCCESS;
}

static void app_homekit_event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data) {
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        if (hap_get_paired_controller_count() == 0) {
            app_homekit_show_qr();
        } else {
            ESP_LOGI(TAG, "Accessory is already paired with a controller.");
        }
    } else if (event_base == HAP_EVENT && event_id == HAP_EVENT_CTRL_UNPAIRED) {
        if (hap_get_paired_controller_count() == 0) {
            app_homekit_show_qr();
        }
    }
}

static int IRAM_ATTR homekit_write(hap_write_data_t write_data[], int count,
        void *serv_priv, void *write_priv)
{
    int i, ret = HAP_SUCCESS;
    hap_write_data_t *write;
    char *deviceName = (char *)serv_priv;
    printf("Write called for Accessory %s\n", deviceName);
    if (strcmp(deviceName, deviceList.device1.name) == 0) {
        for (i = 0; i < count; i++) {
            write = &write_data[i];
            if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
                printf("%s Status -> %s\n", deviceName, write->val.b ? "On" : "Off");
                /* Set the switch state */
                app_driver_set_state(deviceList.device1.id, write->val.b);
                /* Update the HomeKit characteristic */
                hap_char_update_val(write->hc, &(write->val));
                /* Report to RainMaker */
                esp_rmaker_param_update_and_report(
                    esp_rmaker_device_get_param_by_name(device1, ESP_RMAKER_DEF_POWER_NAME),
                    esp_rmaker_bool(write->val.b));

                *(write->status) = HAP_STATUS_SUCCESS;
            } else {
                *(write->status) = HAP_STATUS_RES_ABSENT;
            }
        }
    } else if (strcmp(deviceName, deviceList.device2.name) == 0) {
        for (i = 0; i < count; i++) {
            write = &write_data[i];
            if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
                printf("%s Status -> %s\n", deviceName, write->val.b ? "On" : "Off");
                /* Set the switch state */
                app_driver_set_state(deviceList.device2.id, write->val.b);
                /* Update the HomeKit characteristic */
                hap_char_update_val(write->hc, &(write->val));
                /* Report to RainMaker */
                esp_rmaker_param_update_and_report(
                    esp_rmaker_device_get_param_by_name(device2, ESP_RMAKER_DEF_POWER_NAME),
                    esp_rmaker_bool(write->val.b));

                *(write->status) = HAP_STATUS_SUCCESS;
            } else {
                *(write->status) = HAP_STATUS_RES_ABSENT;
            }
        }
    } else if (strcmp(deviceName, deviceList.device3.name) == 0) {
        for (i = 0; i < count; i++) {
            write = &write_data[i];
            if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
                printf("%s Status -> %s\n", deviceName, write->val.b ? "On" : "Off");
                /* Set the switch state */
                app_driver_set_state(deviceList.device3.id, write->val.b);
                /* Update the HomeKit characteristic */
                hap_char_update_val(write->hc, &(write->val));
                /* Report to RainMaker */
                esp_rmaker_param_update_and_report(
                    esp_rmaker_device_get_param_by_name(device3, ESP_RMAKER_DEF_POWER_NAME),
                    esp_rmaker_bool(write->val.b));
                *(write->status) = HAP_STATUS_SUCCESS;
            } else {
                *(write->status) = HAP_STATUS_RES_ABSENT;
            }
        }
    } else if (strcmp(deviceName, deviceList.device4.name) == 0) {
        for (i = 0; i < count; i++) {
            write = &write_data[i];
            if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
                printf("%s Status -> %s\n", deviceName, write->val.b ? "On" : "Off");
                /* Set the switch state */   
                app_driver_set_state(deviceList.device4.id, write->val.b);
                /* Update the HomeKit characteristic */
                hap_char_update_val(write->hc, &(write->val));
                /* Report to RainMaker */
                esp_rmaker_param_update_and_report(
                    esp_rmaker_device_get_param_by_name(device4, ESP_RMAKER_DEF_POWER_NAME),
                    esp_rmaker_bool(write->val.b));
                *(write->status) = HAP_STATUS_SUCCESS;
            } else {
                *(write->status) = HAP_STATUS_RES_ABSENT;
            }
        } 
    } else if (strcmp(deviceName, deviceList.device5.name) == 0) {
        for (i = 0; i < count; i++) {
            write = &write_data[i];
            if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
                printf("%s Status -> %s\n", deviceName, write->val.b ? "On" : "Off");
                /* Set the switch state */
                app_driver_set_state(deviceList.device5.id, write->val.b);
                /* Update the HomeKit characteristic */
                hap_char_update_val(write->hc, &(write->val));
                /* Report to RainMaker */
                esp_rmaker_param_update_and_report(
                    esp_rmaker_device_get_param_by_name(device5, ESP_RMAKER_DEF_POWER_NAME),
                    esp_rmaker_bool(write->val.b));
                *(write->status) = HAP_STATUS_SUCCESS;
            } else {
                *(write->status) = HAP_STATUS_RES_ABSENT;
            }
        }
    } else if (strcmp(deviceName, deviceList.device6.name) == 0) {
        for (i = 0; i < count; i++) {
            write = &write_data[i];
            if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
                printf("%s Status -> %s\n", deviceName, write->val.b ? "On" : "Off");
                /* Set the switch state */
                app_driver_set_state(deviceList.device6.id, write->val.b);
                /* Update the HomeKit characteristic */
                hap_char_update_val(write->hc, &(write->val));
                /* Report to RainMaker */
                esp_rmaker_param_update_and_report(
                    esp_rmaker_device_get_param_by_name(device6, ESP_RMAKER_DEF_POWER_NAME),
                    esp_rmaker_bool(write->val.b));
                *(write->status) = HAP_STATUS_SUCCESS;
            } else {
                *(write->status) = HAP_STATUS_RES_ABSENT;
            }
        }
    } else if (strcmp(deviceName, display_brightness) == 0) {
        for (i = 0; i < count; i++) {
            write = &write_data[i];
            if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
                printf("%s Status -> %s\n", deviceName, write->val.b ? "On" : "Off");
                if (write->val.b) {
                    // Read Existing Brightness value
                    const hap_val_t *brightnessVal = hap_char_get_val(temperatureDevice_display_brightness_char);
                    brightness = percent_to_display(brightnessVal->i); 
                } else {
                    brightness = 0; 
                }
                setDisplayData();
                /* Update the HomeKit characteristic */
                hap_char_update_val(write->hc, &(write->val));
                esp_rmaker_param_update_and_report(
                        esp_rmaker_device_get_param_by_name(temperatureDevice, ESP_RMAKER_DEF_BRIGHTNESS_NAME),
                        esp_rmaker_int(brightness));
                *(write->status) = HAP_STATUS_SUCCESS;
            } else if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_BRIGHTNESS)) {
                printf("Received Write for Light Brightness %d", write->val.i);
                int newBrightness = percent_to_display(write->val.i);
                if (brightness != newBrightness) {
                    brightness = newBrightness;
                    setDisplayData();
                    hap_char_update_val(write->hc, &(write->val)); // IMPORTANT
                    esp_rmaker_param_update_and_report(
                        esp_rmaker_device_get_param_by_name(temperatureDevice, ESP_RMAKER_DEF_BRIGHTNESS_NAME),
                        esp_rmaker_int(newBrightness));
                }
                *(write->status) = HAP_STATUS_SUCCESS;
            } else {
                *(write->status) = HAP_STATUS_RES_ABSENT;
            }
        }
    } else if (strcmp(deviceName, display_mode) == 0) {
        for (i = 0; i < count; i++) {
            write = &write_data[i];
            if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
                printf("%s Status -> %s\n", deviceName, write->val.b ? "On" : "Off");
                displayMode = write->val.b ? 1 : 0;
                setDisplayData();
                hap_char_update_val(write->hc, &(write->val)); // IMPORTANT
                esp_rmaker_param_update_and_report(
                        esp_rmaker_device_get_param_by_name(temperatureDevice, ESP_RMAKER_DEF_LCD_MODE_NAME),
                        esp_rmaker_int(write->val.b));
                *(write->status) = HAP_STATUS_SUCCESS;
            } else {
                *(write->status) = HAP_STATUS_RES_ABSENT;
            }
        }
    }
    return ret;
}

esp_err_t app_homekit_update_state(int deviceId, bool state)
{
    hap_val_t new_value = {
        .b = state,
    };
    switch (deviceId) {
    case DEVICE_1_ID:
        hap_char_update_val(device1_char, &new_value);
        break;
    case DEVICE_2_ID:
        hap_char_update_val(device2_char, &new_value);
        break;
    case DEVICE_3_ID:
        hap_char_update_val(device3_char, &new_value);
        break;
    case DEVICE_4_ID:
        hap_char_update_val(device4_char, &new_value);
        break;
    case DEVICE_5_ID:
        hap_char_update_val(device5_char, &new_value);
        break;
    case DEVICE_6_ID:
        hap_char_update_val(device6_char, &new_value);
        break;
    case DEVICE_TEMPERATURE_MODE:
        hap_char_update_val(temperatureDevice_display_mode_char, &new_value);
        break;
    default:
        return ESP_FAIL;
        break;
    }
        return ESP_OK;
}


esp_err_t app_homekit_update_temperature_state(int deviceId, float state)
{
    hap_val_t new_value = {
        .f = state,
    };
    switch (deviceId) {
    case DEVICE_TEMPERATURE:
        hap_char_update_val(temperatureDevice_char, &new_value);
        break;
    case DEVICE_HUMIDITY:
        hap_char_update_val(humidityDevice_char, &new_value);
        break;
    default:
        return ESP_FAIL;
        break;
    }
        return ESP_OK;
}

esp_err_t app_homekit_update_brightness_state(int deviceId, int state)
{
    hap_val_t new_value = {
        .i = display_to_percent(state),
    };
    switch (deviceId) {
    case DEVICE_TEMPERATURE_BRIGHTNESS:
        hap_val_t on_value = {
            .b = state == 0 ? false : true,
        };
        hap_char_update_val(temperatureDevice_display_brightness_power_char, &on_value);
        hap_char_update_val(temperatureDevice_display_brightness_char, &new_value);
        break;
    default:
        return ESP_FAIL;
        break;
    }
        return ESP_OK;
}

esp_err_t app_homekit_start()
{
    hap_acc_t *accessory;
    hap_serv_t *service;

    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);

    /* MAIN ACCESSORY
     * Initialise the mandatory parameters for Accessory which will be added as
     * the mandatory services internally
     */
    hap_acc_cfg_t cfg = {
        .name = "Garg HomeKit",
        .manufacturer = MANUFACTURER,
        .model = MODEL,
        .serial_num = SERIAL_NUMBER,
        .fw_rev = FIRMWARE_REVISION,
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = bridge_identify,
        .cid = HAP_CID_BRIDGE,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&cfg);

    /* Add a dummy Product Data */
    uint8_t product_data[] = {'E','S','P','3','2','H','A','P'};
    hap_acc_add_product_data(accessory, product_data, sizeof(product_data));
    /* Add Wi-Fi Transport service required for HAP Spec R16 */
    hap_acc_add_wifi_transport_service(accessory, 0);
    /* Add the Accessory to the HomeKit Database */
    hap_add_accessory(accessory);


    hap_acc_cfg_t device1_cfg = {
        .name = deviceList.device1.name,
        .manufacturer = MANUFACTURER,
        .model = MODEL,
        .serial_num = SERIAL_NUMBER,
        .fw_rev = FIRMWARE_REVISION,
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = accessory_identify,
        .cid = HAP_CID_BRIDGE,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&device1_cfg);
    /* Create the Outlet Service. Include the "name" since this is a user visible service  */
    service = hap_serv_switch_create(app_driver_get_state(deviceList.device1.id));
    hap_serv_add_char(service, hap_char_name_create(deviceList.device1.name));
    /* Set the Accessory name as the Private data for the service,
     * so that the correct accessory can be identified in the
     * write callback
     */
    hap_serv_set_priv(service, strdup(deviceList.device1.name));
    /* Set the write callback for the service */
    hap_serv_set_write_cb(service, homekit_write);
    /* Get pointer to the on_char to be used during update */
    device1_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON);
    /* Add the Outlet Service to the Accessory Object */
    hap_acc_add_serv(accessory, service);
    /* Add the Accessory to the HomeKit Database */
    hap_add_bridged_accessory(accessory, hap_get_unique_aid(deviceList.device1.name));


    /*
     * Bridge Accessory
     */ 
    hap_acc_cfg_t device2_cfg = {
        .name = deviceList.device2.name,
        .manufacturer = MANUFACTURER,
        .model = MODEL,
        .serial_num = SERIAL_NUMBER,
        .fw_rev = FIRMWARE_REVISION,
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = accessory_identify,
        .cid = HAP_CID_BRIDGE,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&device2_cfg);
    /* Create the Fan Service. Include the "name" since this is a user visible service  */
    service = hap_serv_switch_create(app_driver_get_state(deviceList.device2.id));
    hap_serv_add_char(service, hap_char_name_create(deviceList.device2.name));
    /* Set the Accessory name as the Private data for the service,
        * so that the correct accessory can be identified in the
        * write callback
        */
    hap_serv_set_priv(service, strdup(deviceList.device2.name));
    /* Set the write callback for the service */
    hap_serv_set_write_cb(service, homekit_write);
    /* Get pointer to the on_char to be used during update */
    device2_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON);
    /* Add the Fan Service to the Accessory Object */
    hap_acc_add_serv(accessory, service);
    /* Add the Accessory to the HomeKit Database */
    hap_add_bridged_accessory(accessory, hap_get_unique_aid(deviceList.device2.name));


    /*
     * Bridge Accessory
     */ 
    hap_acc_cfg_t device3_cfg = {
        .name = deviceList.device3.name,
        .manufacturer = MANUFACTURER,
        .model = MODEL,
        .serial_num = SERIAL_NUMBER,
        .fw_rev = FIRMWARE_REVISION,
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = accessory_identify,
        .cid = HAP_CID_BRIDGE,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&device3_cfg);
    /* Create the Fan Service. Include the "name" since this is a user visible service  */
    service = hap_serv_switch_create(app_driver_get_state(deviceList.device3.id));
    hap_serv_add_char(service, hap_char_name_create(deviceList.device3.name));
    /* Set the Accessory name as the Private data for the service,
        * so that the correct accessory can be identified in the
        * write callback
        */
    hap_serv_set_priv(service, strdup(deviceList.device3.name));
    /* Set the write callback for the service */
    hap_serv_set_write_cb(service, homekit_write);
    /* Get pointer to the on_char to be used during update */
    device3_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON);
    /* Add the Fan Service to the Accessory Object */
    hap_acc_add_serv(accessory, service);
    /* Add the Accessory to the HomeKit Database */
    hap_add_bridged_accessory(accessory, hap_get_unique_aid(deviceList.device3.name));



    /*
     * Bridge Accessory
     */ 
    hap_acc_cfg_t device4_cfg = {
        .name = deviceList.device4.name,
        .manufacturer = MANUFACTURER,
        .model = MODEL,
        .serial_num = SERIAL_NUMBER,
        .fw_rev = FIRMWARE_REVISION,
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = accessory_identify,
        .cid = HAP_CID_BRIDGE,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&device4_cfg);
    /* Create the Fan Service. Include the "name" since this is a user visible service  */
    service = hap_serv_switch_create(app_driver_get_state(deviceList.device4.id));
    hap_serv_add_char(service, hap_char_name_create(deviceList.device4.name));
    /* Set the Accessory name as the Private data for the service,
        * so that the correct accessory can be identified in the
        * write callback
        */
    hap_serv_set_priv(service, strdup(deviceList.device4.name));
    /* Set the write callback for the service */
    hap_serv_set_write_cb(service, homekit_write);
    /* Get pointer to the on_char to be used during update */
    device4_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON);
    /* Add the Fan Service to the Accessory Object */
    hap_acc_add_serv(accessory, service);
    /* Add the Accessory to the HomeKit Database */
    hap_add_bridged_accessory(accessory, hap_get_unique_aid(deviceList.device4.name));



    /*
     * Bridge Accessory
     */ 
    hap_acc_cfg_t device5_cfg = {
        .name = deviceList.device5.name,
        .manufacturer = MANUFACTURER,
        .model = MODEL,
        .serial_num = SERIAL_NUMBER,
        .fw_rev = FIRMWARE_REVISION,
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = accessory_identify,
        .cid = HAP_CID_BRIDGE,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&device5_cfg);
    /* Create the Fan Service. Include the "name" since this is a user visible service  */
    service = hap_serv_switch_create(app_driver_get_state(deviceList.device5.id));
    hap_serv_add_char(service, hap_char_name_create(deviceList.device5.name));
    /* Set the Accessory name as the Private data for the service,
        * so that the correct accessory can be identified in the
        * write callback
        */
    hap_serv_set_priv(service, strdup(deviceList.device5.name));
    /* Set the write callback for the service */
    hap_serv_set_write_cb(service, homekit_write);
    /* Get pointer to the on_char to be used during update */
    device5_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON);
    /* Add the Fan Service to the Accessory Object */
    hap_acc_add_serv(accessory, service);
    /* Add the Accessory to the HomeKit Database */
    hap_add_bridged_accessory(accessory, hap_get_unique_aid(deviceList.device5.name));



    /*
     * Bridge Accessory
     */ 
    hap_acc_cfg_t device6_cfg = {
        .name = deviceList.device6.name,
        .manufacturer = MANUFACTURER,
        .model = MODEL,
        .serial_num = SERIAL_NUMBER,
        .fw_rev = FIRMWARE_REVISION,
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = accessory_identify,
        .cid = HAP_CID_BRIDGE,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&device6_cfg);
    /* Create the Fan Service. Include the "name" since this is a user visible service  */
    service = hap_serv_switch_create(app_driver_get_state(deviceList.device6.id));
    hap_serv_add_char(service, hap_char_name_create(deviceList.device6.name));
    /* Set the Accessory name as the Private data for the service,
        * so that the correct accessory can be identified in the
        * write callback
        */
    hap_serv_set_priv(service, strdup(deviceList.device6.name));
    /* Set the write callback for the service */
    hap_serv_set_write_cb(service, homekit_write);
    /* Get pointer to the on_char to be used during update */
    device6_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON);
    /* Add the Fan Service to the Accessory Object */
    hap_acc_add_serv(accessory, service);
    /* Add the Accessory to the HomeKit Database */
    hap_add_bridged_accessory(accessory, hap_get_unique_aid(deviceList.device6.name));


    if (isAHT10Connected) {
        /*
        *  Display Brightness
        */
        hap_acc_cfg_t display_brightness_cfg = {
            .name = display_brightness,
            .manufacturer = MANUFACTURER,
            .model = MODEL,
            .serial_num = SERIAL_NUMBER,
            .fw_rev = FIRMWARE_REVISION,
            .hw_rev = NULL,
            .pv = "1.1.0",
            .identify_routine = accessory_identify,
            .cid = HAP_CID_BRIDGE,
        };

        /* Create accessory object */
        accessory = hap_acc_create(&display_brightness_cfg);

        /* Create the Light Bulb Service. Include the "name" since this is a user visible service  */
        service = hap_serv_lightbulb_create(false);

        /* Add the optional characteristic to the Light Bulb Service */
        hap_serv_add_char(service, hap_char_name_create(display_brightness));
        hap_serv_add_char(service, hap_char_brightness_create(0));

        hap_serv_set_priv(service, strdup(display_brightness));
        hap_serv_set_write_cb(service, homekit_write);
        temperatureDevice_display_brightness_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_BRIGHTNESS);
        temperatureDevice_display_brightness_power_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON);
        hap_acc_add_serv(accessory, service);
        hap_add_bridged_accessory(accessory, hap_get_unique_aid(display_brightness));

        /*
        *  Display Mode
        */
        hap_acc_cfg_t display_mode_cfg = {
            .name = display_mode,
            .manufacturer = MANUFACTURER,
            .model = MODEL,
            .serial_num = SERIAL_NUMBER,
            .fw_rev = FIRMWARE_REVISION,
            .hw_rev = NULL,
            .pv = "1.1.0",
            .identify_routine = accessory_identify,
            .cid = HAP_CID_BRIDGE,
        };

        /* Create accessory object */
        accessory = hap_acc_create(&display_mode_cfg);

        service = hap_serv_switch_create(displayMode);

        /* Add the optional characteristic to the Light Bulb Service */
        hap_serv_add_char(service, hap_char_name_create(display_mode));

        hap_serv_set_priv(service, strdup(display_mode));
        hap_serv_set_write_cb(service, homekit_write);
        temperatureDevice_display_mode_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON);
        hap_acc_add_serv(accessory, service);
        hap_add_bridged_accessory(accessory, hap_get_unique_aid(display_mode));

        /*
        * Bridge Accessory Temperature Sensor
        */ 
        hap_acc_cfg_t temperatureDevice_cfg = {
            .name = deviceList.temperatureDevice.name,
            .manufacturer = MANUFACTURER,
            .model = MODEL,
            .serial_num = SERIAL_NUMBER,
            .fw_rev = FIRMWARE_REVISION,
            .hw_rev = NULL,
            .pv = "1.1.0",
            .identify_routine = accessory_identify,
            .cid = HAP_CID_BRIDGE,
        };
        /* Create accessory object */
        accessory = hap_acc_create(&temperatureDevice_cfg);
        /* Create the Temperature Sensor Service */

        service = hap_serv_temperature_sensor_create(0);

        /* Add Name Characteristic */
        hap_serv_add_char(service, hap_char_name_create(deviceList.temperatureDevice.name));

        /* Add Temperature Display Units Characteristic */
        hap_serv_add_char(service, hap_char_temperature_display_units_create(0)); // 0 = Celsius, 1 = Fahrenheit

        /* Set the Accessory name as the Private data for the service,
         * so that the correct accessory can be identified in the
         * write callback
         */
        hap_serv_set_priv(service, strdup(deviceList.temperatureDevice.name));
        /* Get pointer to the on_char to be used during update */
        temperatureDevice_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_CURRENT_TEMPERATURE);
        /* Add the Fan Service to the Accessory Object */
        hap_acc_add_serv(accessory, service);
        /* Add the Accessory to the HomeKit Database */
        hap_add_bridged_accessory(accessory, hap_get_unique_aid(deviceList.temperatureDevice.name));



        /*
        * Bridge Accessory - Humidity Sensor
        */ 
        hap_acc_cfg_t humidityDevice_cfg = {
            .name = deviceList.humidityDevice.name,
            .manufacturer = MANUFACTURER,
            .model = MODEL,
            .serial_num = SERIAL_NUMBER,
            .fw_rev = FIRMWARE_REVISION,
            .hw_rev = NULL,
            .pv = "1.1.0",
            .identify_routine = accessory_identify,
            .cid = HAP_CID_BRIDGE,
        };

        /* Create Accessory Object (Automatically adds Accessory Information Service) */
        accessory = hap_acc_create(&humidityDevice_cfg);

        /* Create the Humidity Sensor Service */
        service = hap_serv_humidity_sensor_create(0); // Initial humidity value

        /* Add Name Characteristic */
        hap_serv_add_char(service, hap_char_name_create(deviceList.humidityDevice.name));

        /* Set the Accessory name as Private Data */
        hap_serv_set_priv(service, strdup(deviceList.humidityDevice.name));

        /* Get Humidity Characteristic */
        humidityDevice_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_CURRENT_RELATIVE_HUMIDITY);

        /* Add Humidity Sensor Service to the Accessory */
        hap_acc_add_serv(accessory, service);

        /* Add Accessory to HomeKit Database */
        hap_add_bridged_accessory(accessory, hap_get_unique_aid(deviceList.humidityDevice.name));
    }


    /* For production accessories, the setup code shouldn't be programmed on to
     * the device. Instead, the setup info, derived from the setup code must
     * be used. Use the factory_nvs_gen utility to generate this data and then
     * flash it into the factory NVS partition.
     *
     * By default, the setup ID and setup info will be read from the factory_nvs
     * Flash partition and so, is not required to set here explicitly.
     *
     * However, for testing purpose, this can be overridden by using hap_set_setup_code()
     * and hap_set_setup_id() APIs, as has been done here.
     */
#ifdef CONFIG_EXAMPLE_USE_HARDCODED_SETUP_CODE
    /* Unique Setup code of the format xxx-xx-xxx. Default: 111-22-333 */
    hap_set_setup_code(CONFIG_EXAMPLE_SETUP_CODE);
    /* Unique four character Setup Id. Default: ES32 */
    hap_set_setup_id(CONFIG_EXAMPLE_SETUP_ID);
    if (hap_get_paired_controller_count() == 0) {
        app_homekit_show_qr();
    }
#endif
    hap_enable_mfi_auth(HAP_MFI_AUTH_HW);
    /* Register our event handler for Wi-Fi, IP and Provisioning related events */
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &app_homekit_event_handler, NULL);
    esp_event_handler_register(HAP_EVENT, HAP_EVENT_CTRL_UNPAIRED, &app_homekit_event_handler, NULL);

    /* After all the initializations are done, start the HAP core */
    if (hap_start() == 0) {
        ESP_LOGI(TAG, "HomeKit started successfully");
        return ESP_OK;
    }
    ESP_LOGE(TAG, "Failed to start HomeKit");
    return ESP_FAIL;

}
