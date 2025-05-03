#include "NetworkManager.h"


#ifdef NET_DNS
static void dns_found_block(const char *hostname, const ip_addr_t *ipaddr, void *arg) {
    DNS_CALLBACK_DATA_T *state = (DNS_CALLBACK_DATA_T*)arg;
    if (ipaddr) {
        *state->ipaddr = *ipaddr;
        state->dns_found = true;
        state->dns_done = true;
    } else {
        state->dns_found = false;
        state->dns_done = true;
    }
}


bool block_get_dns_ip(const char *hostname, ip_addr_t* ipaddr) {

    DNS_CALLBACK_DATA_T *dns_callback_data = (DNS_CALLBACK_DATA_T*)malloc(sizeof(DNS_CALLBACK_DATA_T));
    if (!dns_callback_data) {
        panic("malloc failed");
    }
    dns_callback_data->ipaddr = ipaddr;
    dns_callback_data->dns_done = false;
    dns_callback_data->dns_found = false;

    cyw43_arch_lwip_begin();
    int err = dns_gethostbyname(hostname, ipaddr, dns_found_block, (void*)dns_callback_data);
    cyw43_arch_lwip_end();

    if (err == ERR_OK) {
        return true;
    } else if (err == ERR_INPROGRESS) {
        while (!(dns_callback_data->dns_done)) {
            tight_loop_contents();
        }
    }
    if (dns_callback_data->dns_found) {
        return true;
    } else {
        return false;
    }
}
#endif
