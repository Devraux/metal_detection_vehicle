#ifndef _UDP_
#define _UDP_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "lwip/init.h"        // Dodaj ten nagłówek
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/udp.h"
#include "pico/cyw43_arch.h"

#define UDP_port 4444
//#define server_ip "192.168.1.1"

typedef struct __attribute__((packed)) {
    bool status;                       //Pi pico status -> 1-Active, 2-inactive
    
    float MPU_X;                       //MPU gyroscope calculated X current coordinate
    float MPU_Y;                       //MPU gyroscope calculated Y current coordinate

    uint32_t GPS_Latitude;             //GPS current Latitude
	uint32_t GPS_Latitude_dec;         //GPS current Latitude fract value
	uint32_t GPS_Latitude_Direction;   //GPS current Latitude Direction
	uint32_t GPS_Longitude;            //GPS current Longitude
	uint32_t GPS_Longitude_dec;		   //GPS current Longitude fract value
	uint32_t GPS_Longitude_Direction;  //GPS current Longitude Direction
 
    bool metal_Detection;              //1 -> metal detected 0 otherwise
    uint32_t metal_Detection_Counter;  //metal Detection Counter 
}pico_To_Server_Frame_t; 

typedef struct __attribute__((packed)){
    bool status;                       //Server status -> 1-Active, 2-inactive
    uint8_t direction;                 //Vehicle direction command
    int16_t velocity;                  //Vehicle velocity command
}server_To_Pico_Frame_t;

void UDP_Init(struct udp_pcb **pcb, uint16_t port, void (*recv_callback)(void *, struct udp_pcb *, struct pbuf *, const ip_addr_t *, u16_t));

void UDP_Send_Data(struct udp_pcb *pcb ,const ip_addr_t *server_Ip, uint16_t port, const pico_To_Server_Frame_t *frame);

/// @brief 
/// @param port
/// @param arg additional data - not used
/// @param pcb udp configuration structure
/// @param p received data
/// @param addr data sender IP address
/// @param port data sender port
void UDP_Receive_Callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

void core_1_entry();

void core_1_entry();

void wifi_Init(void);

void pico_Wifi_Transmission_Init(void);
#endif

