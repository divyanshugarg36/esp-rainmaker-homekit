#include <stdio.h>
#include "app_utils.h"

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
