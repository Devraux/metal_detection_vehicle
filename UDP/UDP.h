#ifndef _UDP_
#define _UDP_

#include <string.h>

#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"


#define UDP_PORT 4444
#define BEACON_MSG_LEN_MAX 127
#define BEACON_TARGET "255.255.255.255"
#define BEACON_INTERVAL_MS 1000
#define WIFI_SSID '204A'
#define WIFI_PASSWORD 'omegatronic'

void print_ip_address();
void run_udp_beacon();

#endif