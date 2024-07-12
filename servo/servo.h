#ifndef _servo_
#define _servo_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "hardware/pwm.h"
#include "hardware/gpio.h"

/**
 * @brief servo_init pwm slice init
 * @param gpio_num  GPIO NUMBER
 */
void servo_init(uint8_t gpio_num);

/**
 * @brief servo_set_velocity
 * @param gpio_num gpio number
 * @param velocity value from -250 to 250
 */
void servo_set_velocity(uint8_t gpio_num, int16_t velocity);
 


#endif