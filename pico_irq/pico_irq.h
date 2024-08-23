#ifndef _PICO_IRQ_
#define _PICO_IRQ_

#include <stdint.h> 
#include <stdio.h>

#include "hardware/timer.h"
#include "metal_detection/metal_detection.h"
#include "motion/motion.h"
#include "motion/mpu6050.h"


void gpio_callback(uint gpio, uint32_t events);


#endif