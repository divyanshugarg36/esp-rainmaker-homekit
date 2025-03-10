#include <stdio.h>
#include <app_priv.h>
#include <string.h>
#include "app_config.h"

uint8_t PRIMARY_MAC[] = {0x5C, 0x01, 0x3B, 0x4A, 0x00, 0xAC};
uint8_t SECONDARY_MAC[] = {0x08, 0xD1, 0xF9, 0x35, 0x64, 0x20};

char *node_name = UNASSIGNED;
char *node_type = UNASSIGNED;

Devices deviceList = {
    .device1 = {
        .id = DEVICE_1_ID,
        .uid = "D1",
        .name = UNASSIGNED,
        .gpio = DEVICE_1_OUTPUT_GPIO,
        .gpioIn = DEVICE_1_PCF_GPIO,
        .type = NONE
    },
    .device2 = {
        .id = DEVICE_2_ID,
        .uid = "D2",
        .name = UNASSIGNED,
        .gpio = DEVICE_2_OUTPUT_GPIO,
        .gpioIn = DEVICE_2_PCF_GPIO,
        .type = NONE
    },
    .device3 = {
        .id = DEVICE_3_ID,
        .uid = "D3",
        .name = UNASSIGNED,
        .gpio = DEVICE_3_OUTPUT_GPIO,
        .gpioIn = DEVICE_3_PCF_GPIO,
        .type = NONE
    },
    .device4 = {
        .id = DEVICE_4_ID,
        .uid = "D4",
        .name = UNASSIGNED,
        .gpio = DEVICE_4_OUTPUT_GPIO,
        .gpioIn = DEVICE_4_PCF_GPIO,
        .type = NONE
    },
    .device5 = {
        .id = DEVICE_5_ID,
        .uid = "D5",
        .name = UNASSIGNED,
        .gpio = DEVICE_5_OUTPUT_GPIO,
        .gpioIn = DEVICE_5_PCF_GPIO,
        .type = NONE
    },
    .device6 = {
        .id = DEVICE_6_ID,
        .uid = "D6",
        .name = UNASSIGNED,
        .gpio = DEVICE_6_OUTPUT_GPIO,
        .gpioIn = DEVICE_6_PCF_GPIO,
        .type = NONE
    },
    .device7 = {
        .id = DEVICE_7_ID,
        .uid = "D7",
        .name = UNASSIGNED,
        .gpio = DEVICE_7_OUTPUT_GPIO,
        .gpioIn = DEVICE_7_PCF_GPIO,
        .type = NONE
    },
    .device8 = {
        .id = DEVICE_8_ID,
        .uid = "D8",
        .name = UNASSIGNED,
        .gpio = DEVICE_8_OUTPUT_GPIO,
        .gpioIn = DEVICE_8_PCF_GPIO,
        .type = NONE
    },
    .temperatureDevice = {
        .id = DEVICE_TEMPERATURE,
        .uid = "D9",
        .name = "Temperature",
    },
    .humidityDevice = {
        .id = DEVICE_HUMIDITY,
        .uid = "D10",
        .name = "Humidity",
    },
    .displayBrightnessDevice = {
        .id = DEVICE_DISPLAY_BRIGHTNESS,
        .uid = "D11",
        .name = "Brightness",
    },
    .displayModeDevice = {
        .id = DEVICE_DISPLAY_MODE,
        .uid = "D12",
        .name = "Display Mode",
    }
};

void initialize_device_list(uint8_t *mac) {
    if (memcmp(mac, PRIMARY_MAC, 6) == 0) {
        node_name = strdup("Main Hall");
        node_type = strdup("Primary");

        deviceList.device1.name = strdup("Main Light");
        deviceList.device1.type = DEVICE_TYPE_LIGHT;
        deviceList.device2.name = strdup("Side Light 1");
        deviceList.device2.type = DEVICE_TYPE_LIGHTBULB;
        deviceList.device3.name = strdup("Side Light 2");
        deviceList.device3.type = DEVICE_TYPE_LIGHTBULB;
        deviceList.device4.name = strdup("Fan 1");
        deviceList.device4.type = DEVICE_TYPE_FAN;
        deviceList.device5.name = strdup("Fan 2");
        deviceList.device5.type = DEVICE_TYPE_FAN;
        deviceList.device6.name = strdup("Down Ceiling");
        deviceList.device6.type = DEVICE_TYPE_LIGHT;
        deviceList.device7.name = strdup("Clock Switch");
        deviceList.device7.type = DEVICE_TYPE_SOCKET;
        // deviceList.device8.name = strdup("Ceiling Light 2");
        // deviceList.device8.type = DEVICE_TYPE_LIGHTBULB;
    } else if (memcmp(mac, SECONDARY_MAC, 6) == 0) {
        node_name = strdup("Lobby");
        node_type = strdup("Secondary");

        deviceList.device1.name = strdup("Outer Switch");
        deviceList.device1.type = DEVICE_TYPE_SOCKET;
        deviceList.device2.name = strdup("Table Switch R");
        deviceList.device2.type = DEVICE_TYPE_SOCKET;
        deviceList.device3.name = strdup("Big Lamp");
        deviceList.device3.type = DEVICE_TYPE_LIGHT;
        deviceList.device4.name = strdup("Table Switch L");
        deviceList.device4.type = DEVICE_TYPE_SOCKET;
        deviceList.device5.name = strdup("Outer Bulb");
        deviceList.device5.type = DEVICE_TYPE_LIGHTBULB;
        deviceList.device6.name = strdup("Small Lamp");
        deviceList.device6.type = DEVICE_TYPE_LIGHT;
    }
}