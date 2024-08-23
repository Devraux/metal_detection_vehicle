#include "GPS.h"

static GPS_t GPS = {0};
static GPS_State_t GPS_State;
static buffer_t nmea_Frame_Buffer = {0}; 

void GPS_Init(uint8_t rx_Gpio_t, uint8_t tx_Gpio_t)
{
    irq_set_enabled(UART1_IRQ, false);

    uart_inst_t *uart = uart_get_instance(UART_ID);
    GPS.uart = uart; 
    uart_init(uart, baud_Rate);
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
    if(nmea_Frame_Buffer.buffer_Data[4] == (uint32_t)'M' && nmea_Frame_Buffer.buffer_Data[5] == (uint32_t)'C')
    {
        ASCII_Convert(nmea_Frame_Buffer.buffer_Data, nmea_Frame_Buffer.counter);
        GPS.Hour =  (merge_2(nmea_Frame_Buffer.buffer_Data[7], nmea_Frame_Buffer.buffer_Data[8]) + 2) % 24;
        GPS.Minute = merge_2(nmea_Frame_Buffer.buffer_Data[9], nmea_Frame_Buffer.buffer_Data[10]);
        GPS.Second = merge_2(nmea_Frame_Buffer.buffer_Data[11], nmea_Frame_Buffer.buffer_Data[12]);

        GPS.Latitude = merge_4(nmea_Frame_Buffer.buffer_Data[19], nmea_Frame_Buffer.buffer_Data[20], nmea_Frame_Buffer.buffer_Data[21],nmea_Frame_Buffer.buffer_Data[22]);
        GPS.Latitude_dec = merge_4(nmea_Frame_Buffer.buffer_Data[24],nmea_Frame_Buffer.buffer_Data[25], nmea_Frame_Buffer.buffer_Data[26],nmea_Frame_Buffer.buffer_Data[27]);
        GPS.Latitude_Direction = nmea_Frame_Buffer.buffer_Data[30];

        GPS.Longitude = merge_5(nmea_Frame_Buffer.buffer_Data[32], nmea_Frame_Buffer.buffer_Data[33], nmea_Frame_Buffer.buffer_Data[34],nmea_Frame_Buffer.buffer_Data[35], nmea_Frame_Buffer.buffer_Data[36]);
        GPS.Longitude_dec = merge_4(nmea_Frame_Buffer.buffer_Data[38],nmea_Frame_Buffer.buffer_Data[39], nmea_Frame_Buffer.buffer_Data[40],nmea_Frame_Buffer.buffer_Data[41]);
        GPS.Longitude_Direction = nmea_Frame_Buffer.buffer_Data[44];
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

   
    //printf("H: %d M: %d S: %d\n", GPS.Hour, GPS.Minute, GPS.Second);
    //printf("Latitude: %d Latitude_dec: %d Latitude_direction: %c\n", GPS.Latitude, GPS.Latitude_dec, GPS.Latitude_Direction + '0');
    //printf("Longitude: %d Longitude_dec: %d Longitude_direction:%c\n", GPS.Longitude, GPS.Longitude_dec, GPS.Longitude_Direction + '0');
    //printf("\n -====================-\n");
   }

uint32_t merge_2(uint32_t digit_1, uint32_t digit_2)
{
    uint32_t result = (digit_1 << 3) + (digit_1 << 1) + digit_2;
    return result;
}

uint32_t merge_3(uint32_t digit_1, uint32_t digit_2, uint32_t digit_3)
{
    uint32_t result = 0;

    result = digit_1;
    result = (result << 3) + (result << 1) + digit_2;
    result = (result << 3) + (result << 1) + digit_3;

    return result;
}

uint32_t merge_4(uint32_t digit_1, uint32_t digit_2, uint32_t digit_3, uint32_t digit_4)
{
    uint32_t result = 0;

    result= digit_1;
    result= (result<< 3) + (result<< 1) + digit_2;
    result= (result<< 3) + (result<< 1) + digit_3;
    result= (result<< 3) + (result<< 1) + digit_4;

    return result;
}

uint32_t merge_5(uint32_t digit_1, uint32_t digit_2, uint32_t digit_3, uint32_t digit_4, uint32_t digit_5)
{    
    uint32_t result = 0;

    result = digit_1;
    result = (result << 3) + (result << 1) + digit_2;
    result = (result << 3) + (result << 1) + digit_3;
    result = (result << 3) + (result << 1) + digit_4;
    result = (result << 3) + (result << 1) + digit_5;

    return result;
}

void ASCII_Convert(uint32_t *data, uint32_t data_Size)
{
    for(uint32_t i = 0; i < data_Size; i++)
        data[i] -= '0';
}

void get_GPS_Info(GPS_t *GPS_Struct)
{
    memcpy(GPS_Struct, &GPS, sizeof(GPS_t));
}