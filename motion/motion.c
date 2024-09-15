# include "motion.h"

motion_t motion = {0};
PID_Regulator_t PID_Regulator = {0};

void hall_Init(uint8_t gpio_num, void *gpio_callback)
{   
    gpio_init(gpio_num);
    gpio_set_dir(gpio_num, GPIO_IN);
    gpio_set_irq_enabled_with_callback(gpio_num, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
}

void motion_Init(uint8_t servo_front_left_t, uint8_t servo_front_right_t, uint8_t servo_back_left_t, uint8_t servo_back_right_t, uint8_t hall_left_t, uint8_t hall_right_t, void *gpio_callback)
{
    servo_Init(servo_front_left_t);         //Servo initialization
    servo_Init(servo_front_right_t);        //Servo initialization
    servo_Init(servo_back_left_t);          //Servo initialization
    servo_Init(servo_back_right_t);         //Servo initialization

    hall_Init(hall_left_t, gpio_callback);  //Left hall sensor initialization <-> NOT USED
    hall_Init(hall_right_t, gpio_callback); //Right hall sensor initialization

    motion.adjusted_Angle  = 90.0f;         //Initial value of device yaw angle
    motion.current_Yaw     = 90.0f;         //Device Current Yaw
    
    PID_Regulator.P_Factor = 10.0f;         //PID Regulator factor initialize   <-> The Best Factor: 10.0
    PID_Regulator.I_Factor = 17.0f;         //PID Regulator factor initialize   <-> The Best Factor: 17.0  
    PID_Regulator.D_Factor = 0.90f;          //PID Regulator factor initialize  <-> The Best Factor: 1.0

    mpu_Init();
}

void distance_Update(void)
{
    if(motion.move_Direction == drive_forward)       // forward
        motion.distance += hall_distance;            // ~0.05 meter = 5cm

    else if(motion.move_Direction == drive_backward) // backward
        motion.distance -= hall_distance;            // ~0.05 meter = 5cm
    
    motion.distance_Absolute += hall_distance;       //Device absolute distance 
}

uint8_t get_Move_Direction(void)
{
    return motion.move_Direction;
}

float get_Distance(void)
{
    return motion.distance;
}

float get_Absolute_Distance(void)
{
    return motion.distance_Absolute;
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
            motion.move_Time_Stamp = time_us_32();              //time stamp
            //servo_Set_Velocity(servo_front_left, velocity_t);
            //servo_Set_Velocity(servo_front_right,velocity_t);
            //servo_Set_Velocity(servo_back_left,  velocity_t);
            //servo_Set_Velocity(servo_back_right, velocity_t);

            drive_Forward();
            motion.current_Yaw = mpu_Get_Yaw();
            motion.move_Direction = drive_forward;
        break;

        case drive_backward: //back
            motion.move_Time_Stamp = time_us_32();
            //servo_Set_Velocity(servo_front_left, -velocity_t);
            //servo_Set_Velocity(servo_front_right,-velocity_t);
            //servo_Set_Velocity(servo_back_left,  -velocity_t);
            //servo_Set_Velocity(servo_back_right, -velocity_t);

            drive_Backward();
            motion.current_Yaw = mpu_Get_Yaw();
            motion.move_Direction = drive_backward;
        break;

        case drive_left: //left
            motion.move_Time_Stamp = time_us_32();              //Metal detection necessary time stamp <-> elimination pi pico timer errors
            servo_Set_Velocity(servo_front_left, -velocity_t/4);
            servo_Set_Velocity(servo_front_right, velocity_t/5);
            servo_Set_Velocity(servo_back_left,  -velocity_t/5);
            servo_Set_Velocity(servo_back_right,  velocity_t/4);
            busy_wait_ms(40);                                   //Delay necessary for MPU6050 to get stable yaw angle
            motion.adjusted_Angle = mpu_Get_Yaw() + 10.0f;              //MPU6050 set adjusted yaw <-> necessary for PID regulator 
            motion.current_Yaw = motion.adjusted_Angle - 10.0f; //MPU6050 set adjusted yaw <-> necessary for PID regulator
            PID_Regulator.I_Segment = 0.0f;                     //Clear PID I segment accumulation error - I segment
            motion.move_Direction = drive_left;                 //Set move direction
        break;

        case drive_right: //right
            motion.move_Time_Stamp = time_us_32();              //Metal detection necessary time stamp <-> elimination pi pico timer errors
            servo_Set_Velocity(servo_front_left,  velocity_t/6);
            servo_Set_Velocity(servo_front_right,-velocity_t/5);
            servo_Set_Velocity(servo_back_left,   velocity_t/5);
            servo_Set_Velocity(servo_back_right, -velocity_t/6);
            busy_wait_ms(40);                                   //Delay necessary for MPU6050 to get stable yaw angle
            motion.adjusted_Angle = mpu_Get_Yaw() - 10.0f;              //MPU6050 set adjusted yaw <-> necessary for PID regulator 
            motion.current_Yaw = motion.adjusted_Angle + 10.0f; //MPU6050 set adjusted yaw <-> necessary for PID regulator
            PID_Regulator.I_Segment = 0.0f;                     //Clear PID I segment accumulation error - I segment
            motion.move_Direction = drive_right;                //Set move direction
        break;

        case drive_stop: //stop
            servo_Set_Velocity(servo_front_left, 0); //STOP Vehicle
            servo_Set_Velocity(servo_front_right,0); //STOP Vehicle
            servo_Set_Velocity(servo_back_left,  0); //STOP Vehicle
            servo_Set_Velocity(servo_back_right, 0); //STOP Vehicle
            motion.move_Time_Stamp = time_us_32();   //Metal detection necessary time stamp <-> elimination pi pico timer errors
            motion.move_Direction = drive_stop;      //Set move direction

        default:
            servo_Set_Velocity(servo_front_left, 0); //STOP Vehicle
            servo_Set_Velocity(servo_front_right,0); //STOP Vehicle
            servo_Set_Velocity(servo_back_left,  0); //STOP Vehicle
            servo_Set_Velocity(servo_back_right, 0); //STOP Vehicle
            motion.move_Time_Stamp = time_us_32();   //Metal detection necessary time stamp <-> elimination pi pico timer errors
            motion.move_Direction = drive_stop;      //Set move direction
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
    motion.adjusted_Angle = mpu_Get_Yaw();
}

void turn_Right()
{
    float current_Angle = mpu_Get_Yaw();
    while(mpu_Get_Yaw() >= current_Angle - 90.0f)
        move(drive_right, 250);

    move(drive_stop, 0); //STOP
    motion.adjusted_Angle = mpu_Get_Yaw();
}

void drive_Forward()
{   printf("%f\n", mpu_Get_Yaw());
    float error = motion.adjusted_Angle - motion.current_Yaw;
    PID_Regulator.P_Segment  = error * PID_Regulator.P_Factor; 
    PID_Regulator.I_Segment += error * PID_Regulator.I_Factor * PID_Elapsed_Time;

    if(PID_Regulator.I_Segment >= 250.0f)
        PID_Regulator.I_Segment = 250.0f;
    else if (PID_Regulator.I_Segment <= -250.0f)
        PID_Regulator.I_Segment = -250.0f;

    PID_Regulator.D_Segment  = ((error - PID_Regulator.D_Last_Error) / PID_Elapsed_Time) * PID_Regulator.D_Factor;
    PID_Regulator.D_Last_Error = error;
    static int32_t left_Wheels_Velocity  = 0;
    static int32_t right_Wheels_Velocity = 0;

    if(error >= 0.0f)      //Vehicle was driving too far to the right
    {   
        left_Wheels_Velocity  = round(250 - (PID_Regulator.P_Segment + PID_Regulator.I_Segment + PID_Regulator.D_Segment));
        right_Wheels_Velocity = round(250 + (PID_Regulator.P_Segment + PID_Regulator.I_Segment + PID_Regulator.D_Segment));

        servo_Set_Velocity(servo_front_left, left_Wheels_Velocity);
        servo_Set_Velocity(servo_front_right,right_Wheels_Velocity);
        servo_Set_Velocity(servo_back_left,  left_Wheels_Velocity);
        servo_Set_Velocity(servo_back_right, right_Wheels_Velocity);
    }
    else if(error <= 0.0f) //Vehicle was driving too far to the left 
    {   
        left_Wheels_Velocity  = round(250 + (-PID_Regulator.P_Segment - PID_Regulator.I_Segment - PID_Regulator.D_Segment));
        right_Wheels_Velocity = round(250 - (-PID_Regulator.P_Segment - PID_Regulator.I_Segment - PID_Regulator.D_Segment));

        servo_Set_Velocity(servo_front_left, left_Wheels_Velocity);
        servo_Set_Velocity(servo_front_right,right_Wheels_Velocity);
        servo_Set_Velocity(servo_back_left,  left_Wheels_Velocity);
        servo_Set_Velocity(servo_back_right, right_Wheels_Velocity);
    }
}

void drive_Backward()
{   printf("%f\n", mpu_Get_Yaw());
    float error = motion.adjusted_Angle - motion.current_Yaw;
    PID_Regulator.P_Segment  = error * PID_Regulator.P_Factor; 
    PID_Regulator.I_Segment += error * PID_Regulator.I_Factor * PID_Elapsed_Time;

    if(PID_Regulator.I_Segment >= 250.0f)
        PID_Regulator.I_Segment = 250.0f;
    else if (PID_Regulator.I_Segment <= -250.0f)
        PID_Regulator.I_Segment = -250.0f;

    PID_Regulator.D_Segment  = ((error - PID_Regulator.D_Last_Error) / PID_Elapsed_Time) * PID_Regulator.D_Factor;
    PID_Regulator.D_Last_Error = error;
    static int32_t left_Wheels_Velocity  = 0;
    static int32_t right_Wheels_Velocity = 0;
    
    if(error >= 0.0f)    
    {   
        left_Wheels_Velocity  = round(-250 + (-PID_Regulator.P_Segment - PID_Regulator.I_Segment - PID_Regulator.D_Segment));
        right_Wheels_Velocity = round(-250 - (-PID_Regulator.P_Segment - PID_Regulator.I_Segment - PID_Regulator.D_Segment));

        servo_Set_Velocity(servo_front_left, left_Wheels_Velocity);
        servo_Set_Velocity(servo_front_right,right_Wheels_Velocity);
        servo_Set_Velocity(servo_back_left,  left_Wheels_Velocity);
        servo_Set_Velocity(servo_back_right, right_Wheels_Velocity);
    }
    else if(error <= 0.0f) 
    {   
        left_Wheels_Velocity  = round(-250 - (PID_Regulator.P_Segment + PID_Regulator.I_Segment + PID_Regulator.D_Segment));
        right_Wheels_Velocity = round(-250 + (PID_Regulator.P_Segment + PID_Regulator.I_Segment + PID_Regulator.D_Segment));

        servo_Set_Velocity(servo_front_left, left_Wheels_Velocity);
        servo_Set_Velocity(servo_front_right,right_Wheels_Velocity);
        servo_Set_Velocity(servo_back_left,  left_Wheels_Velocity);
        servo_Set_Velocity(servo_back_right, right_Wheels_Velocity);
    }
}

uint32_t get_move_Time_Stamp(void)
{
    return motion.move_Time_Stamp;
}