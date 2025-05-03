#ifdef altlasten
static err_t tcp_connected(void *arg, struct altcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("Verbindung fehlgeschlagen!\n");
        return err;
    }
    printf("Verbunden!\n");
    char msg[104];
    time_t now = time(NULL);
    struct tm start = *localtime(&now);
    struct tm end = start;
    end.tm_mon += 1;
    mktime(&end);

    snprintf(msg, sizeof(msg), "GET /api/calendars/calendar.fte1?start=%04d-%02d-%02dT%02d:%02d:%02dZ&end=%04d-%02d-%02dT23:59:59Z HTTP/1.1\r\n",
            start.tm_year + 1900, start.tm_mon + 1, start.tm_mday, start.tm_hour, start.tm_min, start.tm_sec,
            end.tm_year + 1900, end.tm_mon + 1, end.tm_mday);
                        

    const char *msg2 = "Host: 192.168.0.54:8123\r\n"
                        "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiI4NTkyNDAyZmUyZWY0YmI3YTljYTZiYzczOTkzMzRmMiIsImlhdCI6MTc0NTc3MzU4MiwiZXhwIjoyMDYxMTMzNTgyfQ.ER5nX_OAYzBtFG1ATkkcPjK0ljOmcGLbH2eb7ja6iVk\r\n"
                        "Connection: close\r\n"
                        "\r\n";

    altcp_write(tpcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);
    altcp_write(tpcb, msg2, strlen(msg2), TCP_WRITE_FLAG_COPY);
    altcp_output(tpcb); // Daten senden
    printf("Nachricht gesendet\n");
    return ERR_OK;
}

static err_t tcp_recv_data(void *arg, struct altcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        printf("Verbindung beendet\n");
        printf("Fehler: %d\n", err);
        altcp_close(tpcb);
        return ERR_OK;
    }

    printf("Empfangen: %.*s\n", p->tot_len, (char *)p->payload);
    altcp_recved(tpcb, p->tot_len);
    pbuf_free(p);

    return ERR_OK;
}
#endif