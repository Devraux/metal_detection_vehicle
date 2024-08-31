#include "robot_Boot_Strap.h"

static struct repeating_timer timer;
static pico_To_Server_Frame_t pico_To_Server_Data = {0};
static server_To_Pico_Frame_t server_To_Pico_Data_Buffer = {0};
static GPS_t GPS_Data; //GPS Data

static float X = 0.0f, Y = 0.0f;


void robot_Boot_Strap(void)
{
    stdio_init_all();

    //INITIALIZATION: IRQ NVIC
    irq_Init();

    //INITIALIZATION: WIFI | UDP
    pico_Wifi_Transmission_Init(SSID, PASSWORD);

    //INITIALIZATION: HALL Sensor | MPU6050 | SERVO |
    motion_Init(servo_front_left, servo_front_right, servo_back_left, servo_back_right, hall_left, hall_right, &gpio_callback);
   
    //INITIALIZATION: metal Detector
    metal_Detect_Init(metal_detect_gpio, &gpio_callback);

    //INITIALIZATION: GPS
    GPS_Init(rx_Gpio,tx_Gpio);



    add_repeating_timer_ms(-235, &period_Robot_Measurements, NULL, &timer);
}

bool period_Robot_Measurements(struct repeating_timer *timer)
{   //printf("metal_detection: %d\n",get_Metal_Detection_Status());
    GPS_Get_Info(&GPS_Data);
    pico_To_Server_Data.GPS_Latitude = GPS_Data.Latitude;
    pico_To_Server_Data.GPS_Latitude_dec =  GPS_Data.Latitude_dec;
    pico_To_Server_Data.GPS_Latitude_Direction = GPS_Data.Latitude_Direction;
    pico_To_Server_Data.GPS_Longitude =  GPS_Data.Longitude;
    pico_To_Server_Data.GPS_Longitude_dec =  GPS_Data.Longitude_dec;
    pico_To_Server_Data.GPS_Longitude_Direction =  GPS_Data.Longitude_Direction;

    pico_To_Server_Data.metal_Detection = get_Metal_Detection_Status();
    //pico_To_Server_Data[data_Frame_Counter].metal_Detection_Counter = get_Metal_Detection_Counter();

    motion_Get_XY(&X, &Y);
    pico_To_Server_Data.MPU_X = X;
    pico_To_Server_Data.MPU_X = Y;

    pico_To_Server_Data.status = 0; //everything goes good
  
    // Add received data from device to Queue and clear data structure
    queue_try_add(&queue_Pico_To_Server, &pico_To_Server_Data);
    memset(&pico_To_Server_Data, 0, sizeof(pico_To_Server_Frame_t));  //clear sended data  

    if(queue_is_full(&queue_Server_To_Pico))
    {
        queue_try_remove(&queue_Server_To_Pico, &server_To_Pico_Data_Buffer);
        move(server_To_Pico_Data_Buffer.direction, server_To_Pico_Data_Buffer.velocity);
    }
    
    return true;
}

