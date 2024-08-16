#include "GPS.h"

GPS_t GPS = {0};
GPS_State_t GPS_State;
buffer_t nmea_Frame_Buffer = {0}; 
buffer_t tmp_Buffer = {0};

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

    char *nmea_Frame_Buffer_Data = (char*)malloc(GPS_MSG_MAX_LEN * sizeof(char));
    char *tmp_Buffer_Data = (char*)malloc(GPS_MSG_MAX_LEN * sizeof(char));

    buffer_Init(&nmea_Frame_Buffer, nmea_Frame_Buffer_Data, GPS_MSG_MAX_LEN);
    buffer_Init(&tmp_Buffer, tmp_Buffer_Data, GPS_MSG_MAX_LEN);
}

void uart_Handler(void)
{
    char buffer[GPS_MSG_MAX_LEN] = {0};
    char word_to_find[10] = "MC";

    while(uart_is_readable(GPS.uart))
    {
        char data = uart_getc(GPS.uart); 

    }

    switch(GPS_State)
    {
        case STATE_GPRMC:

            GPS_State = STATE_GPVTG;
        break;

        case STATE_GPVTG:
            GPS_State = STATE_GPGGA;
        break;

        case STATE_GPGGA:
            GPS_State =STATE_GPGSA;
        break;

        case STATE_GPGSA:
            GPS_State = STATE_GPGSV;
        break;

        case STATE_GPGSV:
            GPS_State = STATE_GPGLL;
        break;

        case STATE_GPGLL:
            GPS_State = STATE_GOT_MSG;
        break;

        case STATE_GOT_MSG:
            GPS_State = STATE_GPRMC;
        break;
        
    }
}