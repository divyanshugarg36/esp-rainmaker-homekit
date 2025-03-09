#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdbool.h>

typedef enum
{
    NONE,
    DEVICE_TYPE_SOCKET,
    DEVICE_TYPE_LIGHT,
    DEVICE_TYPE_LIGHTBULB,
    DEVICE_TYPE_FAN
} DeviceType;

typedef struct
{
    int id;
    char *uid;
    char *name;
    int gpio;
    int gpioIn;
    DeviceType type;
} Switch;

typedef struct
{
    Switch device1;
    Switch device2;
    Switch device3;
    Switch device4;
    Switch device5;
    Switch device6;
    Switch device7;
    Switch device8;
    Switch temperatureDevice;
    Switch humidityDevice;
    Switch displayBrightnessDevice;
    Switch displayModeDevice;
} Devices;

extern Devices deviceList; // Global variable declaration

void initialize_device_list(uint8_t *mac);

#endif // APP_CONFIG_H
