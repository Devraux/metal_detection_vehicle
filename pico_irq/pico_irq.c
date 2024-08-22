#include "pico_irq.h"


void gpio_callback(uint gpio, uint32_t events)
{  
    switch(gpio)
    {
        static uint32_t Hall_time = 0;
        case 16 ... 17:
            if(time_us_32() >= Hall_time + 250) //debouncing
            { 
                Hall_time = time_us_32();
                
                if(get_Move_Direction() == 0 && get_Move_Direction() == 1) // if move direction == Back or move direction == Left
                    distance_Update();
            }
        break;

        case 18: //metal_detection
            metal_Detect_Callback();
        break;

        default:
            printf("nothing to do");
    }
 
}