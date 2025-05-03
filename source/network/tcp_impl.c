#include "NetworkManager.h"


#ifdef NET_TCP
    void default_tcp_error(void *arg, err_t err) {
        printf("TCP ERROR CALLBACK: %d\n", err);
    }

    TCP_CLIENT_T* new_tcp_client(const char* host, int port,altcp_connected_fn connected_fn, altcp_recv_fn recv_fn, altcp_err_fn err_fn) {
        printf("TCP-Client starten...\n");

        ip_addr_t ip;
        if (!ip4addr_aton("192.168.0.54", &ip)) {
            #ifdef NET_DNS
                if (!block_get_dns_ip(host, &ip)) {
                    printf("DNS-Fehler: Host %s konnte nicht aufgelöst werden\n", host);
                    return NULL;
                }
            #else
                printf("IP-Adresse ungültig\n");

            #endif
            return NULL;
        }

        
        TCP_CLIENT_T* client = (TCP_CLIENT_T*)malloc(sizeof(TCP_CLIENT_T));
        if (!client) {
            printf("Speicherzuweisung fehlgeschlagen\n");
            return NULL;
        }

        client->pcb = altcp_new(&altcp_tcp_allocator);
        assert(client->pcb);

        altcp_arg(client->pcb, client);
        altcp_recv(client->pcb, recv_fn);
        altcp_err(client->pcb, err_fn);

        err_t err = altcp_connect(client->pcb, &ip, 8123, connected_fn);

        if (err != ERR_OK)
        {
            printf("error initiating connect, err=%d\n", err);
            altcp_close(client->pcb);
            free(client);
            return NULL;
        }
        return client; 
    }

    void close_client(TCP_CLIENT_T* client) {
        if (client) {
            if (client->pcb) {
                altcp_close(client->pcb);
            }
            if (client->tls_config) {
                altcp_tls_free_config(client->tls_config);
            }
            free(client);
        }
    }


        #ifdef NET_TCP_TLS
        TCP_CLIENT_T* new_tls_client(const char* host, int port, const char* Cert, altcp_connected_fn connected_fn, altcp_recv_fn recv_fn, altcp_err_fn err_fn) {
            
            ip_addr_t ip;
            if (!ip4addr_aton("192.168.0.54", &ip)) {
                #ifdef NET_DNS
                    if (!block_get_dns_ip(host, &ip)) {
                        printf("DNS-Fehler: Host %s konnte nicht aufgelöst werden\n", host);
                        return NULL;
                    }
                #else
                    printf("IP-Adresse ungültig\n");
    
                #endif
                return NULL;
            }

            TCP_CLIENT_T* client = (TCP_CLIENT_T*)malloc(sizeof(TCP_CLIENT_T));

            if (!client) {
                printf("Speicherzuweisung fehlgeschlagen\n");
                return NULL;
            }
            client->tls_config = altcp_tls_create_config_client((const u8_t*)Cert, sizeof(Cert));
            if (!client->tls_config) {
                printf("TLS-Config konnte nicht erstellt werden\n");
                free(client);
                return NULL;
            }

            client->pcb = altcp_tls_new(client->tls_config, IPADDR_TYPE_ANY);
            assert(client->pcb);

            altcp_arg(client->pcb, client);
            altcp_recv(client->pcb, recv_fn);
            altcp_err(client->pcb, err_fn);

            int error = mbedtls_ssl_set_hostname(altcp_tls_context(client->pcb), host);
            if (error != 0) {
                printf("error setting hostname, err=%d\n", error);
                altcp_close(client->pcb);
                return NULL;
            }
            cyw43_arch_lwip_begin();
            err_t err = altcp_connect(client->pcb, &ip, 8123, connected_fn);
            cyw43_arch_lwip_end();

            if (err != ERR_OK)
            {
                printf("error initiating connect, err=%d\n", err);
                altcp_close(client->pcb);
                altcp_tls_free_config(client->tls_config);
                free(client);
                return NULL;
            }

            return client; 
        }
        #endif
#endif
