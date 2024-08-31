#include "UDP.h"

static server_To_Pico_Frame_t server_TO_Pico_Data = {0};
static pico_To_Server_Frame_t pico_To_Server_Data = {0};
static struct udp_pcb *send_pcb = NULL;
static struct udp_pcb *receive_pcb = NULL;
static ip_addr_t server_Ip;
queue_t queue_Server_To_Pico;
queue_t queue_Pico_To_Server;

static server_To_Pico_Frame_t server_To_Pico_Data_Buffer;
static pico_To_Server_Frame_t pico_To_Server_Data_Buffer;


void UDP_Receive_Init(void (*recv_callback)(void *, struct udp_pcb *, struct pbuf *, const ip_addr_t *, u16_t)) 
{
    if (receive_pcb == NULL)
    {
        receive_pcb = udp_new();
        
        if (receive_pcb != NULL)
        {
            err_t err = udp_bind(receive_pcb, IP_ADDR_ANY, UDP_port);
            if (err == ERR_OK) 
                udp_recv(receive_pcb, UDP_Receive_Callback, NULL);
            else 
                printf("Failed to bind UDP PCB\n");
        }
        else 
            printf("Failed to create UDP PCB\n");
    }
}

void UDP_Send_Data(const pico_To_Server_Frame_t *frame)
{   
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, sizeof(pico_To_Server_Frame_t), PBUF_RAM);

    if(p != NULL)
    {
        memcpy(p->payload, frame, sizeof(pico_To_Server_Frame_t));
        err_t err = udp_sendto(send_pcb, p, &server_Ip, UDP_port);
        if (err != ERR_OK) 
            printf("Failed to send UDP packet\n");
        pbuf_free(p);
    }
}   

void UDP_Receive_Callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    if (p != NULL)
    {
        server_To_Pico_Frame_t received_data;
        memcpy(&received_data, p->payload, sizeof(server_To_Pico_Frame_t));
        pbuf_free(p);

        //if (!queue_try_add(&queue, &received_data)) 
        //   printf("Queue is full\n");
        
    }
}

void pico_Hardware_wifi_Init(const char* ssid, const char* password)
{
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();

    int result = cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 10000);
    
    if (result != 0) {
        printf("WiFi connection failed!\n");
        return;
    }

    ip4addr_aton(server_IP, &server_Ip);
    
    send_pcb = udp_new();
    if (!send_pcb)
    {
        printf("Failed to create UDP PCB\n");
        return;
    }

    print_Ip_Address();
}

void core_1_Entry(void)
{
    //UDP_Receive_Init(UDP_Receive_Callback);

    //while (true) 
    //   tight_loop_contents();

    while(true)
    {
        if(queue_is_full(&queue_Pico_To_Server))
        {
            for(uint8_t i = 0; i < pico_to_server_queue_size; i++)
            {
                queue_try_remove(&queue_Pico_To_Server, &pico_To_Server_Data_Buffer);
                UDP_Send_Data(&pico_To_Server_Data_Buffer);
                memset(&pico_To_Server_Data_Buffer, 0, sizeof(pico_To_Server_Frame_t));
            }
        }

        else
        tight_loop_contents();
    }



}

void pico_Wifi_Transmission_Init(const char *ssid, const char *password)
{
    pico_Hardware_wifi_Init(ssid, password);
    sleep_ms(1000); //waiting for ip address
    
    UDP_Queue_init();
    
    multicore_launch_core1(core_1_Entry);
}

void UDP_Queue_init(void)
{
    queue_init(&queue_Server_To_Pico, sizeof(server_To_Pico_Frame_t), server_to_pico_queue_size);
    queue_init(&queue_Pico_To_Server, sizeof(pico_To_Server_Frame_t), pico_to_server_queue_size);
}

void print_Ip_Address(void)
{
    struct netif *netif = netif_list;
    while (netif != NULL) {
        if (netif_is_up(netif) && netif->ip_addr.addr != 0) {
            printf("IP address: %s\n", ip4addr_ntoa((ip4_addr_t *)&netif->ip_addr));
            return;
        }
        netif = netif->next;
    }
    printf("Unable to get IP address\n");
}