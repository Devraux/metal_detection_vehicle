#include "pico_irq.h"

void irq_Init(void)
{
    irq_set_priority(UART0_IRQ,   3);   //UART NVIC configuration
    irq_set_priority(UART1_IRQ,   3);   //GPIO NVIC configuration

    irq_set_priority(TIMER_IRQ_0, 2);   //TIMER NVIC configuration
    irq_set_priority(TIMER_IRQ_1, 2);   //TIMER NVIC configuration
    irq_set_priority(TIMER_IRQ_2, 2);   //TIMER NVIC configuration
    irq_set_priority(TIMER_IRQ_3, 2);   //TIMER NVIC configuration

    irq_set_priority(I2C0_IRQ, 1);      //I2C NVIC configuration 
    irq_set_priority(I2C1_IRQ, 1);      //I2C NVIC configuration 

    irq_set_priority(IO_IRQ_BANK0, 0);  //GPIO NVIC configuration
    irq_set_enabled(IO_IRQ_BANK0, true);//GPIO IRQ Enable
}

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
                if(get_Move_Direction() == drive_forward || get_Move_Direction() == drive_backward) //Update distance only when <-> move direction == Back or move direction == Left
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