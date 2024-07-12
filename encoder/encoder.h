#ifndef _encoder_
#define _encoder_

#include <stdint.h>
#include <hardware/gpio.h>
#include <stdbool.h>

#define encoder_1 1
#define encoder_2 2
#define encoder_3 3
#define encoder_4 4


void encoder_init(uint8_t gpio_num, void (*encoder_callback)());

void encoder_callback(void);


#endif