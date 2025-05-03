#include <stdio.h>
#include <sys/time.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"

#include <string.h>
#include <math.h>

#include "PW.h"

#include "Cert.h"
#include "home_assistant/HomeAssistant.h"
#include "home_assistant/JSON/Discovery.h"

#define NET_DNS 1

bool lock_timer = false; 
HomeAssistant_MQTT* mqtt_client_public = NULL;

void gpio_callback(uint gpio, uint32_t events) {
    if(gpio_get(gpio) == 1) {
        mqtt_client_public->publish("HomeAutomation/lock/Haustuer/state", "UNLOCKED");
        return;
    };
    mqtt_client_public->publish("HomeAutomation/lock/Haustuer/state", "LOCKED");
};

int64_t abschliessen(alarm_id_t id, void *arg) {
    lock_timer = false;
    HomeAssistant_MQTT* mqtt_client = (HomeAssistant_MQTT*)arg;
    gpio_put(15, 0);
    return 0;
}


void lock_callback(void* arg, char* payload, size_t len) {
    HomeAssistant_MQTT* mqtt_client = (HomeAssistant_MQTT*)arg;
    printf("Lock command received: %.*s\n", (int)len, payload);
    if (strncmp(payload, "LOCK", len) == 0) {

        printf("Locking the door...\n");
        gpio_put(15, 0);
        
        // Add your lock logic here
    } else if (strncmp(payload, "UNLOCK", len) == 0) {
        if (!lock_timer) {
            lock_timer = true;
            alarm_id_t id = add_alarm_in_ms(3000, abschliessen, arg, true);
        }
        printf("Unlocking the door...\n");
        gpio_put(15, 1);
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
    gpio_pull_down(15);
    gpio_init(14);
    gpio_set_dir(14, GPIO_IN);
    gpio_pull_down(14);

    
    if(!connect_wifi())
        return -1;
    
    start_sntp();
    
    HomeAssistant_MQTT mqtt_client("192.168.0.54", 1883, "Pico2W_Lock_Haustuer");
    mqtt_client.setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);
    mqtt_client.connect();
    mqtt_client.registerHandler("HomeAutomation/lock/Haustuer/command", lock_callback, (void*)&mqtt_client);
    mqtt_client.publish(discovery_topic, entity_type);
    mqtt_client_public = &mqtt_client;
    gpio_put(15, 0);
    mqtt_client.publish("HomeAutomation/lock/Haustuer/state", "LOCKED");
    gpio_set_irq_enabled_with_callback(14, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    while(true) {
        sleep_ms(100);
    }

    cyw43_arch_deinit();
    return 0;
    
}