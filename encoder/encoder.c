#include "encoder.h"

void encoder_init(uint8_t gpio_num, void (*encoder_callback)())
{
    gpio_init(gpio_num);
    gpio_set_dir(gpio_num, GPIO_IN);
    gpio_pull_up(gpio_num);    

    gpio_set_irq_enabled_with_callback(gpio_num, GPIO_IRQ_EDGE_RISE, true, encoder_callback);
}