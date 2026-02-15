#include <stdio.h>
#include "pico/stdlib.h"
#include "ST7735_TFT.h"

void init(void)
{
    TFT_GreenTab_Initialize();
    printf("Width: %d  Height: %d\n", tft_width, tft_height);
}

void update(void)
{
}

void draw(void)
{
    fillScreen(Color565(127, 0, 0));
    drawLine(0, 0, 100, 100, Color565(127, 127, 127));
}

bool timer_callback(struct repeating_timer *t)
{
    printf("Timer %d  - Delay %ldus\n", t->alarm_id, t->delay_us);
    return true;
}

int64_t alarm_callback(alarm_id_t id, void *user_data)
{
    printf("Alarm %d\n", id);
    return 2000 * 1000;     // 2000 milliseconds = 2 seconds = 2000000 microseconds
}

int main(void)
{
    init();

    struct repeating_timer timer;
    add_repeating_timer_ms(3000, timer_callback, NULL, &timer);
    add_alarm_in_ms(2000, alarm_callback, NULL, false);

    while (true)
    {
        update();
        draw();
    }

    return 0;
}

/*
static uint8_t reverse(uint8_t b)
{
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}
*/
