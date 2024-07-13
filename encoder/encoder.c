#include "encoder.h"


static uint32_t time = 0;

void encoder_init(uint8_t gpio_data, uint8_t gpio_clk)
{
    gpio_init(gpio_data);
    gpio_init(gpio_clk);

    gpio_set_dir(gpio_data, GPIO_IN);
    gpio_set_dir(gpio_clk, GPIO_IN);

    gpio_pull_up(gpio_data);
    gpio_pull_up(gpio_clk);    

    //gpio_set_irq_enabled_with_callback(gpio_num, GPIO_IRQ_EDGE_RISE, true, encoder_callback);

    gpio_set_irq_enabled(gpio_data, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(gpio_clk, GPIO_IRQ_EDGE_RISE, true);

    gpio_set_irq_callback((gpio_irq_callback_t) gpio_callback);
}


void gpio_callback(uint8_t gpio_data, uint8_t gpio_clk, callback_params_t* params) {
    
    if(gpio_get(encoder_0_clk) == 1 && gpio_get(encoder_0_data) == 0 && time_us_32() > time + 50000) //50 ms
        params->encoder_0_counter++;
    
    if(gpio_get(encoder_0_clk) == 0 && gpio_get(encoder_0_data) == 1 && time_us_32() > time + 50000)
        params->encoder_0_counter--;

    
    if(gpio_get(encoder_1_clk) == 1 && gpio_get(encoder_1_data) == 0 && time_us_32() > time + 50000)
        params->encoder_1_counter++;
    
    if(gpio_get(encoder_1_clk) == 0 && gpio_get(encoder_1_data) == 1 && time_us_32() > time + 50000)
        params->encoder_1_counter--;


    if(gpio_get(encoder_2_clk) == 1 && gpio_get(encoder_2_data) == 0 && time_us_32() > time + 50000)
        params->encoder_2_counter++;
    
    if(gpio_get(encoder_2_clk) == 0 && gpio_get(encoder_2_data) == 1 && time_us_32() > time + 50000)
        params->encoder_2_counter--;


    if(gpio_get(encoder_3_clk) == 1 && gpio_get(encoder_3_data) == 0 && time_us_32()> time + 50000)
        params->encoder_3_counter++;
    
    if(gpio_get(encoder_3_clk) == 0 && gpio_get(encoder_3_data) == 1 && time_us_32() > time + 50000)
        params->encoder_3_counter--;


    time = time_us_32(); 
}