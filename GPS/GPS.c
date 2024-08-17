#include "GPS.h"

GPS_t GPS = {0};
GPS_State_t GPS_State;
buffer_t nmea_Frame_Buffer = {0}; 

void GPS_Init(uint8_t rx_Gpio_t, uint8_t tx_Gpio_t)
{
    irq_set_enabled(UART1_IRQ, false);

    uart_inst_t *uart = uart_get_instance(UART_ID);
    GPS.uart = uart; 
    uart_init(uart, baudrate);
    gpio_set_function(rx_Gpio_t, GPIO_FUNC_UART);
    gpio_set_function(tx_Gpio_t, GPIO_FUNC_UART);

    uart_set_irq_enables(uart, true, false); 
    irq_set_exclusive_handler(UART1_IRQ, uart_Handler);
    irq_set_enabled(UART1_IRQ, true);  

    uint32_t *nmea_Frame_Buffer_Data = (uint32_t*)malloc(GPS_MSG_MAX_LEN * sizeof(char));
    buffer_Init(&nmea_Frame_Buffer, nmea_Frame_Buffer_Data, GPS_MSG_MAX_LEN);
}

void uart_Handler(void)
{
    while(uart_is_readable(GPS.uart))
    {
        uint32_t data = uart_getc(GPS.uart); 

        if(data == '$')
            GPS_State = GOT_FRAME_BEGINNING;
        
        if(data == '*')
        {
            GPS_State = GOT_NMEA_FRAME;
            NMEA_FRAME_PARSE();
            buffer_Clear(&nmea_Frame_Buffer);
        }

        if(GPS_State == GOT_FRAME_BEGINNING)
            buffer_Add(&nmea_Frame_Buffer, data);
    }
}

void NMEA_FRAME_PARSE(void)
{
    for(uint8_t i = 0; i < nmea_Frame_Buffer.counter ; i++)
    {  
        if(nmea_Frame_Buffer.buffer_Data[4] == (uint32_t)'M' && nmea_Frame_Buffer.buffer_Data[5] == (uint32_t)'C')
        {
            printf("received GPRMC\n");
        }
        
        /* REST OF THIS DATA ARE NOT NECESSARY FOR MY PROJECT 
        if(nmea_Frame_Buffer.buffer_Data[4] == (uint32_t)'T' && nmea_Frame_Buffer.buffer_Data[5] == (uint32_t)'G')
        {
            printf("received GPVTG\n");
        }
        if(nmea_Frame_Buffer.buffer_Data[4] == (uint32_t)'G' && nmea_Frame_Buffer.buffer_Data[5] == (uint32_t)'A')
        {
            printf("received GPGGA\n");
        }
        if(nmea_Frame_Buffer.buffer_Data[4] == (uint32_t)'S' && nmea_Frame_Buffer.buffer_Data[5] == (uint32_t)'A')
        {
            printf("received GPGSA\n");
        }
        if(nmea_Frame_Buffer.buffer_Data[4] == (uint32_t)'S' && nmea_Frame_Buffer.buffer_Data[5] == (uint32_t)'V')
        {
            printf("received GPGSV\n");
        }  
        if(nmea_Frame_Buffer.buffer_Data[4] == (uint32_t)'L' && nmea_Frame_Buffer.buffer_Data[5] == (uint32_t)'L')
        {
            printf("received GPGLL\n");
        }
        //printf("\n------=END of NMEA FRAME=------\n");
        */  
    }
}