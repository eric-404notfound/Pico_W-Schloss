#include "NetworkManager.h"

#ifdef NET_MQTT

MQTT_CLIENT_DATA_T* new_mqtt_client(const char* host, uint16_t port, const char *client_id) {

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

    MQTT_CLIENT_DATA_T* state = (MQTT_CLIENT_DATA_T*)calloc(1, sizeof(MQTT_CLIENT_DATA_T));
    if (!state) {
        printf("MQTT client instance calloc error");
        return NULL;
    }


    state->mqtt_server_address = ip;
    state->mqtt_server_port = port;
    state->hostname = host;


    state->mqtt_client_inst = mqtt_client_new();
    if (!state->mqtt_client_inst) {
        printf("MQTT client instance creation error");
        return NULL;
    }

    state->mqtt_client_info.client_user = NULL;
    state->mqtt_client_info.client_pass = NULL;
    char * clid = (char *)calloc(1, strlen(client_id) + 1);
    if (!clid) {
        printf("MQTT clientid instance calloc error");
        return NULL;
    }
    strcpy(clid, client_id);
    state->mqtt_client_info.client_id = clid;

    return state;
}

void mqtt_set_username_password(MQTT_CLIENT_DATA_T *state, const char *username, const char *password) {

    state->mqtt_client_info.client_user = calloc(1, strlen(username) + 1);
    state->mqtt_client_info.client_pass = calloc(1, strlen(password) + 1);
    strcpy((char *)state->mqtt_client_info.client_user, username);
    strcpy((char *)state->mqtt_client_info.client_pass, password);

}

void free_mqtt_client(MQTT_CLIENT_DATA_T *state) {
    if(state){
    if (state->mqtt_client_inst) {
        mqtt_client_free(state->mqtt_client_inst);
        state->mqtt_client_inst = NULL;
    }
    if (state->mqtt_client_info.client_user) {
        free((void*)state->mqtt_client_info.client_user);
        state->mqtt_client_info.client_user = NULL;
    }
    if (state->mqtt_client_info.client_pass) {
        free((void*)state->mqtt_client_info.client_pass);
        state->mqtt_client_info.client_pass = NULL;
    }
    if (state->mqtt_client_info.client_id) {
        free((void*)state->mqtt_client_info.client_id);
        state->mqtt_client_info.client_id = NULL;
    }

    free(state);
    }
}




void start_client(MQTT_CLIENT_DATA_T *state, mqtt_connection_cb_t connectin_change_cb, mqtt_incoming_publish_cb_t mqtt_incoming_publish_cb, mqtt_incoming_data_cb_t mqtt_incoming_data_cb, void* arg) {



    cyw43_arch_lwip_begin();
    if (mqtt_client_connect(state->mqtt_client_inst, &state->mqtt_server_address, state->mqtt_server_port, connectin_change_cb, arg, &state->mqtt_client_info) != ERR_OK) {
        panic("MQTT broker connection error");
    }
    mbedtls_ssl_set_hostname(altcp_tls_context(state->mqtt_client_inst->conn), state->hostname);

    mqtt_set_inpub_callback(state->mqtt_client_inst, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);
    cyw43_arch_lwip_end();
    }



#ifdef NET_TCP_TLS

    void mbedtls_debug(void *ctx, int level, const char *file, int line, const char *str) {
        printf("mbedTLS [%d] %s:%d: %s", level, file, line, str);
    }
    

    int cert_verify_callback(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags) {
        const mbedtls_x509_crt *expected = (const mbedtls_x509_crt *)data;
        printf("Zertifikatstiefe: %d\n", depth);
        if (crt->raw.len == expected->raw.len &&
            memcmp(crt->raw.p, expected->raw.p, crt->raw.len) == 0) {
            *flags = 0; // Gültig
            return 0;
        } else {
            *flags |= MBEDTLS_X509_BADCERT_NOT_TRUSTED;
            return -1;
        }
    }


    void mqtt_set_tls_config(MQTT_CLIENT_DATA_T *state, const char* cert) {
        printf("Zurzeit wird das Certifikat nicht überprüft (Problem wird noch analysiert)\n");
        
        //state->mqtt_client_info.tls_config = altcp_tls_create_config_client(cert, strlen(cert) + 1);
        state->mqtt_client_info.tls_config = altcp_tls_create_config_client(NULL, 0);

        #ifdef ProblemBehoben
            mbedtls_ssl_config* conf = altcp_tls_context(state->mqtt_client_info.tls_config);

            mbedtls_ssl_conf_dbg(conf, mbedtls_debug, NULL);
            mbedtls_debug_set_threshold(4); // 0=aus, 4=voll


            
            mbedtls_x509_crt_init(&state->cacert);
            if (mbedtls_x509_crt_parse(&state->cacert, (const unsigned char*)cert, strlen(cert) + 1) != 0) {
                printf("Zertifikat konnte nicht geparst werden\n");
                return;
            }

            mbedtls_ssl_conf_verify(conf, cert_verify_callback, &state->cacert);
            mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_REQUIRED);
        #endif
    }
    #endif
#endif
