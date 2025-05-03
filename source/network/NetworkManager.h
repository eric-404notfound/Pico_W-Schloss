#pragma once

#include "pico/cyw43_arch.h"
#include "lwip/ip_addr.h"

#define NET_MQTT 1

#define NET_TCP 1
#define NET_TCP_TLS 1

#define NET_DNS 1
#define NET_SNTP 1

#include "Wifi_PW.h"

#ifdef __cplusplus
extern "C" {
#endif

bool connect_wifi();


#ifdef NET_DNS 
    #include "lwip/dns.h"
    typedef struct DNS_CALLBACK_DATA_T_ {
        ip_addr_t *ipaddr;
        volatile bool dns_done;
        bool dns_found;
    } DNS_CALLBACK_DATA_T;
    bool block_get_dns_ip(const char *hostname, ip_addr_t* ipaddr);
#endif

#ifdef NET_TCP
    #include "lwip/altcp_tcp.h"
    #include "lwip/altcp.h"
    #include "lwip/tcp.h"
    #include "lwip/altcp_tls.h"
    #define TCP_BUF_SIZE 1024
    
    static struct altcp_allocator_s altcp_tcp_allocator = {
        altcp_tcp_alloc,  // Standard TCP-Allocator Funktion
        NULL              // Keine Free-Funktion nötig
    };

    struct altcp_tls_config; // Forward-Deklaration für C
    typedef struct altcp_tls_config altcp_tls_config;

    typedef struct TCP_CLIENT_T_ {
        struct altcp_pcb *pcb;
        ip_addr_t remote_addr;
        uint8_t buffer[TCP_BUF_SIZE];
        altcp_tls_config* tls_config;
        int buffer_len;
        int sent_len;
        bool complete;
        int run_count;
        bool connected;
    } TCP_CLIENT_T;

    void default_tcp_error(void *arg, err_t err);

    TCP_CLIENT_T* new_tcp_client(const char* host, int port,altcp_connected_fn connected_fn, altcp_recv_fn recv_fn, altcp_err_fn err_fn);

    void close_client(TCP_CLIENT_T* client);

    #ifdef NET_TCP_TLS
        #include "lwip/altcp_tls.h"
        TCP_CLIENT_T* new_tls_client(const char* host, int port, const char* Cert, altcp_connected_fn connected_fn, altcp_recv_fn recv_fn, altcp_err_fn err_fn);
    #endif

#endif

#ifdef NET_SNTP
    #include "lwip/apps/sntp.h"
    void start_sntp();
#endif


#ifdef NET_MQTT
    #include "lwip/apps/mqtt.h"
    #include "lwip/apps/mqtt_priv.h" // needed to set hostname
    #define MQTT_TOPIC_LEN 100 //max length of topic

    typedef struct MQTT_CLIENT_DATA_{
        mqtt_client_t* mqtt_client_inst;
        struct mqtt_connect_client_info_t mqtt_client_info;
        ip_addr_t mqtt_server_address;
        uint16_t mqtt_server_port;
        bool connected;

        char data[MQTT_OUTPUT_RINGBUF_SIZE];
        char topic[MQTT_TOPIC_LEN];
        uint32_t len;
        
        int subscribe_count;
        bool stop_client;
    } MQTT_CLIENT_DATA_T;

    MQTT_CLIENT_DATA_T* new_mqtt_client(const char *host, uint16_t port, const char *client_id);
    void free_mqtt_client(MQTT_CLIENT_DATA_T *state);
    void start_client(MQTT_CLIENT_DATA_T *state, mqtt_connection_cb_t connectin_change_cb, mqtt_incoming_publish_cb_t mqtt_incoming_publish_cb, mqtt_incoming_data_cb_t mqtt_incoming_data_cb, void* arg);
    void mqtt_set_username_password(MQTT_CLIENT_DATA_T *state, const char *username, const char *password);
#endif

#ifdef __cplusplus
}
#endif