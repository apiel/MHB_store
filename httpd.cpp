#include <lwip/api.h>
#include <string.h>
#include <espressif/esp_common.h>

#include "config.h"
#include "store.h"
#include "wifi.h"

char uptimeResponse[128];
char * getUptime()
{
    int sec = xTaskGetTickCount() * portTICK_PERIOD_MS / 1000;
    int min = sec * 0.01666667;
    sec -= min * 60;
    int h = min * 0.01666667;
    min -= h*60;
    int day = h * 0.041666667;
    h -= day * 24;

    snprintf(uptimeResponse, sizeof(uptimeResponse),
        "HTTP/1.1 200 OK\r\n"
        "Content-type: application/json\r\n\r\n"
        "%d day %d hrs %d min %d sec",
        day, h, min, sec);

    return uptimeResponse;
}

char * ok()
{
    return (char*)
        "HTTP/1.1 200 OK\r\n"
        "Content-type: application/json\r\n\r\n"
        "OK";
}

char * parse_request(void *data)
{
    char * response = NULL;
    // printf("data: %s\n", (char *)data);
    if (strstr((char *)data, (char *)"/open")) {
        store.open();
        response = ok();
    } else if (strstr((char *)data, (char *)"/close")) {
        store.close();
        response = ok();
    } else if (strstr((char *)data, (char *)"/toggle")) {
        store.toggle();
        response = ok();
    } else if (strstr((char *)data, (char *)"/stop")) {
        store.stop();
        response = ok();
    } else if (strstr((char *)data, (char *)"/uptime")) {
        response = getUptime();
    } else if (strstr((char *)data, (char *)"/reboot")) {
        printf("Reboot\n");
        sdk_system_restart();
    } else {
        printf("unknown route\n");
    }

    return response;
}

#define HTTPD_PORT 80

struct netconn *client = NULL;
struct netconn *nc = NULL;

void httpd_init()
{
    nc = netconn_new(NETCONN_TCP);
    if (nc == NULL) {
        printf("Failed to allocate socket\n");
        vTaskDelete(NULL);
    }
    netconn_bind(nc, IP_ADDR_ANY, HTTPD_PORT);
    netconn_listen(nc);
    netconn_set_recvtimeout(nc, 2000);
    netconn_set_sendtimeout(nc, 2000);
}

void httpd_loop()
{
    char * response = NULL;
    err_t err = netconn_accept(nc, &client);
    if (err == ERR_OK) {
        struct netbuf *nb;
        if ((err = netconn_recv(client, &nb)) == ERR_OK) {
            void *data = NULL;
            u16_t len;
            if (netbuf_data(nb, &data, &len) == ERR_OK) {
                // printf("Received data:\n%.*s\n", len, (char*) data);
                response = parse_request(data);
            }
            if (!response) {
                response = (char *)"HTTP/1.1 404 OK\r\nContent-type: text/html\r\n\r\nUnknown route\r\n";
            }
            netconn_write(client, response, strlen(response), NETCONN_COPY);
        }
        netbuf_delete(nb);
    }
    netconn_close(client);
    netconn_delete(client);
}
