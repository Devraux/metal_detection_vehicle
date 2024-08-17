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
    if(nmea_Frame_Buffer.buffer_Data[4] == (uint32_t)'M' && nmea_Frame_Buffer.buffer_Data[5] == (uint32_t)'C')
    {

        //for(uint8_t j = 0; j < nmea_Frame_Buffer.counter; j++)
            printf("%c", nmea_Frame_Buffer.buffer_Data[1]);
        printf("\n");

        //char* parse_Pointer = strtoke((char*)nmea_Frame_Buffer.buffer_Data, ",");
                //GPRMC header -> unnecessary
                // -- DO NOTHING -- //

        //parse_Pointer = strtoke(NULL, ",");
            //char hourString[3] = {parse_Pointer[0], parse_Pointer[1], '\0'};
            //char minuteString[3] = {parse_Pointer[2], parse_Pointer[3], '\0'};
            //char secondString[3] = {parse_Pointer[4], parse_Pointer[5], '\0'};
            //printf("%c\n", parse_Pointer);
            //GPS.Hour = atoi(hourString);
            //GPS.Minute = atoi(minuteString);
            //GPS.Second = atoi(secondString);

        //parse_Pointer = strtok(NULL, ","); // received data status -> unnecessary
                // -- DO NOTHING -- //  
       
        //parse_Pointer = strtok(NULL, ",");

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
    //printf("H: %d, M: %d, S: %d\n", GPS.Hour, GPS.Minute, GPS.Second);
}

char* strtoke(char *str, const char *delim)
{
  static char *start = NULL; /* stores string str for consecutive calls */
  char *token = NULL; /* found token */
  /* assign new start in case */
  if (str) start = str;
  /* check whether text to parse left */
  if (!start) return NULL;
  /* remember current start as found token */
  token = start;
  /* find next occurrence of delim */
  start = strpbrk(start, delim);
  /* replace delim with terminator and move start to follower */
  if (start) *start++ = '\0';
  /* done */
  return token;
}



//connect 2 values
/*
uint32_t a[2] = {3, 4}; 
    uint32_t result;

    result = (a[0] << 3) + (a[0] << 1) + a[1];

    printf("Result: %d\n", result);
*/

//connect 4 values
/*
int main() {
    unsigned char a[4] = {1, 2, 3, 4};  // Przykładowe wartości
    unsigned int b = 0;

    // Dodawanie wartości do zmiennej b przy użyciu przesunięć i dodawania
    b = a[0];
    
    // Dodaj a[1] na miejsce setek
    b = (b << 3) + (b << 1) + a[1];
    
    // Dodaj a[2] na miejsce dziesiątek
    b = (b << 3) + (b << 1) + a[2];
    
    // Dodaj a[3] na miejsce jedności
    b = (b << 3) + (b << 1) + a[3];

    // Wyświetlenie wyniku
    printf("Wynik: %u\n", b);

    return 0;
}
*/