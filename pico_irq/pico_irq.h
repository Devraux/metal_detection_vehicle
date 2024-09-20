#ifndef _PICO_IRQ_
#define _PICO_IRQ_

#include <stdint.h> 
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "metal_detection/metal_detection.h"
#include "motion/motion.h"
#include "motion/mpu6050.h"

/// @brief gpio callback function which contain gpio irq callbacks
/// @param gpio gpio number which caused irq
/// @param events event type - Falling | Rising | Falling-Rising | High | Low <-> Edge / state  
void gpio_callback(uint gpio, uint32_t events);

/// @brief initialize irq priorities
/// @param --
void irq_Init(void);
#endif