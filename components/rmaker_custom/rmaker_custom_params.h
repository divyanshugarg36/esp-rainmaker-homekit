#include <stdint.h>
#include <esp_err.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h>

#define ESP_RMAKER_DEF_HUMIDITY_NAME      "Humidity"
#define ESP_RMAKER_DEF_LCD_MODE_NAME      "Display Mode"
#define ESP_RMAKER_DEVICE_HUMIDITY_SENSOR "esp.device.humidity"
#define ESP_RMAKER_PARAM_HUMIDITY         "esp.param.humidity"
#define ESP_RMAKER_PARAM_HUMIDITY         "esp.param.humidity"

esp_rmaker_param_t *esp_rmaker_power_push_param_create(const char *param_name, bool val);
esp_rmaker_param_t *esp_rmaker_fan_speed_param_create(const char *param_name, int val);
esp_rmaker_param_t *esp_rmaker_lcd_brightness_param_create(const char *param_name, int val);
esp_rmaker_param_t *esp_rmaker_lcd_mode_param_create(const char *param_name, int val);
