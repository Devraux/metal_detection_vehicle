#include "main.h"

pico_To_Server_Frame_t Data;



int main()
{
    Data.status = 1;
    Data.MPU_X = 0.0f;
    stdio_init_all();
    pico_Wifi_Transmission_Init(SSID, PASSWORD);

    while(true)
    {
        UDP_Send_Data(&Data);
        printf("data sending\n");
        sleep_ms(1000);
    }
    return 0;
}