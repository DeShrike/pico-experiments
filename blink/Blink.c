#include "pico/stdlib.h"

int main()
{
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
    //const uint LED1_PIN = PICO_DEFAULT_LED_PIN;
    const uint LED1_PIN = 16;
    gpio_init(LED1_PIN);

    gpio_set_dir(LED1_PIN, GPIO_OUT);

    while (true)
    {
        gpio_put(LED1_PIN, 1);
        sleep_ms(1000);
        gpio_put(LED1_PIN, 0);
        sleep_ms(1000);
    }
#endif
}
