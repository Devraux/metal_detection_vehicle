#include "main.h"


int main() 
{
    stdio_init_all();

    //hall_Init(hall_right, &gpio_callback);
    //hall_Init(hall_left, &gpio_callback);
    //servo_init(servo_front_left);
    //servo_init(servo_front_right);
    //servo_init(servo_back_left);
    //servo_init(servo_back_right);

    //udp_Init();
    //udp_Data_Send();
    //cyw43_arch_deinit();



    //metal_Detect_Init(metal_detect_gpio, &gpio_callback);
    GPS_Init(rx_Gpio,tx_Gpio);
    int num;
    int result;
    while (true) 
    {   /*
        result = scanf("%d", &num);
    
        if (result == 1)
            move(num, 250);
        else
            num = 0;

            
        */
       
    

    printf("\n");
    sleep_ms(100);
    }
    return 0;
}