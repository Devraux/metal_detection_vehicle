#ifndef _SERVO_
#define _SERVO_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"



/// @brief servo initialization
/// @param gpio_num Pi Pico GPIO number  
void servo_Init(uint8_t gpio_num);


/// @brief servo_set_velocity
/// @param gpio_num gpio number
/// @param velocity value from -250 to 250
/// @note in most cases it is recommended to use maximum velocity eg. 250 or -250 
void servo_Set_Velocity(uint8_t gpio_num, int16_t velocity);
 


#endif