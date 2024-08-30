#include "pico_irq.h"


void gpio_callback(uint gpio, uint32_t events)
{
    switch(gpio)
    { 
        case 16: // Right hall sensor - not used
        break;

        static uint32_t Hall_time = 0;
        case 17: //Left hall sensor
            if(time_us_32() >= Hall_time + 250) //debouncing
            { 
                Hall_time = time_us_32();
                if(get_Move_Direction() == 0 || get_Move_Direction() == 1) //Update distance only when <-> move direction == Back or move direction == Left
                {   
                    distance_Update();
                    XY_Position_Update(mpu_Get_Yaw());
                }
            }
        break;

        case 18: //metal_detection
            metal_Detect_Irq();
        break;

        default:
            printf("nothing to do");
    }
 
}