#ifndef _UDP_
#define _UDP_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "lwip/init.h"        
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/udp.h"
#include "pico/cyw43_arch.h"
#include "pico/util/queue.h"

#define UDP_port 4444
#define server_IP "192.168.137.1"
#define SSID "krzysztofplonka"
#define PASSWORD "82K67?0f"

typedef struct __attribute__((packed)) {
    bool status;                       //Pi pico status -> 0-Active, 1-inactive
    
    float MPU_X;                       //MPU gyroscope calculated X current coordinate
    float MPU_Y;                       //MPU gyroscope calculated Y current coordinate

    uint32_t GPS_Latitude;             //GPS current Latitude
	uint32_t GPS_Latitude_dec;         //GPS current Latitude fract value
	uint32_t GPS_Latitude_Direction;   //GPS current Latitude Direction
	uint32_t GPS_Longitude;            //GPS current Longitude
	uint32_t GPS_Longitude_dec;		   //GPS current Longitude fract value
	uint32_t GPS_Longitude_Direction;  //GPS current Longitude Direction
 
    bool metal_Detection;              //1 -> metal detected 0 otherwise
    //uint32_t metal_Detection_Counter;  //metal Detection Counter 
}pico_To_Server_Frame_t; 

typedef struct __attribute__((packed)){
    bool status;                       //Server status -> 0-Active, 1-inactive
    uint8_t direction;                 //Vehicle direction command
    int16_t velocity;                  //Vehicle velocity command
}server_To_Pico_Frame_t;

extern queue_t queue;

/// @brief UDP receive initialization
/// @param pcb UDP configuration structure
/// @param recv_callback callback function called whenever data is received
void UDP_Receive_Init(void (*recv_callback)(void *, struct udp_pcb *, struct pbuf *, const ip_addr_t *, u16_t));

/// @brief UDP send data 
/// @param server_Ip server ip 
/// @param frame data frame to send
void UDP_Send_Data(const pico_To_Server_Frame_t *frame);

/// @brief callback function called whenever data is received
/// @param port UDP port
/// @param arg additional data - not used
/// @param pcb udp configuration structure
/// @param p received data
/// @param addr data sender IP address
/// @param port data sender port
void UDP_Receive_Callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

/// @brief pico hardware(like cyw43) initialization
/// @param ssid SSID
/// @param password PASSWORD
void pico_Hardware_wifi_Init(const char* ssid, const char* password);

/// @brief core 1 entry
/// @param - 
void core_1_Entry(void);

/// @brief pi pico wifi transmission initialization
/// @param -  
void pico_Wifi_Transmission_Init(const char* ssid, const char* password);

void UDP_Queue_init(uint32_t queue_Size);

void print_Ip_Address();
#endif

