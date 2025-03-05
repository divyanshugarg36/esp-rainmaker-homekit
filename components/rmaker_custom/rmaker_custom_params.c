#include <stdint.h>
#include <esp_err.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h>

esp_rmaker_param_t *esp_rmaker_fan_speed_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, ESP_RMAKER_PARAM_SPEED,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, ESP_RMAKER_UI_DROPDOWN);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(1), esp_rmaker_int(6), esp_rmaker_int(1));
    }
    return param;
}

esp_rmaker_param_t *esp_rmaker_lcd_brightness_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, ESP_RMAKER_PARAM_BRIGHTNESS,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, ESP_RMAKER_UI_DROPDOWN);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(0), esp_rmaker_int(7), esp_rmaker_int(1));
    }
    return param;
}

// LCD Mode - Temperature or Humidity
esp_rmaker_param_t *esp_rmaker_lcd_mode_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, ESP_RMAKER_PARAM_MODE,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, ESP_RMAKER_UI_DROPDOWN);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(0), esp_rmaker_int(1), esp_rmaker_int(1));
    }
    return param;
}