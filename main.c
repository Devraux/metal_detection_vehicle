#include "main.h"


int main() 
{
    stdio_init_all();

    //udp_Init();
    //udp_Data_Send();
    //cyw43_arch_deinit();


    motion_Init(servo_front_left, servo_front_right, servo_back_left, servo_back_right, hall_left, hall_right, &gpio_callback);
    //metal_Detect_Init(metal_detect_gpio, &gpio_callback);
    //GPS_Init(rx_Gpio,tx_Gpio);
    int num;
    int result;
    float X, Y;
   
     
    sleep_ms(2000);
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());

    move(drive_forward, 250);
    sleep_ms(2000);
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());

    move(drive_right,250);
    sleep_ms(400);
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());

    move(drive_forward, 250);
    sleep_ms(2000);
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());   
    /*
    move(drive_left,250);
    sleep_ms(1500);
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());

    move(drive_forward, 250);
    sleep_ms(2000);
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());

    move(drive_left, 250);
    sleep_ms(1500);
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());

    move(drive_forward, 250);
    sleep_ms(2000);
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());

    move(drive_left, 250);
    sleep_ms(1500);
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());
    */
    move(4, 250); //STOP
    motion_Get_XY(&X, &Y); 
    printf("X: %f, Y: %f\n", X, Y);
    printf("Angle: %f\n", mpu_Get_Yaw());
    
    while (true) 
    {   
        //result = scanf("%d", &num);
    
        //if (result == 1)
        //    move(num, 250);
        //else
        //    move(num,0);

        //motion_Get_XY(&X, &Y);       
        
        

        //printf("X: %f, Y: %f\n", X, Y);
        //printf("Angle: %f\n", mpu_Get_Yaw());
        sleep_ms(1000);
    }
    return 0;
}