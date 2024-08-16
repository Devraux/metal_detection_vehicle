#ifndef _GPS_
#define _GPS_

#include <stdint.h>  
#include <stdio.h>
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

    char data[GPS_MSG_MAX_LEN];

    uint8_t Hour;
	uint8_t Minute;
	uint8_t Second;
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;

    double Latitude;
	char Latitude_Direction;
	double Longitude;
	char Longitude_Direction;
	double Altitude;

	double Speed_Knots;
	double Speed_Kilometers;

	uint8_t Satellites_Number;
	uint8_t Quality;    // 0 - no Fix, 1 - Fix, 2 - Dif. Fix
	uint8_t Fix_Mode;    // 1 - no Fiz, 2 - 2D, 3 - 3D
	double Dop;         // Dilution of precision
	double Hdop;        // Dilution of precision for flat coords
	double Vdop;        // Dilution of precision for height

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
#endif