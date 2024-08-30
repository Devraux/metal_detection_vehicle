#include "main.h"

pico_To_Server_Frame_t Data;



int main()
{
    robot_Boot_Strap();
    
    while(true)
    {
        sleep_ms(1000);
    }
    return 0;
}