#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define DURATION_EACH 10000	// 10 seconds

int sleepix = 0;
int sleeps[] = { 125, 1000, 200, 800, 250, 900, 50, 700 };
int count;

#define SLEEPS_COUNT (sizeof(sleeps) / sizeof(int))

int main()
{
    stdio_init_all();
    if (cyw43_arch_init())
    {
        printf("Wi-Fi init failed");
        return -1;
    }

    const uint LED1_PIN = 16;
    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);

    count = DURATION_EACH / sleeps[sleepix];

    while (true)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        gpio_put(LED1_PIN, 0);
        sleep_ms(sleeps[sleepix]);

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        gpio_put(LED1_PIN, 1);
        sleep_ms(sleeps[sleepix]);

	count--;
        if (count <= 0)
        {
         	sleepix = (sleepix + 1) % SLEEPS_COUNT;
      		count = DURATION_EACH / sleeps[sleepix];
        }
    }
}
