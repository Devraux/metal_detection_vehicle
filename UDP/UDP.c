#include "UDP.h"

static server_To_Pico_Frame_t server_TO_Pico_Data = {0};
static pico_To_Server_Frame_t pico_To_Server_Data = {0};
struct udp_pcb *udp_pcb_core_0;
struct udp_pcb *udp_pcb_core_1;

void UDP_Init(struct udp_pcb **pcb, uint16_t port, void (*recv_callback)(void *, struct udp_pcb *, struct pbuf *, const ip_addr_t *, u16_t)) 
{
    *pcb = udp_new();
    
    if(*pcb != NULL) 
    {
        err_t err = udp_bind(*pcb, IP_ADDR_ANY, port);
        if (err == ERR_OK) 
            udp_recv(*pcb, recv_callback, NULL);
        else 
            printf("Failed to bind UDP PCB\n");
        
    } 

    else
        printf("Failed to create UDP PCB\n");
}

void UDP_Send_Data(struct udp_pcb *pcb ,const ip_addr_t *server_Ip, uint16_t port, const pico_To_Server_Frame_t *frame)
{
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, sizeof(pico_To_Server_Frame_t), PBUF_RAM);
        if(p != NULL)
        {
            memcpy(p->payload, frame, sizeof(pico_To_Server_Frame_t));
            err_t err = udp_sendto(pcb, p, server_Ip, port);

            if (err != ERR_OK) 
                printf("Failed to send UDP packet\n");

            pbuf_free(p);
        }
}   

void UDP_Receive_Callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    if (p != NULL)
    {
        server_To_Pico_Frame_t *frame = (server_To_Pico_Frame_t *)p->payload;
        
        server_TO_Pico_Data.status = frame->status;       //Data copy
        server_TO_Pico_Data.direction = frame->direction; //Data copy
        server_TO_Pico_Data.velocity = frame->velocity;   //Data copy
        
        pbuf_free(p);
    }
}

void pico_Hardware_wifi_Init()
{
    if (cyw43_arch_init())
    {
        printf("Wi-Fi initialization failed\n");
        return;
    }

    // LWIP initialization
    lwip_init();

}

void UDP_Core_0_Init(void)
{
    ip_addr_t server_ip;
    UDP_Init(&udp_pcb_core_0, UDP_port, NULL); 

    IP4_ADDR(&server_ip, 192, 168, 1, 1); 
}

void UDP_Core_1_Init()
{
    //UDP initialization
    UDP_Init(&udp_pcb_core_1, UDP_port, UDP_Receive_Callback);

    //MULTICORE initialization and core_1 function launch
    multicore_launch_core1(core_1_entry);
}

void pico_Wifi_Transmission_Init()
{
    UDP_Core_0_Init();
    UDP_Core_1_Init();

    //TODO
}