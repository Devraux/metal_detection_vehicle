#ifndef _motion_
#define _motion_

#include <stdint.h>
#include "hardware/gpio.h"
#include "servo/servo.h"

#define servo_front_right 10     //front - right 
#define servo_back_right 11      //back  - right
#define servo_back_left 12       //back  - left
#define servo_front_left 13      //front - left

#define hall_right 16            //right hall sensor
#define hall_left 17             //left hall sensor

typedef struct motion_t{
    float    distance;
    uint32_t distance_Absolute;
    uint8_t  move_Direction; // 0-> forward 1-> back 2->left 3->right 9->STOP
    uint32_t velocity;       // current velocity from -250 to 250
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

/// @brief vehicle current direction getter
/// @param - 
/// @return 0-> forward 1-> back 2->left 3->right 9->STOP
uint8_t get_Move_Direction(void);

/// @brief causes robot movement and provides soft-start
/// @param move_direction_t 0-> forward 1-> back 2->left 3->right
/// @param velocity_t vehicle to set velocity
void move(uint8_t move_direction_t, int16_t velocity_t);


//TODO - accelerometer compulsory
void turn_left();
void turn_right();

#endif