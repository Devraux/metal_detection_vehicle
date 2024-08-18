#ifndef _GPS_
#define _GPS_

#include <stdint.h>  
#include <stdio.h>
#include <stdlib.h>
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "string.h"
#include "buffer/buffer.h"

#define rx_Gpio 9
#define tx_Gpio 8
#define baudrate 9600
#define UART_ID 1
#define GPS_MSG_MAX_LEN 255

typedef enum GPS_State_t{ 
    GOT_FRAME_BEGINNING,  // got '$' char
    GOT_NMEA_FRAME,       // got full frame
}GPS_State_t;

typedef struct GPS_t{
    uart_inst_t *uart;

    uint32_t Hour;
	uint32_t Minute;
	uint32_t Second;
	uint32_t Day;
	uint32_t Month;
	uint32_t Year;

    double Latitude;
	char Latitude_Direction;
	double Longitude;
	char Longitude_Direction;
	double Altitude;

	double Speed_Knots;
	double Speed_Kilometers;

	// uint8_t Satellites_Number;
	// uint8_t Quality;    // 0 - no Fix, 1 - Fix, 2 - Dif. Fix
	// uint8_t Fix_Mode;    // 1 - no Fiz, 2 - 2D, 3 - 3D
	// double Dop;         // Dilution of precision
	// double Hdop;        // Dilution of precision for flat coords
	// double Vdop;        // Dilution of precision for height

}GPS_t;

extern GPS_t GPS;

/// @brief GPS module initialization
/// @param rx_Gpio_t - rx GPIO PIN
/// @param tx_Gpio_t - tx GPIO PIN
void GPS_Init(uint8_t rx_Gpio_t, uint8_t tx_Gpio_t);

/// @brief TODO
/// @param GPS 
void GPS_read(GPS_t *GPS);

/// @brief PI PICO UART IRQ HANDLER
/// @param  -
void uart_Handler(void);

/// @brief parse received frame and save result in GPS data structure 
/// @param -
void NMEA_FRAME_PARSE(void);

/// @brief modifications of c stroke function
/// @param str string
/// @param delim delimeter 
char* strtoke(char *str, const char *delim);

/// @brief merge 2 digits for example: a = 1, b = 2, result = 12 
/// @param digit_1 first digit
/// @param digit_2 second digit
uint32_t merge_2(uint32_t digit_1, uint32_t digit_2);

/// @brief merge 4 digits for example: a = 1, b = 2, c = 3, d = 4, result = 1234 
/// @param digit_1 first digit
/// @param digit_2 second digit
/// @param digit_3 third digit
/// @param digit_4 fourth digit 
uint32_t merge_4(uint32_t digit_1, uint32_t digit_2, uint32_t digit_3, uint32_t digit_4);

/// @brief convert data from ASCII to uint32_t
/// @param data input data
/// @param data_Size size of data
void ASCII_Convert(uint32_t *data, uint32_t data_Size);
#endif