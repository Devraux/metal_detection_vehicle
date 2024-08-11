#include "pico_irq.h"


void gpio_callback(uint gpio, uint32_t events)
{   
    switch(gpio)
    {
        static uint32_t time = 0;
        
        case 16 ... 17:
            if(time_us_32() >= time + 250) //debouncing
                distance_Update();
            time = time_us_32();
        break;



        default:
            printf("nothing to do");
    }
 
   //printf("hello from irq!!! \n");
}