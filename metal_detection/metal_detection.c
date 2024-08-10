#include "metal_detection.h"

metal_detect_data metal_detect_data_t = {0};

void metal_detect_init(uint8_t gpio_num, void *gpio_callback)
{
    gpio_init(gpio_num);
    gpio_set_dir(gpio_num, GPIO_IN);
    gpio_pull_up(gpio_num);
    gpio_set_irq_enabled_with_callback(gpio_num, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
}


void metal_detect_callback(void)
{
    static uint32_t last_detect_time = 0;
    uint32_t current_detect_time = time_us_64();
    static uint32_t elapsed_time;    

    elapsed_time = current_detect_time - last_detect_time;
    last_detect_time = current_detect_time;

    static uint a = 0;
    if(a >= 500){
        printf("elapsed time: %d\n", elapsed_time);
        a = 0;
    }
    a++;
    //printf("last time: %d\n current_time: %d", last_detect_time, current_detect_time);
    
    //TODO
    //metal_detect_data_t->detected_metal_counter++;
}