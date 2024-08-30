#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include <stdlib.h>

#define UDP_PORT 4444
#define DEST_IP "192.168.137.1"  // Zmień na adres IP odbiorcy

typedef struct __attribute__((packed)) {
    bool status;     // Przykładowy status
    float random_x;  // Losowa wartość X
    float random_y;  // Losowa wartość Y
    uint32_t counter;  // Licznik wysłanych pakietów
} pico_To_Server_Frame_t;

void send_udp_data(struct udp_pcb *pcb, const ip_addr_t *dest_ip, uint16_t port) {
    static uint32_t counter = 0;
    pico_To_Server_Frame_t frame = {0};
    
    // Wypełnianie ramki losowymi danymi
    frame.status = true;
    frame.random_x = (float)rand() / RAND_MAX * 100.0f;
    frame.random_y = (float)rand() / RAND_MAX * 100.0f;
    frame.counter = counter++;

    // Alokowanie bufora UDP
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, sizeof(frame), PBUF_RAM);
    if (p != NULL) {
        memcpy(p->payload, &frame, sizeof(frame));
        udp_sendto(pcb, p, dest_ip, port);
        pbuf_free(p);
    } else {
        printf("Failed to allocate pbuf\n");
    }
}

int main() {
    stdio_init_all();
    printf("Initializing WiFi...\n");

    if (cyw43_arch_init()) {
        printf("CYW43 initialization failed\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    int result = cyw43_arch_wifi_connect_timeout_ms("krzysztofplonka", "82K67?0f", CYW43_AUTH_WPA2_AES_PSK, 10000);
    if (result != 0) {
        printf("WiFi connection failed with error code: %d\n", result);
        return 1;
    }

    ip_addr_t dest_ip;
    ip4addr_aton(DEST_IP, &dest_ip);

    // Inicjalizacja struktury UDP
    struct udp_pcb *pcb = udp_new();
    if (!pcb) {
        printf("Failed to create UDP PCB\n");
        return 1;
    }

    while (true) {
        send_udp_data(pcb, &dest_ip, UDP_PORT);
        printf("Sent UDP data\n");
        sleep_ms(1000);
    }

    udp_remove(pcb);
    cyw43_arch_deinit();
    return 0;
}
