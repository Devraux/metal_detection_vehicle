#ifndef _motion_
#define _motion_

#include <stdint.h>
#include "hardware/gpio.h"
#include "servo/servo.h"
#include "math.h"
#include "mpu6050.h"

#define servo_front_right 10   //front <-> right 
#define servo_back_right  11   //back  <-> right
#define servo_back_left   12   //back  <-> left
#define servo_front_left  13   //front <-> left

#define hall_right 16          //right hall sensor
#define hall_left  17          //left hall sensor
#define hall_distance 0.05f    //traveled distance between hall sensor irq's

#define drive_forward   1
#define drive_backward  2
#define drive_left      3
#define drive_right     4
#define drive_stop      5

typedef struct motion_t{
    float    distance;
    uint32_t distance_Absolute;
    uint8_t  move_Direction; // 0-> forward 1-> back 2->left 3->right 9->STOP
    uint32_t velocity;       // current velocity from -250 to 250
    float current_position_X;
    float current_position_Y;
}motion_t; 

/// @brief vehicle motion devices(hall sensors and servos initialization)
/// @param servo_1_gpio servo 1 gpio pin
/// @param servo_2_gpio servo 2 gpio pin
/// @param servo_3_gpio servo 3 gpio pin
/// @param servo_4_gpio servo 4 gpio pin
/// @param hall_1_gpio  hall sensor gpio pin
/// @param hall_2_gpio  hall sensor gpio pin 
void motion_Init(uint8_t servo_front_left_t, uint8_t servo_front_right_t, uint8_t servo_back_left_t, uint8_t servo_back_right_t, uint8_t hall_left_t, uint8_t hall_right_t, void *gpio_callback);

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
/// @return 1-> forward 2-> back 3->left 4->right 5->STOP
uint8_t get_Move_Direction(void);

/// @brief causes robot movement and provides soft-start
/// @param move_direction_t 0-> forward 1-> back 2->left 3->right 4 -> STOP
/// @param velocity_t vehicle to set velocity
void move(uint8_t move_direction_t, int16_t velocity_t);

/// @brief compute and update X and Y coordinates of device
/// @param angle angle for example from mpu6050
void XY_Position_Update(float angle);

/// @brief calculate XY cartesian position
/// @param X X position  
/// @param Y Y position
void motion_Get_XY(float *X, float *Y);

/// @brief converter from degrees to radians 
/// @param radians 
/// @return 
float deg_To_Rad(float degrees);

/// @brief Turn left based on accelerometer
/// @param -
void turn_Left(void);

/// @brief Turn Right based on accelerometer
/// @param -
void turn_Right(void);

//TODO
//void drive_Forward();  bad function name
//void drive_Backward(); bad function name
#endif