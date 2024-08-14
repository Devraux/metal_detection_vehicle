#ifndef _METAL_DETECTION_
#define _METAL_DETECTION_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#define metal_detect_gpio 18

typedef struct metal_detect_data
{
    //uint32_t last_detect_time;
    //uint32_t elapsed_time;
    bool is_metal_detected;       // 0 -> metal is not detected
    uint32_t detected_metal_counter; //counts metal occurrences 

}metal_detect_data;

extern metal_detect_data metal_detect_data_t;

/// @brief initialize metal detection part
/// @param  -- 
void metal_detect_init(uint8_t gpio_num_t, void *gpio_callback);

/// @brief metal detection
/// @param  --
/// @return 0 -> metal not detected, 1 -> metal detected
void metal_detect_callback(void);

#endif