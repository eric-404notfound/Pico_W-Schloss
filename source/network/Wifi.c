#include "NetworkManager.h"



bool connect_wifi() {

    if (cyw43_arch_init() != 0) {
        printf("failed to initialise\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();
    int status = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000);
    for (int i = 0; i < 5; i++) {
        printf("Waiting for Wi-Fi...\n");
        
        if (status == 0) {
            printf("Connected to Wi-Fi\n");
            break;
        }
        sleep_ms(1000);
        status = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000);
    }
    if (!(status == 0)) {
        cyw43_arch_deinit();
        if (status == -2) {
            printf("Timeout connecting to Wi-Fi\n");
        }else {
            printf("Status: %d\n", status);
        }
        return false;
    }
    return true;
}
