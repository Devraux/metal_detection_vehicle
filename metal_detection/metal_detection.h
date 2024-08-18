#ifndef _METAL_DETECTION_
#define _METAL_DETECTION_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "buffer/buffer.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#define metal_detect_gpio 18

typedef struct metal_detect_data_t
{
    bool metal_Detected;              // 0 -> metal is not detected, 1-> metal detected
    uint32_t detected_Metal_Counter;  //counts metal occurrences 
    uint32_t detection_Average;       //additional variable storing the average value of the time between the occurrences of the generator signal edges  
}metal_detect_data_t;

extern metal_detect_data_t metal_detect_data;

/// @brief initialize metal detection part
/// @param  -- 
void metal_Detect_Init(uint8_t gpio_num_t, void *gpio_callback);

/// @brief metal detection
/// @param  --
/// @return 0 -> metal not detected, 1 -> metal detected
void metal_Detect_Callback(void);

/// @brief metal detection get average value
/// @param timer pico sdk standard operator 
/// @return average time between metal detector generator edges
bool metal_Detect_Get_Avg(struct repeating_timer *timer);

/// @brief checks whether metal is occurred
/// @param  --
void check_Metal_Detect(void);

void get_Metal_Info(metal_detect_data_t *metal_detect_data);
#endif