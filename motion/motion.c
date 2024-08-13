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
    motion.distance += 0.0447761;
}

float get_Distance(void)
{
    return motion.distance;
}

void move(uint8_t side, int16_t velocity)
{
    switch(side)
    {
        case 1: //forward
            servo_set_velocity(servo_front_left, velocity);
            servo_set_velocity(servo_front_right,velocity);
            servo_set_velocity(servo_back_left,  velocity);
            servo_set_velocity(servo_back_right, velocity);
        break;

        case 2: //back
            servo_set_velocity(servo_front_left, -velocity);
            servo_set_velocity(servo_front_right,-velocity);
            servo_set_velocity(servo_back_left,  -velocity);
            servo_set_velocity(servo_back_right, -velocity);
        break;

        case 3: //left
            servo_set_velocity(servo_front_left, -velocity/5);
            servo_set_velocity(servo_front_right, velocity);
            servo_set_velocity(servo_back_left,  -velocity/5);
            servo_set_velocity(servo_back_right,  velocity);
 
        break;

        case 4: //right
            servo_set_velocity(servo_front_left,  velocity/5);
            servo_set_velocity(servo_front_right,-velocity);
            servo_set_velocity(servo_back_left,   velocity/5);
            servo_set_velocity(servo_back_right, -velocity);

        break;

        default:
            servo_set_velocity(servo_front_left, 0);
            servo_set_velocity(servo_front_right,0);
            servo_set_velocity(servo_back_left,  0);
            servo_set_velocity(servo_back_right, 0);
    }   
}