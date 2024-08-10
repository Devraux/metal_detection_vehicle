#include "pico_irq.h"


void gpio_callback(uint gpio, uint32_t events)
{   
    printf("hello form gpio_callback irq func\n");
    switch(gpio)
    {
        case 14:
           // metal_detect_callback();
            printf("button 14 pressed\n");
        break;

        case 15:
           // metal_detect_callback();
           printf("button 15 pressed\n");
        break;

        default:
            printf("nothing to do");
    }
 
   //printf("hello from irq!!! \n");
}