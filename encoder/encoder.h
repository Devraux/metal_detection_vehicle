#ifndef _encoder_
#define _encoder_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "hardware/gpio.h"
#include "hardware/rtc.h"
#include "hardware/timer.h"


#define encoder_0_data 0
#define encoder_0_clk  1
#define encoder_1_data 2
#define encoder_1_clk  3
#define encoder_2_data 4
#define encoder_2_clk  5
#define encoder_3_data 6
#define encoder_3_clk  7 


typedef struct{
    int16_t encoder_0_counter;
    int16_t encoder_1_counter;
    int16_t encoder_2_counter;
    int16_t encoder_3_counter;
}callback_params_t;

void encoder_init(uint8_t gpio_data, uint8_t gpio_clk);

void gpio_callback(uint8_t gpio_data, uint8_t gpio_clk, callback_params_t* params);


#endif