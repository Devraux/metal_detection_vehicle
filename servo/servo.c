#include "servo.h"

void servo_init(uint8_t gpio_num)
{
    uint8_t slice_number = pwm_gpio_to_slice_num(gpio_num);

    gpio_set_function(gpio_num, GPIO_FUNC_PWM);
    pwm_set_clkdiv(slice_number, 250);
    pwm_set_enabled(slice_number, true);
    pwm_set_wrap(slice_number, 10000); // 10000 -> pwm_freq = 50Hz
}

void servo_set_velocity(uint8_t gpio_num, int16_t velocity)
{
    velocity += 750;
    
    if(velocity <= 505)
        velocity = 505;

    else if(velocity >= 995)
        velocity = 995;

    pwm_set_gpio_level(gpio_num, velocity);
}