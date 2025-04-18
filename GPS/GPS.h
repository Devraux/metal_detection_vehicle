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

#define GPS_Rx_Gpio 9
#define GPS_Tx_Gpio 8
#define baud_Rate 9600
#define UART_ID 1
#define GPS_MSG_MAX_LEN 255


/// @attention
//  My project doesn't use a lot of informations received from GPS receiver device <-> I only use GPRMC frame to obtain:
// - Longitude
// - Latitude
// - Hour
// - Minute
// - Second
// To save Pi Pico energy and computations I commented some unnecessary functionalities
// Commented code is untested and containing bugs and errors <-> I t will be fixed in next updates
// Current State: COMMENTED CODE NOT FIXED

typedef enum GPS_State_t{ 
    GOT_FRAME_BEGINNING,  // got '$' char
    GOT_NMEA_FRAME,       // got full frame
}GPS_State_t;

typedef struct GPS_t{
    uart_inst_t *uart;

    uint32_t Hour;
	uint32_t Minute;
	uint32_t Second;
	//uint32_t Day;
	//uint32_t Month;
	//uint32_t Year;

    uint32_t Latitude;
	uint32_t Latitude_dec;         //fract value
	uint32_t Latitude_Direction;  
	uint32_t Longitude;
	uint32_t Longitude_dec;		   //fract value
	uint32_t Longitude_Direction;
	//double Altitude;

	//uint32_t speed_Knots;
	//uint32_t speed_Knots_dec;  //fract value

	// uint8_t Satellites_Number;
	// uint8_t Quality;    // 0 - no Fix, 1 - Fix, 2 - Dif. Fix
	// uint8_t Fix_Mode;   // 1 - no Fix, 2 - 2D, 3 - 3D
	// double Dop;         // Dilution of precision
	// double Hdop;        // Dilution of precision for flat coords
	// double Vdop;        // Dilution of precision for height

}GPS_t;


/// @brief GPS module initialization
/// @param rx_Gpio_t - rx GPIO PIN
/// @param tx_Gpio_t - tx GPIO PIN
void GPS_Init(uint8_t rx_Gpio_t, uint8_t tx_Gpio_t);

/// @brief PI PICO UART IRQ HANDLER
/// @param  -
static void uart_Handler(void);

/// @brief parse received frame and save result in GPS data structure 
/// @param -
static void NMEA_FRAME_PARSE(void);

/// @brief merge 2 digits for example: a = 1, b = 2, result = 12 
/// @param digit_1 first digit
/// @param digit_2 second digit
static uint32_t merge_2(uint32_t digit_1, uint32_t digit_2);

/// @brief merge 3 digits for example: a = 1, b = 2, c = 3, result = 123 
/// @param digit_1 first digit
/// @param digit_2 second digit
/// @param digit_3 third digit
static uint32_t merge_3(uint32_t digit_1, uint32_t digit_2, uint32_t digit_3);

/// @brief merge 4 digits for example: a = 1, b = 2, c = 3, d = 4, result = 1234 
/// @param digit_1 first digit
/// @param digit_2 second digit
/// @param digit_3 third digit
/// @param digit_4 fourth digit 
static uint32_t merge_4(uint32_t digit_1, uint32_t digit_2, uint32_t digit_3, uint32_t digit_4);

/// @brief merge 5 digits for example: a = 1, b = 2, c = 3, d = 4, e = 5 result = 12345
/// @param digit_1 first digit
/// @param digit_2 second digit
/// @param digit_3 third digit
/// @param digit_4 fourth digit 
/// @param digit_5 fifth digit
static uint32_t merge_5(uint32_t digit_1, uint32_t digit_2, uint32_t digit_3, uint32_t digit_4, uint32_t digit_5);

/// @brief convert data from ASCII to uint32_t
/// @param data input data
/// @param data_Size size of data
static void ASCII_Convert(uint32_t *data, uint32_t data_Size);

/// @brief get data received from GPS module(NEO6M) and copy it for GPS_Struct structure
/// @param GPS_Struct user GPS structure
void GPS_Get_Info(GPS_t *GPS_Struct);
#endif