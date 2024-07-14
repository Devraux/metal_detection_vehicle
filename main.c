#include "main.h"
#include <string.h>

int main()
{ 
    stdio_init_all();

    void (*callback_1) = &encoder_callback;
    encoder_init(encoder_0_data, encoder_0_clk, callback_1);
    encoder_init(encoder_1_data, encoder_1_clk, callback_1);
    encoder_init(encoder_2_data, encoder_2_clk, callback_1);
    encoder_init(encoder_3_data, encoder_3_clk, callback_1);
struct netif_netif = netif_list;

    while (true)
    {
        printf("encoder_1: %d\n", encoder_data.encoder_0_counter);
        printf("encoder_2: %d\n", encoder_data.encoder_1_counter);
        printf("encoder_3: %d\n", encoder_data.encoder_2_counter);
        printf("encoder_4: %d\n", encoder_data.encoder_3_counter);
        sleep_ms(500);
    }

    return 0;
}
