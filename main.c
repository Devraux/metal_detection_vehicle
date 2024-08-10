#include "main.h"



int main() 
{
    stdio_init_all();

    metal_detect_init(14, gpio_callback);
    metal_detect_init(15, gpio_callback);
 
    
    //udp_Init();
    //udp_Data_Send();
    //cyw43_arch_deinit();


    while(1)
    {
        printf("still running...\n");
        sleep_ms(1000);
    }

    return 0;
}