#include <stdio.h>
#include <sys/time.h>

#include "pico/stdlib.h"
#include "pico/time.h"


#include <string.h>
#include <math.h>

#include "Cert.h"
#include "home_assistant/HomeAssistant.h"
#include "home_assistant/JSON/Discovery.h"

#define NET_DNS 1


void lock_callback(void* arg, char* payload, size_t len) {
    HomeAssistant_MQTT* mqtt_client = (HomeAssistant_MQTT*)arg;
    printf("Lock command received: %.*s\n", (int)len, payload);
    if (strncmp(payload, "LOCK", len) == 0) {
        printf("Locking the door...\n");
        mqtt_client->publish("HomeAutomation/lock/Haustuer/state", "LOCKED");
        gpio_put(15, 1);
        
        // Add your lock logic here
    } else if (strncmp(payload, "UNLOCK", len) == 0) {
        printf("Unlocking the door...\n");
        mqtt_client->publish("HomeAutomation/lock/Haustuer/state", "UNLOCKED");
        gpio_put(15, 0);
        // Add your unlock logic here
    } else {
        printf("Unknown command: %.*s\n", (int)len, payload);
    }
}


int main(){

    stdio_init_all();
    printf("\nHallo welt\n\n");
    gpio_init(15);
    gpio_set_dir(15, GPIO_OUT);
    
    
    if(!connect_wifi())
        return -1;
    
    start_sntp();
    
    HomeAssistant_MQTT mqtt_client("192.168.0.54", 1883, "Pico2W_Lock_Haustuer");
    mqtt_client.setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);
    mqtt_client.connect();
    mqtt_client.registerHandler("HomeAutomation/lock/Haustuer/command", lock_callback, (void*)&mqtt_client);
    mqtt_client.publish(discovery_topic, entity_type);


    while(true) {
        sleep_ms(100);
    }

    cyw43_arch_deinit();
    return 0;
    
}