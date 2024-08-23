# include "motion.h"

motion_t motion = {0};

void hall_Init(uint8_t gpio_num, void *gpio_callback)
{   
    gpio_init(gpio_num);
    gpio_set_dir(gpio_num, GPIO_IN);
    gpio_set_irq_enabled_with_callback(gpio_num, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
}

void move_Init(uint8_t servo_front_left_t, uint8_t servo_front_right_t, uint8_t servo_back_left_t, uint8_t servo_back_right_t, uint8_t hall_left_t, uint8_t hall_right_t, void *gpio_callback)
{
    servo_init(servo_front_left_t);
    servo_init(servo_front_right_t);
    servo_init(servo_back_left_t);
    servo_init(servo_back_right_t);

    hall_Init(hall_left_t, gpio_callback);
    hall_Init(hall_right_t, gpio_callback);
}

void distance_Update(void)
{
    if(motion.move_Direction == 0)      // forward
        motion.distance += hall_distance;   // ~0.04477 meter

    else if(motion.move_Direction == 1) //backward
        motion.distance -= hall_distance;   // ~0.04477 meter
    
    motion.distance_Absolute += 0.0447761;
}

uint8_t get_Move_Direction(void)
{
    return motion.move_Direction;
}

float get_Distance(void)
{
    return motion.distance;
}

void XY_Position_Update(float angle)
{
    motion.current_position_X += hall_distance * cos(angle);
    motion.current_position_Y += hall_distance * sin(angle);
}

void get_XY_Position(float *X, float *Y)
{
    *X = motion.current_position_X;
    *Y = motion.current_position_Y;
}

void move(uint8_t move_direction_t, int16_t velocity_t)
{
    switch(move_direction_t)
    {
        case 0: //forward
            servo_set_velocity(servo_front_left, velocity_t);
            servo_set_velocity(servo_front_right,velocity_t);
            servo_set_velocity(servo_back_left,  velocity_t);
            servo_set_velocity(servo_back_right, velocity_t);
            motion.move_Direction = 0;
        break;

        case 1: //back
            servo_set_velocity(servo_front_left, -velocity_t);
            servo_set_velocity(servo_front_right,-velocity_t);
            servo_set_velocity(servo_back_left,  -velocity_t);
            servo_set_velocity(servo_back_right, -velocity_t);
            motion.move_Direction = 1;
        break;

        case 2: //left
            servo_set_velocity(servo_front_left, -velocity_t/5);
            servo_set_velocity(servo_front_right, velocity_t);
            servo_set_velocity(servo_back_left,  -velocity_t/5);
            servo_set_velocity(servo_back_right,  velocity_t);
            motion.move_Direction = 2;
 
        break;

        case 3: //right
            servo_set_velocity(servo_front_left,  velocity_t/5);
            servo_set_velocity(servo_front_right,-velocity_t);
            servo_set_velocity(servo_back_left,   velocity_t/5);
            servo_set_velocity(servo_back_right, -velocity_t);
            motion.move_Direction = 3;

        break;

        default:
            servo_set_velocity(servo_front_left, 0); //STOP Vehicle
            servo_set_velocity(servo_front_right,0); //STOP Vehicle
            servo_set_velocity(servo_back_left,  0); //STOP Vehicle
            servo_set_velocity(servo_back_right, 0); //STOP Vehicle
            motion.move_Direction = 9;
    }   
}
