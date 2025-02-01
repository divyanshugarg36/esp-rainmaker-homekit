#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include <esp32_mock.h>

#define I2C_MASTER_NUM I2C_NUM_0  // I2C port number
#define I2C_MASTER_SDA_IO 21      // GPIO for SDA
#define I2C_MASTER_SCL_IO 22      // GPIO for SCL
#define I2C_MASTER_FREQ_HZ 100000 // I2C clock frequency
#define AHT10_ADDR 0x38           // I2C address of the AHT10 sensor
#define AHT10_CMD_TRIGGER 0xAC    // Command to trigger measurement
#define AHT10_CMD_SOFTRESET 0xBA  // Command to reset the sensor
#define AHT10_CMD_INIT 0xE1       // Command to initialize the sensor

// Initialize I2C
void i2c_master_init()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

// Write command to AHT10
esp_err_t aht10_write_command(uint8_t *cmd, size_t len)
{
    i2c_cmd_handle_t handle = i2c_cmd_link_create();
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (AHT10_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(handle, cmd, len, true);
    i2c_master_stop(handle);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, handle, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(handle);
    return ret;
}

// Read data from AHT10
esp_err_t aht10_read_data(uint8_t *data, size_t length)
{
    i2c_cmd_handle_t handle = i2c_cmd_link_create();
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (AHT10_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(handle, data, length, I2C_MASTER_LAST_NACK);
    i2c_master_stop(handle);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, handle, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(handle);
    return ret;
}

// Reset and Initialize AHT10
void aht10_init()
{
    uint8_t reset_cmd = AHT10_CMD_SOFTRESET;
    aht10_write_command(&reset_cmd, 1);
    vTaskDelay(pdMS_TO_TICKS(20)); // Wait after reset

    uint8_t init_cmd[] = {AHT10_CMD_INIT, 0x08, 0x00}; // Init command for AHT10
    aht10_write_command(init_cmd, sizeof(init_cmd));
    vTaskDelay(pdMS_TO_TICKS(20)); // Wait for initialization
}

// Get temperature and humidity
void aht10_get_temp_humidity(float *temperature, float *humidity)
{
    uint8_t measure_cmd[] = {AHT10_CMD_TRIGGER, 0x33, 0x00};
    uint8_t data[6];

    aht10_write_command(measure_cmd, sizeof(measure_cmd));
    vTaskDelay(pdMS_TO_TICKS(80)); // Wait for measurement

    if (aht10_read_data(data, 6) == ESP_OK)
    {
        uint32_t raw_humidity = ((data[1] << 16) | (data[2] << 8) | data[3]) >> 4;
        uint32_t raw_temperature = ((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5];

        *humidity = ((float)raw_humidity / 1048576.0) * 100.0;
        *temperature = ((float)raw_temperature / 1048576.0) * 200.0 - 50.0;
    }
    else
    {
        *humidity = -1.0;
        *temperature = -1.0;
    }
}

// Main application
void app_main()
{
    float temperature = 0.0, humidity = 0.0;

    i2c_master_init();
    aht10_init();

    while (1)
    {
        aht10_get_temp_humidity(&temperature, &humidity);
        printf("Temperature: %.2f °C, Humidity: %.2f %%\n", temperature, humidity);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
