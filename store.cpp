#include <espressif/esp_common.h>
#include <esp8266.h>

#include "config.h"
#include "store.h"

Store::Store()
{
    gpio_enable(PIN_RELAY_POWER, GPIO_OUTPUT);
    gpio_enable(PIN_RELAY_MOTOR, GPIO_OUTPUT);
}

void Store::stop()
{
    gpio_write(PIN_RELAY_POWER, RELAY_OFF);
    printf("Stop store\n");
}

void Store::start(int mode)
{
    _mode = mode;
    gpio_write(PIN_RELAY_POWER, RELAY_OFF);
    gpio_write(PIN_RELAY_MOTOR, mode);
    gpio_write(PIN_RELAY_POWER, RELAY_ON);
    printf("start store\n");
}

void Store::open()
{
    start(RELAY_OPEN);
    printf("open store\n");
}

void Store::close()
{
    start(RELAY_CLOSE);
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
