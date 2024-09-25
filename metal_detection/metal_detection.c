#include "metal_detection.h"

metal_detect_data_t metal_detect_data = {0};
static buffer_t buffer;
static struct repeating_timer timer;
static uint32_t last_Detection_ABS_Distance;

void metal_Detect_Init(uint8_t gpio_num_t, void *gpio_callback)
{
    gpio_init(gpio_num_t);
    gpio_set_dir(gpio_num_t, GPIO_IN);
    gpio_pull_up(gpio_num_t);
    gpio_set_irq_enabled_with_callback(gpio_num_t, GPIO_IRQ_EDGE_FALL, true, gpio_callback);

    add_repeating_timer_ms(-154, compute_Detections_Data, NULL, &timer);    // ~5 times per second
}

void metal_Detect_Irq(void)
{   
    metal_detect_data.current_Edge_Counter++;
}

static bool compute_Detections_Data(struct repeating_timer *timer)
{
    gpio_set_irq_enabled(metal_detect_gpio, GPIO_IRQ_EDGE_FALL, false);

    if(metal_detect_data.metal_Detection_Startup == false)
    {
        metal_detect_data.metal_Detection_Startup = true;
        return true;
    }

    if(((metal_detect_data.current_Edge_Counter >= metal_detect_data.previous_Edge_Counter + 2) || (metal_detect_data.current_Edge_Counter <= metal_detect_data.previous_Edge_Counter - 2)) && (get_Absolute_Distance() ))
    {    
        last_Detection_ABS_Distance = get_Absolute_Distance();
        metal_detect_data.metal_Detected = true;
    }
    else 
        metal_detect_data.metal_Detected = false;

    if(metal_detect_data.previous_Edge_Counter == 0)
        metal_detect_data.metal_Detected = false;

    if(last_Detection_ABS_Distance == get_Absolute_Distance())
        metal_detect_data.metal_Detected = false;

    //printf("Previous Counter: %d, Curent counter: %d\n",metal_detect_data.previous_Edge_Counter, metal_detect_data.current_Edge_Counter);
    metal_detect_data.previous_Edge_Counter = metal_detect_data.current_Edge_Counter;
    metal_detect_data.current_Edge_Counter = 0;  
    gpio_set_irq_enabled(metal_detect_gpio, GPIO_IRQ_EDGE_FALL, true);

    return true;
}

bool get_Metal_Detection_Status(void)
{
    return metal_detect_data.metal_Detected;
}

uint32_t get_Metal_Detection_Counter(void)
{
    return metal_detect_data.detected_Metal_Counter;
}

void disable_Metal_Detection(void)
{
    cancel_repeating_timer(&timer);
    gpio_set_irq_enabled(metal_detect_gpio, GPIO_IRQ_EDGE_FALL, false);
    metal_detect_data.current_Edge_Counter = 0;
    metal_detect_data.previous_Edge_Counter = 0;
}

void enable_Metal_Detection(void)
{
    add_repeating_timer_ms(-154, compute_Detections_Data, NULL, &timer); //@attention this method needs improvement
    gpio_set_irq_enabled(metal_detect_gpio, GPIO_IRQ_EDGE_FALL, true);
}

void reset_metal_Detection(void)
{
    metal_detect_data.metal_Detection_Startup = false;
}