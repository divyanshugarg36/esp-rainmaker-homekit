#include "nvs_flash.h"
#include "nvs.h"

#include "app_priv.h"
#include <iot_button.h>

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
    int32_t state = BUTTON_ACTIVE_LOW;
    if (nvs_open("storage", NVS_READONLY, &my_handle) == ESP_OK) {
        char key[10];
        sprintf(key, "device_%d", device);
        nvs_get_i32(my_handle, key, &state);
        nvs_close(my_handle);
    }
    return state;
}

