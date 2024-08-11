# include "motion.h"

motion_t motion = {0};

void hall_Init(uint8_t gpio_num, void *gpio_callback)
{   
    gpio_init(gpio_num);
    gpio_set_dir(gpio_num, GPIO_IN);
    gpio_set_irq_enabled_with_callback(gpio_num, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
}

void move_Init(uint8_t servo_1_gpio_t, uint8_t servo_2_gpio_t, uint8_t servo_3_gpio_t, uint8_t servo_4_gpio_t, uint8_t hall_1_gpio_t, uint8_t hall_2_gpio_t, void *gpio_callback)
{
    servo_init(servo_1_gpio);
    servo_init(servo_2_gpio);
    servo_init(servo_3_gpio);
    servo_init(servo_4_gpio);

    hall_Init(hall_1_gpio, gpio_callback);
    hall_Init(hall_2_gpio, gpio_callback);
}

void distance_Update()
{
    
}

uint32_t get_Distance(void)
{
    return motion.distance;
}
