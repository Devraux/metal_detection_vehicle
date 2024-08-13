#ifndef _motion_
#define _motion_

#include <stdint.h>
#include "hardware/gpio.h"
#include "servo/servo.h"

#define servo_front_right 10     //front - right 
#define servo_back_right 11     //back  - right
#define servo_back_left 12     //back  - left
#define servo_front_left 13     //front - left

#define hall_right 16      //right hall sensor
#define hall_left 17      //left hall sensor

typedef struct motion_t{
    uint32_t hall_counter;
    bool soft_start;   // True-> soft start finished, false otherwise
    float distance;
    uint32_t velocity; // velocity from -250 to 250

}motion_t; 

/// @brief vehicle motion devices(hall sensors and servos initialization)
/// @param servo_1_gpio servo 1 gpio pin
/// @param servo_2_gpio servo 2 gpio pin
/// @param servo_3_gpio servo 3 gpio pin
/// @param servo_4_gpio servo 4 gpio pin
/// @param hall_1_gpio  hall sensor gpio pin
/// @param hall_2_gpio  hall sensor gpio pin 
void move_Init(uint8_t servo_front_left_t, uint8_t servo_front_right_t, uint8_t servo_back_left_t, uint8_t servo_back_right_t, uint8_t hall_left_t, uint8_t hall_right_t, void *gpio_callback);

/// @brief hall initialization func
/// @param gpio_num hall sensor gpio pin number
/// @param gpio_callback gpio callback function
void hall_Init(uint8_t gpio_num, void *gpio_callback);

/// @brief distance update
void distance_Update(void);

/// @brief get robot distance
/// @return current distance
float get_Distance(void);

/// @brief causes robot movement and provides soft-start
/// @param side 1-> forward 2-> back 3->left 4->right 5->forward-left 6->forward-right 7->back-left 8->back-right
/// @param velocity 
void move(uint8_t side, int16_t velocity);

#endif