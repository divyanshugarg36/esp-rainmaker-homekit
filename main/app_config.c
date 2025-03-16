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
        .gpioIn = PCF_GPIO_1,
        .type = NONE
    },
    .device2 = {
        .id = DEVICE_2_ID,
        .uid = "D2",
        .name = UNASSIGNED,
        .gpio = DEVICE_2_OUTPUT_GPIO,
        .gpioIn = PCF_GPIO_2,
        .type = NONE
    },
    .device3 = {
        .id = DEVICE_3_ID,
        .uid = "D3",
        .name = UNASSIGNED,
        .gpio = DEVICE_3_OUTPUT_GPIO,
        .gpioIn = PCF_GPIO_3,
        .type = NONE
    },
    .device4 = {
        .id = DEVICE_4_ID,
        .uid = "D4",
        .name = UNASSIGNED,
        .gpio = DEVICE_4_OUTPUT_GPIO,
        .gpioIn = PCF_GPIO_4,
        .type = NONE
    },
    .device5 = {
        .id = DEVICE_5_ID,
        .uid = "D5",
        .name = UNASSIGNED,
        .gpio = DEVICE_5_OUTPUT_GPIO,
        .gpioIn = PCF_GPIO_5,
        .type = NONE
    },
    .device6 = {
        .id = DEVICE_6_ID,
        .uid = "D6",
        .name = UNASSIGNED,
        .gpio = DEVICE_6_OUTPUT_GPIO,
        .gpioIn = PCF_GPIO_6,
        .type = NONE
    },
    .device7 = {
        .id = DEVICE_7_ID,
        .uid = "D7",
        .name = UNASSIGNED,
        .gpio = DEVICE_7_OUTPUT_GPIO,
        .gpioIn = PCF_GPIO_7,
        .type = NONE
    },
    .device8 = {
        .id = DEVICE_8_ID,
        .uid = "D8",
        .name = UNASSIGNED,
        .gpio = DEVICE_8_OUTPUT_GPIO,
        .gpioIn = PCF_GPIO_8,
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

        deviceList.device1.name = strdup("Clock Switch");
        deviceList.device1.type = DEVICE_TYPE_SOCKET;
        deviceList.device1.gpioIn = PCF_GPIO_1;

        deviceList.device2.name = strdup("Fan 2");
        deviceList.device2.type = DEVICE_TYPE_FAN;
        deviceList.device2.gpioIn =PCF_GPIO_3;

        deviceList.device3.name = strdup("Side Light 2");
        deviceList.device3.type = DEVICE_TYPE_LIGHTBULB;
        deviceList.device3.gpioIn =PCF_GPIO_2;

        deviceList.device4.name = strdup("Fan 1");
        deviceList.device4.type = DEVICE_TYPE_FAN;
        deviceList.device4.gpioIn = PCF_GPIO_4;

        // deviceList.device5.name = strdup("Nothing");
        deviceList.device5.type = DEVICE_TYPE_SOCKET;
        deviceList.device5.gpioIn = PCF_GPIO_5;

        deviceList.device6.name = strdup("Down Ceiling");
        deviceList.device6.type = DEVICE_TYPE_LIGHT;
        deviceList.device6.gpioIn =PCF_GPIO_7;

        deviceList.device7.name = strdup("Side Light 1");
        deviceList.device7.type = DEVICE_TYPE_LIGHTBULB;
        deviceList.device7.gpioIn =  PCF_GPIO_6;

        deviceList.device8.name = strdup("Main Light");
        deviceList.device8.type = DEVICE_TYPE_LIGHT;
        deviceList.device8.gpioIn = PCF_GPIO_8;

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