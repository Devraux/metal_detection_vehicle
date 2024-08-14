#include "pico_irq.h"


void gpio_callback(uint gpio, uint32_t events)
{  
    switch(gpio)
    {
        static uint32_t time = 0;
        static int counter = 0;
        case 16 ... 17:
            if(time_us_32() >= time + 250){ //debouncing
                distance_Update();
                counter++;
                printf("distance %f\n", get_Distance());
            }
        break;

        case 18: //metal_detection
            metal_Detect_Callback();
        break;



        default:
            printf("nothing to do");
    }
 
   //printf("hello from irq!!! \n");
}