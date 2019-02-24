#include <espressif/esp_common.h>
#include <esp8266.h>

#include "config.h"
#include "store.h"

Store::Store()
{
    gpio_enable(PIN_OPEN, GPIO_OUTPUT);
    gpio_enable(PIN_CLOSE, GPIO_OUTPUT);
    gpio_enable(RELAY_OPEN, GPIO_OUTPUT);
}

void Store::stop()
{
    gpio_write(OFF, PIN_OPEN);
    gpio_write(OFF, PIN_CLOSE);
    printf("Stop store\n");
}

void Store::open()
{
    stop();
    _mode = RELAY_OPEN;
    gpio_write(RELAY_OPEN, PIN_RELAY);
    gpio_write(ON, PIN_OPEN);
    printf("open store\n");
}

void Store::close()
{
    stop();
    _mode = RELAY_CLOSE;
    gpio_write(RELAY_CLOSE, PIN_RELAY);
    gpio_write(ON, PIN_CLOSE);
    printf("close store\n");
}

void Store::toggle()
{
    if (_mode != RELAY_OPEN) {
        open();
    } else {
        close();
    }
}

Store store = Store();
