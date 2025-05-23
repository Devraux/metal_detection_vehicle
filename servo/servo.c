#include "servo.h"

void servo_Init(uint8_t gpio_num)
{
    uint8_t slice_number = pwm_gpio_to_slice_num(gpio_num);

    gpio_set_function(gpio_num, GPIO_FUNC_PWM);
    gpio_set_dir(gpio_num, true);
    pwm_set_clkdiv(slice_number, 250);     //f_PWM_clk = 500 000 Hz
    pwm_set_enabled(slice_number, true);
    pwm_set_wrap(slice_number, 10000 - 1); // 10000 -> PWM_freq = 50Hz
}

void servo_Set_Velocity(uint8_t gpio_num, int16_t velocity)
{
    if(velocity == 0)
    {
        pwm_set_gpio_level(gpio_num, velocity);
        return;
    }

    if(gpio_num == 10 || gpio_num == 11)
        velocity = -velocity;
 
    velocity += 750;

    if(velocity <= 500)       //500 -> T_on = 1ms
        velocity = 500;

    else if(velocity >= 1000) //1000 -> T_on = 2ms
        velocity = 1000;
        
    pwm_set_gpio_level(gpio_num, velocity);
}