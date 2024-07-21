#ifndef _UDP_
#define _UDP_

#include <string.h>
#include "pico/stdlib.h"

#include "pico/multicore.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"


#define UDP_PORT 4444
#define BEACON_MSG_LEN_MAX 127
#define BEACON_TARGET "192.168.0.101"
#define BEACON_INTERVAL_MS 1000
#define WIFI_SSID "204A"
#define WIFI_PASSWORD "omegatronic"


/// @brief udp
/// @return 
uint8_t udp_Init();
void udp_Print_Ip();
void udp_Data_Send();

#endif