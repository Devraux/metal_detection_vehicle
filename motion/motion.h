#ifndef _motion_
#define _motion_

#include <stdint.h>
#include "hardware/gpio.h"
#include "servo/servo.h"

#define servo_1_gpio 10     //front - right 
#define servo_2_gpio 11     //back  - right
#define servo_3_gpio 12     //back  - left
#define servo_4_gpio 13     //front - left

#define hall_1_gpio 16      //right hall sensor
#define hall_2_gpio 17      //left hall sensor

typedef struct motion_t{
    uint32_t hall_counter;
    uint32_t distance;
    float converted_distance;
    bool making_turn;
}motion_t; 

/// @brief vehicle motion devices(hall sensors and servos initialization)
/// @param servo_1_gpio servo 1 gpio pin
/// @param servo_2_gpio servo 2 gpio pin
/// @param servo_3_gpio servo 3 gpio pin
/// @param servo_4_gpio servo 4 gpio pin
/// @param hall_1_gpio  hall sensor gpio pin
/// @param hall_2_gpio  hall sensor gpio pin 
void move_Init(uint8_t servo_1_gpio_t, uint8_t servo_2_gpio_t, uint8_t servo_3_gpio_t, uint8_t servo_4_gpio_t, uint8_t hall_1_gpio_t, uint8_t hall_2_gpio_t, void *gpio_callback);

/// @brief hall initialization func
/// @param gpio_num hall sensor gpio pin number
/// @param gpio_callback gpio callback function
void hall_Init(uint8_t gpio_num, void *gpio_callback);

/// @brief distance update
void distance_Update(void);

/// @brief get robot distance
/// @return current distance
uint32_t get_distance(void);

#endif