
#include "private_lwipopts_default.h"
// Generally you would define your own explicit list of lwIP options
// (see https://www.nongnu.org/lwip/2_1_x/group__lwip__opts.html)
//
// This example uses a common include to avoid repetition


/* SNTP-Modul */
#define SNTP_SERVER_DNS        1
#define SNTP_MAX_SERVERS       1
#define SNTP_SERVER_ADDRESS    "pool.ntp.org"
#define SNTP_STARTUP_DELAY     0
#define SNTP_UPDATE_DELAY      3600000

/* TCP-Modul*/
#define MEMP_NUM_SYS_TIMEOUT  8

//TLS
#undef TCP_WND
#define TCP_WND  (16 * 1024)

#define LWIP_ALTCP 1
#define LWIP_ALTCP_TLS 1
#define LWIP_ALTCP_TLS_MBEDTLS   1

/* DNS-Modul*/
#define LWIP_DNS                     1
#define DNS_DEBUG  LWIP_DBG_OFF

#define LWIP_DEBUG                   1

/* MQTT-Modul*/
#define MQTT_OUTPUT_RINGBUF_SIZE 1024




//#define debugging 1
#ifdef debugging
  #define LWIP_DEBUG                   1
  #define ALTCP_DEBUG         LWIP_DBG_ON
  #define ALTCP_TLS_DEBUG     LWIP_DBG_ON
  #define TCP_DEBUG           LWIP_DBG_ON
  #define MBEDTLS_DEBUG_C
  #define ALTCP_MBEDTLS_DEBUG  LWIP_DBG_ON
  #define LWIP_DBG_TYPES_ON   (LWIP_DBG_ON | LWIP_DBG_TRACE | LWIP_DBG_STATE)
  #define LWIP_DBG_MIN_LEVEL  LWIP_DBG_LEVEL_ALL
  #define MQTT_DEBUG              LWIP_DBG_ON
  #define MQTT_APP_DEBUG          LWIP_DBG_ON
#endif




/* Zeit-Setter: wird in sntp.c aufgerufen */
#define SNTP_SET_SYSTEM_TIME(sec)    \
  do {                              \
    struct timeval tv;              \
    tv.tv_sec  = (sec);             \
    tv.tv_usec = 0;                 \
    settimeofday(&tv, NULL);        \
    printf("Set time to %s", ctime(&tv.tv_sec)); \
  } while(0)