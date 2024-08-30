#include "robot_Boot_Strap.h"

static struct repeating_timer timer;
static pico_To_Server_Frame_t pico_To_Servo_Data[30] = {0};
static GPS_t GPS_Data; //GPS Data

static float X = 0.0f, Y = 0.0f;
static uint32_t data_Frame_Counter; //frames waiting for send using WIFI


void robot_Boot_Strap(void)
{
    //INITIALIZATION: HALL Sensor | MPU6050 | SERVO |
    motion_Init(servo_front_left, servo_front_right, servo_back_left, servo_back_right, hall_left, hall_right, &gpio_callback);
   
    //INITIALIZATION: metal Detector
    metal_Detect_Init(metal_detect_gpio, &gpio_callback);

    //INITIALIZATION: GPS
    GPS_Init(rx_Gpio,tx_Gpio);

    add_repeating_timer_ms(-235, &period_Robot_Measurements, NULL, NULL);
}

bool period_Robot_Measurements(struct repeating_timer *timer)
{
    GPS_Get_Info(&GPS_Data);
    pico_To_Servo_Data[data_Frame_Counter].GPS_Latitude = GPS_Data.Latitude;
    pico_To_Servo_Data[data_Frame_Counter].GPS_Latitude_dec =  GPS_Data.Latitude_dec;
    pico_To_Servo_Data[data_Frame_Counter].GPS_Latitude_Direction = GPS_Data.Latitude_Direction;
    pico_To_Servo_Data[data_Frame_Counter].GPS_Longitude =  GPS_Data.Longitude;
    pico_To_Servo_Data[data_Frame_Counter].GPS_Longitude_dec =  GPS_Data.Longitude_dec;
    pico_To_Servo_Data[data_Frame_Counter].GPS_Longitude_Direction =  GPS_Data.Longitude_Direction;

    pico_To_Servo_Data[data_Frame_Counter].metal_Detection = get_Metal_Detection_Status();
    pico_To_Servo_Data[data_Frame_Counter].metal_Detection_Counter = get_Metal_Detection_Counter();

    motion_Get_XY(&X, &Y);
    pico_To_Servo_Data[data_Frame_Counter].MPU_X = X;
    pico_To_Servo_Data[data_Frame_Counter].MPU_X = Y;

    pico_To_Servo_Data[data_Frame_Counter].status = 0; //everything goes good
  
    data_Frame_Counter++;


    if(data_Frame_Counter >= 8)
    {
        data_Send();
        data_Frame_Counter = 0;
    } 


    return true;
}

void data_Send(void)
{

}