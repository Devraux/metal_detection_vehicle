#include "metal_detection.h"

static metal_detect_data_t metal_detect_data = {0};
static buffer_t buffer;
static struct repeating_timer timer;

void metal_Detect_Init(uint8_t gpio_num_t, void *gpio_callback)
{
    gpio_init(gpio_num_t);
    gpio_set_dir(gpio_num_t, GPIO_IN);
    gpio_pull_up(gpio_num_t);
    gpio_set_irq_enabled_with_callback(gpio_num_t, GPIO_IRQ_EDGE_FALL, true, gpio_callback);

    uint32_t *buffer_data = (uint32_t*)malloc(450 * sizeof(uint32_t)); // redundancy - 450 samples
    buffer_Init(&buffer, buffer_data, 450);   
    add_repeating_timer_ms(-215, compute_Detections_Data, NULL, &timer);  // ~5 times per second
}

void metal_Detect_Irq(void)
{
    static uint32_t previous_edge_time = 0;
    uint32_t current_edge_time = time_us_32();
    uint32_t elapsed_time = current_edge_time - previous_edge_time;
    previous_edge_time = current_edge_time;

    buffer_Add(&buffer, elapsed_time);  
}

static bool compute_Detections_Data(struct repeating_timer *timer)
{
    uint32_t sum = 0;
    for(uint32_t i = 0; i < buffer.buffer_Size; i++)
        sum += buffer.buffer_Data[i];

    metal_detect_data.detection_Average = sum / buffer.counter;
    check_Metal_Detect();
    buffer_Clear(&buffer);
    return true;
}

static void check_Metal_Detect(void)
{
    static uint32_t metal_detection_avg_t = 0; 
    //ATTENTION CODE SHOULD BE TESTED BEGINNING THIS MOMENT
    if(metal_detection_avg_t >= metal_detect_data.detection_Average + 2 || metal_detection_avg_t <= metal_detect_data.detection_Average - 2)
    {
        metal_detect_data.metal_Detected = true;
        //metal_detect_data.detected_Metal_Counter++;
    }

    else        
        metal_detect_data.metal_Detected = false;

    metal_detection_avg_t = metal_detect_data.detection_Average; 

}

bool get_Metal_Detection_Status(void)
{
    return metal_detect_data.metal_Detected;
}

uint32_t get_Metal_Detection_Counter(void)
{
    return metal_detect_data.detected_Metal_Counter;
}