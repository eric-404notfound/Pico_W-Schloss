
// Eigene Header-Datei siehe pico-examples lwipops_examples_common.h
// #include "lwipopts_example_common.h"

#ifndef _LWIPOPTS_EXAMPLE_COMMONH_H
  #define _LWIPOPTS_EXAMPLE_COMMONH_H
#endif


/* LWIP */
#ifndef NO_SYS
  #define NO_SYS                    1
#endif
#ifndef LWIP_SOCKET
  #define LWIP_SOCKET               0
#endif
#if PICO_CYW43_ARCH_POLL
  #define MEM_LIBC_MALLOC           1
#else
  #define MEM_LIBC_MALLOC           0
#endif
#define MEM_ALIGNMENT               4
#ifndef MEM_SIZE
  #define MEM_SIZE                  8000  
#endif
#define MEMP_NUM_SYS_TIMEOUT        8
#define MEMP_NUM_ARP_QUEUE          10
#define PBUF_POOL_SIZE              24
#define LWIP_ARP                    1
#define LWIP_ETHERNET               1
#define LWIP_ICMP                   1
#define LWIP_RAW                    1
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1
#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETCONN                0
#define MEM_STATS                   0
#define SYS_STATS                   0
#define MEMP_STATS                  0
#define LINK_STATS                  0
#define LWIP_CHKSUM_ALGORITHM       3
#define LWIP_IPV4                   1
#define LWIP_NETIF_TX_SINGLE_PBUF   1
#define LWIP_UDP                    1


/* DHCP */
#define LWIP_DHCP                   1
#define DHCP_DOES_ARP_CHECK         0
#define LWIP_DHCP_DOES_ACD_CHECK    0

/* TCP */
#define LWIP_TCP                    1
#define TCP_MSS                     1460
//                                  (8 * TCP_MSS)
#define TCP_WND                     (16 * TCP_MSS)
#define TCP_SND_BUF                 (8 * TCP_MSS)
#define LWIP_TCP_KEEPALIVE          1
#define TCP_SND_QUEUELEN            ((4 * (TCP_SND_BUF) + (TCP_MSS - 1)) / (TCP_MSS))
#define MEMP_NUM_TCP_SEG            32

/* ALTCP */
#define LWIP_ALTCP                  1
#define LWIP_ALTCP_TLS              1

/* ALTCP-TLS */
#define LWIP_ALTCP_TLS_MBEDTLS      1

/* SNTP*/
#define SNTP_SERVER_DNS             1
#define SNTP_MAX_SERVERS            1
#define SNTP_SERVER_ADDRESS         "pool.ntp.org"
#define SNTP_STARTUP_DELAY          0
#define SNTP_UPDATE_DELAY           3600000

/* Zeit-Setter: wird in sntp.c aufgerufen */
#define SNTP_SET_SYSTEM_TIME(sec)   \
  do {                              \
    struct timeval tv;              \
    tv.tv_sec  = (sec);             \
    tv.tv_usec = 0;                 \
    settimeofday(&tv, NULL);        \
    printf("Set time to %s",        \
                ctime(&tv.tv_sec)); \
  } while(0)                        \



/* DNS-Modul*/
#define LWIP_DNS                     1




/* MQTT-Modul*/
#define MQTT_OUTPUT_RINGBUF_SIZE     2048

#define LWIP_DEBUG                   1
#define ALTCP_MBEDTLS_DEBUG  LWIP_DBG_ON

/*  Debugging */
#ifndef NDEBUG
  #define LWIP_DEBUG                  1
  #define LWIP_STATS                  1
  #define LWIP_STATS_DISPLAY          1
#endif


#ifdef full_debugging
  #define LWIP_DEBUG                  1
  #define DNS_DEBUG                   LWIP_DBG_OFF
  #define ETHARP_DEBUG                LWIP_DBG_OFF
  #define NETIF_DEBUG                 LWIP_DBG_OFF
  #define PBUF_DEBUG                  LWIP_DBG_OFF
  #define API_LIB_DEBUG               LWIP_DBG_OFF
  #define API_MSG_DEBUG               LWIP_DBG_OFF
  #define SOCKETS_DEBUG               LWIP_DBG_OFF
  #define ICMP_DEBUG                  LWIP_DBG_OFF
  #define INET_DEBUG                  LWIP_DBG_OFF
  #define IP_DEBUG                    LWIP_DBG_OFF
  #define IP_REASS_DEBUG              LWIP_DBG_OFF
  #define RAW_DEBUG                   LWIP_DBG_OFF
  #define MEM_DEBUG                   LWIP_DBG_OFF
  #define MEMP_DEBUG                  LWIP_DBG_OFF
  #define SYS_DEBUG                   LWIP_DBG_OFF
  #define TCP_DEBUG                   LWIP_DBG_OFF
  #define TCP_INPUT_DEBUG             LWIP_DBG_OFF
  #define TCP_OUTPUT_DEBUG            LWIP_DBG_OFF
  #define TCP_RTO_DEBUG               LWIP_DBG_OFF
  #define TCP_CWND_DEBUG              LWIP_DBG_OFF
  #define TCP_WND_DEBUG               LWIP_DBG_OFF
  #define TCP_FR_DEBUG                LWIP_DBG_OFF
  #define TCP_QLEN_DEBUG              LWIP_DBG_OFF
  #define TCP_RST_DEBUG               LWIP_DBG_OFF
  #define UDP_DEBUG                   LWIP_DBG_OFF
  #define TCPIP_DEBUG                 LWIP_DBG_OFF
  #define PPP_DEBUG                   LWIP_DBG_OFF
  #define SLIP_DEBUG                  LWIP_DBG_OFF
  #define DHCP_DEBUG                  LWIP_DBG_OFF
  #define MQTT_DEBUG                   LWIP_DBG_ON
  #define MQTT_APP_DEBUG               LWIP_DBG_ON
#endif





