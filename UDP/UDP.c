#include "UDP.h"

static server_To_Pico_Frame_t server_TO_Pico_Data = {0};
static pico_To_Server_Frame_t pico_To_Server_Data = {0};
static struct udp_pcb* send_pcb = NULL;
static struct udp_pcb* receive_pcb = NULL;

void UDP_Receive_Init(uint16_t port, void (*recv_callback)(void *, struct udp_pcb *, struct pbuf *, const ip_addr_t *, u16_t)) 
{
    if (receive_pcb == NULL)
    {
        receive_pcb = udp_new();
        
        if (receive_pcb != NULL)
        {
            err_t err = udp_bind(receive_pcb, IP_ADDR_ANY, port);
            if (err == ERR_OK) 
                udp_recv(receive_pcb, UDP_Receive_Callback, NULL);
            else 
                printf("Failed to bind UDP PCB\n");
        }
        else 
            printf("Failed to create UDP PCB\n");
    }
}

void UDP_Send_Data(const ip_addr_t *server_Ip, uint16_t port, const pico_To_Server_Frame_t *frame)
{   
    if (send_pcb == NULL) 
        send_pcb = udp_new();
    
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, sizeof(pico_To_Server_Frame_t), PBUF_RAM);

    if(p != NULL)
    {
        memcpy(p->payload, frame, sizeof(pico_To_Server_Frame_t));
        err_t err = udp_sendto(send_pcb, p, server_Ip, port);
        if (err != ERR_OK) 
            printf("Failed to send UDP packet\n");
        pbuf_free(p);
    }
}   

void UDP_Receive_Callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    if (p != NULL)
    {
        //server_To_Pico_Frame_t *frame = (server_To_Pico_Frame_t *)p->payload;
        
        //server_TO_Pico_Data.status = frame->status;       //Data copy
        //server_TO_Pico_Data.direction = frame->direction; //Data copy
        //server_TO_Pico_Data.velocity = frame->velocity;   //Data copy
        
        server_To_Pico_Frame_t received_data;
        memcpy(&received_data, p->payload, sizeof(server_To_Pico_Frame_t));

        pbuf_free(p);
    }
}

void pico_Hardware_wifi_Init(const char* ssid, const char* password)
{
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 10000) != 0) {
        printf("WiFi connection failed!\n");
        return;
    }

    lwip_init();
}

void core_1_Entry(void)
{
    UDP_Receive_Init(UDP_port, UDP_Receive_Callback);

     while (true) 
        tight_loop_contents();
}

void pico_Wifi_Transmission_Init()
{
    pico_Hardware_wifi_Init("SSID", "PASSWORD");
    multicore_launch_core1(core_1_Entry);
}