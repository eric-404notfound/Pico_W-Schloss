#include "NetworkManager.h"

#ifdef NET_SNTP
    void start_sntp(){
        setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);
        tzset();
        sntp_init();
    }
#endif