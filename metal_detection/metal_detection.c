#include "metal_detection.h"

metal_detect_data metal_detect_data_t = {0};

void metal_detect_init(uint8_t gpio_num_t, void *gpio_callback)
{
    gpio_init(gpio_num_t);
    gpio_set_dir(gpio_num_t, GPIO_IN);
    gpio_pull_up(gpio_num_t);
    gpio_set_irq_enabled_with_callback(gpio_num_t, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
}

void metal_detect_callback(void)
{
    static uint32_t previous_edge_time = 0;
    uint32_t current_edge_time = time_us_32();
    uint32_t elapsed_time = current_edge_time - previous_edge_time;
    previous_edge_time = current_edge_time;

    












    static uint a = 0;
    if(a >= 1700){
        printf("elapsed time: %d\n", elapsed_time);
        a = 0;
    }
    a++;
    
}