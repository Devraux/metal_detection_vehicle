# include "motion.h"

motion_t motion = {0};

void hall_Init(uint8_t gpio_num, void *gpio_callback)
{   
    gpio_init(gpio_num);
    gpio_set_dir(gpio_num, GPIO_IN);
    gpio_set_irq_enabled_with_callback(gpio_num, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
}

void motion_Init(uint8_t servo_front_left_t, uint8_t servo_front_right_t, uint8_t servo_back_left_t, uint8_t servo_back_right_t, uint8_t hall_left_t, uint8_t hall_right_t, void *gpio_callback)
{
    servo_Init(servo_front_left_t);
    servo_Init(servo_front_right_t);
    servo_Init(servo_back_left_t);
    servo_Init(servo_back_right_t);

    hall_Init(hall_left_t, gpio_callback);
    hall_Init(hall_right_t, gpio_callback);

    mpu_Init();
}

void distance_Update(void)
{
    if(motion.move_Direction == drive_forward)      // forward
        motion.distance += hall_distance;   // ~0.05 meter

    else if(motion.move_Direction == drive_backward) //backward
        motion.distance -= hall_distance;   // ~0.05 meter
    
    motion.distance_Absolute += hall_distance;
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
    if(get_Move_Direction() == drive_forward)  //forward
    {  
        motion.current_position_X += hall_distance * cos(deg_To_Rad(angle));
        motion.current_position_Y += hall_distance * sin(deg_To_Rad(angle));
    }

    if(get_Move_Direction() == drive_backward) //backward
    {   
        motion.current_position_X += hall_distance * cos(deg_To_Rad(angle + 180.0f));
        motion.current_position_Y += hall_distance * sin(deg_To_Rad(angle + 180.0f));
    }
}

void motion_Get_XY(float *X, float *Y)
{
    *X = motion.current_position_X;
    *Y = motion.current_position_Y;
}

void move(uint8_t move_direction_t, int16_t velocity_t)
{
    switch(move_direction_t)
    {
        case drive_forward: //forward
            servo_Set_Velocity(servo_front_left, velocity_t);
            servo_Set_Velocity(servo_front_right,velocity_t);
            servo_Set_Velocity(servo_back_left,  velocity_t);
            servo_Set_Velocity(servo_back_right, velocity_t);
            motion.move_Direction = drive_forward;
        break;

        case drive_backward: //back
            servo_Set_Velocity(servo_front_left, -velocity_t);
            servo_Set_Velocity(servo_front_right,-velocity_t);
            servo_Set_Velocity(servo_back_left,  -velocity_t);
            servo_Set_Velocity(servo_back_right, -velocity_t);
            motion.move_Direction = drive_backward;
        break;

        case drive_left: //left
            servo_Set_Velocity(servo_front_left, -velocity_t/5);
            servo_Set_Velocity(servo_front_right, velocity_t);
            servo_Set_Velocity(servo_back_left,  -velocity_t/5);
            servo_Set_Velocity(servo_back_right,  velocity_t);
            motion.move_Direction = drive_left;
 
        break;

        case drive_right: //right
            servo_Set_Velocity(servo_front_left,  velocity_t);
            servo_Set_Velocity(servo_front_right,-velocity_t/5);
            servo_Set_Velocity(servo_back_left,   velocity_t);
            servo_Set_Velocity(servo_back_right, -velocity_t/5);
            motion.move_Direction = drive_right;

        break;

        case drive_stop: //stop
            servo_Set_Velocity(servo_front_left, 0); //STOP Vehicle
            servo_Set_Velocity(servo_front_right,0); //STOP Vehicle
            servo_Set_Velocity(servo_back_left,  0); //STOP Vehicle
            servo_Set_Velocity(servo_back_right, 0); //STOP Vehicle
            motion.move_Direction = drive_stop;

        default:
            servo_Set_Velocity(servo_front_left, 0); //STOP Vehicle
            servo_Set_Velocity(servo_front_right,0); //STOP Vehicle
            servo_Set_Velocity(servo_back_left,  0); //STOP Vehicle
            servo_Set_Velocity(servo_back_right, 0); //STOP Vehicle
            motion.move_Direction = drive_stop;
    }   
}

float deg_To_Rad(float degrees)
{
    if(degrees >= 360)
        degrees = fmod(degrees, 360.0f);

    if(degrees < 0)
        degrees = fmod(degrees + 360.0f, 360.0f);

    float result = degrees * (M_PI / 180.0f);
    return result;
}

void turn_Left()
{
    float current_Angle = mpu_Get_Yaw();
    while(mpu_Get_Yaw() <= current_Angle + 90.0f)
        move(drive_left, 250);
    move(drive_stop, 0); //STOP
}

void turn_Right()
{
    float current_Angle = mpu_Get_Yaw();
    while(mpu_Get_Yaw() >= current_Angle - 90.0f)
        move(drive_right, 250);
    move(drive_stop, 0); //STOP
}

/// @todo
//void drive_Forward();  bad function name
//void drive_Backward(); bad function name