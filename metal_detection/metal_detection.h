#ifndef _METAL_DETECTION_
#define _METAL_DETECTION_

#include <stdint.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"

typedef struct metal_detect_data
{
    //uint32_t last_detect_time;
    //uint32_t elapsed_time;
    uint8_t is_metal_detected;
    uint32_t detected_metal_counter;

}metal_detect_data;

/// @brief initialize metal detection part
/// @param  -- 
void metal_detect_init(uint8_t gpio_num, void *gpio_callback);

/// @brief metal detection
/// @param  --
/// @return 0 -> metal not detected, 1 -> metal detected
void metal_detect_callback(metal_detect_data *metal_detect_data_t);

#endif