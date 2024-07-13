#include "encoder.h"


static uint32_t time = 0;
encoder_data_t encoder_data = {0};

void encoder_init(uint8_t gpio_data, uint8_t gpio_clk, void (*callback)())
{
    gpio_init(gpio_data);
    gpio_init(gpio_clk);

    gpio_set_dir(gpio_data, GPIO_IN);
    gpio_set_dir(gpio_clk, GPIO_IN);

    gpio_pull_up(gpio_data);
    gpio_pull_up(gpio_clk);    

    gpio_set_irq_enabled_with_callback(gpio_clk, GPIO_IRQ_EDGE_RISE, true, callback);
    gpio_set_irq_enabled_with_callback(gpio_data, GPIO_IRQ_EDGE_RISE, true, callback);
}


void encoder_callback() 
{
    uint32_t  present_time = time_us_32();

    if(gpio_get(encoder_0_clk) == 0)
        encoder_data.encoder_0_counter++;
    if(gpio_get(encoder_0_data) == 0)
        encoder_data.encoder_0_counter--;


    
    if(gpio_get(encoder_1_clk) == 0)
        encoder_data.encoder_1_counter++;
    if(gpio_get(encoder_1_data) == 0)
        encoder_data.encoder_1_counter--;


    if(gpio_get(encoder_2_clk) == 1 && gpio_get(encoder_2_data) == 0 && time_us_32() > time + 50000)
        encoder_data.encoder_2_counter++;
    
    if(gpio_get(encoder_2_clk) == 0 && gpio_get(encoder_2_data) == 1 && time_us_32() > time + 50000)
        encoder_data.encoder_2_counter--;


    if(gpio_get(encoder_3_clk) == 0)
        encoder_data.encoder_3_counter++;
    if(gpio_get(encoder_3_data) == 0)
        encoder_data.encoder_3_counter--;
 

    
}