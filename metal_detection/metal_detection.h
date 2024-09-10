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
#include "motion/motion.h"

#define metal_detect_gpio 18

typedef struct metal_detect_data_t
{
    bool metal_Detected;              // 0 -> metal is not detected, 1-> metal detected
    uint32_t detected_Metal_Counter;  //counts metal occurrences 
    uint32_t detection_Average;       //additional variable storing the average value of the time between the occurrences of the generator signal edges  
}metal_detect_data_t;


/// @brief initialize metal detection part
/// @param  -- 
void metal_Detect_Init(uint8_t gpio_num_t, void *gpio_callback);

/// @brief metal detection
/// @param  --
/// @return 0 -> metal not detected, 1 -> metal detected
void metal_Detect_Irq(void);

/// @brief compute metal detections data
/// @param timer pico sdk standard operator 
/// @return average time between metal detector generator edges
static bool compute_Detections_Data(struct repeating_timer *timer);

/// @brief checks whether metal is occurred
/// @param  --
static void check_Metal_Detect(void);

/// @brief checks whether metal has been detected
/// @param --
/// @return true -> metal detected, false -> metal not detected
bool get_Metal_Detection_Status(void);

/// @brief return metal detections
/// @param  --
/// @return number of metal detections
uint32_t get_Metal_Detection_Counter(void);
#endif