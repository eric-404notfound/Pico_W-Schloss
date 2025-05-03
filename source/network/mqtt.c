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
    state->mqtt_client_info.client_id = client_id;

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

    #if NET_MQTT_TLS && NET_TCP_TLS
        // This is important for MBEDTLS_SSL_SERVER_NAME_INDICATION
        mbedtls_ssl_set_hostname(altcp_tls_context(state->mqtt_client_inst->conn), MQTT_SERVER);
    #endif
    mqtt_set_inpub_callback(state->mqtt_client_inst, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);
    cyw43_arch_lwip_end();
    }

#endif
