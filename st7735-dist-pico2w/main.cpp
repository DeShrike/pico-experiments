#include "pico/time.h"
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "hardware/spi.h"
#include "utils.hpp"
#include "st7735/ST7735_TFT.hpp"

#define WIDTH	128
#define HEIGHT  160

ST7735_TFT myTFT;
void init();

#define POINTS 6
uint8_t px[POINTS], py[POINTS];
int8_t dx[POINTS], dy[POINTS];
uint16_t *buffer;
#define IX(x, y)  ((y) * WIDTH + (x))

void setup()
{
   for (uint8_t i = 0; i < POINTS; i++)
   {
     px[i] = (int8_t)(get_rand_32() % WIDTH);
     py[i] = (int8_t)(get_rand_32() % HEIGHT);
     dx[i] = get_rand_32() % 1000 > 500 ? 1 : -1;
     dy[i] = get_rand_32() % 1000 > 500 ? 1 : -1;
   }

   buffer = (uint16_t*)malloc(WIDTH * HEIGHT * sizeof(uint16_t));
}

void update()
{
   int16_t nx, ny;
   for (uint8_t i = 0; i < POINTS; i++)
   {
	  nx = px[i] + dx[i];
      if (nx < 0)
      {
         nx = 0;
         dx[i] = -dx[i];
      }
      if (nx >= WIDTH)
      {
         nx = WIDTH - 1;
         dx[i] = -dx[i];
      }

	  ny = py[i] + dy[i];
      if (ny < 0)
      {
         ny = 0;
         dy[i] = -dy[i];
      }
      if (ny >= HEIGHT)
      {
         ny = HEIGHT - 1;
         dy[i] = -dy[i];
      }

      px[i] = (uint8_t)nx;
      py[i] = (uint8_t)ny;
   }
}

void draw()
{
   uint16_t dist[POINTS];
   uint16_t c;
   uint8_t t;
   for (uint8_t y = 0; y < HEIGHT; y++)
   {
      for (uint8_t x = 0; x < WIDTH; x++)
      {
         for (uint8_t p = 0; p < POINTS; p++)
         {
            dist[p] = (abs(px[p] - x) + abs(py[p] - y)) % 255;
         }

         //buffer[IX(x, y)] = Color565_Reversed(dist[0], dist[1], dist[2]);

         buffer[IX(x, y)] = Color565_Reversed(
              dist[0] + dist[3],
              dist[1] - dist[4],
              dist[2] + dist[5]);

         //buffer[IX(x, y)] = Color565_Reversed(
         //     std::min(dist[0], dist[3]),
         //     std::min(dist[1], dist[4]),
         //     std::min(dist[2], dist[5]));
      }
   }

   myTFT.TFTdrawBitmap16FullScreen(buffer);
}

int main(void)
{
  init();
  setup();
  while (true)
  {
    update();
    draw();
  }

  myTFT.TFTPowerDown();

  return 0;
}

void init(void)
{
   TFT_MILLISEC_DELAY(1000);

   bool useHardwareSPI = true; // true for hardware spi,

   if (useHardwareSPI == true)
   {
      // hw spi
      uint32_t TFT_SCLK_FREQ = 8000 ; // Spi freq in KiloHertz , 1000 = 1Mhz
      myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
   }
   else
   {
      // sw spi
      myTFT.TFTInitSPIType();
   }

   // NOTE if using Hardware SPI clock and data pins will be tied to
   // the chosen interface eg Spi0 CLK=18 DIN=19)
   int8_t SDIN_TFT = 19;
   int8_t SCLK_TFT = 18;
   int8_t DC_TFT = 3;
   int8_t CS_TFT = 2;
   int8_t RST_TFT = 17;
   myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);

   uint8_t OFFSET_COL = 0;    // 2, These offsets can be adjusted for any issues->
   uint8_t OFFSET_ROW = 0;    // 3, with screen manufacture tolerance/defects
   uint16_t TFT_WIDTH = WIDTH;  // Screen width in pixels
   uint16_t TFT_HEIGHT = HEIGHT; // Screen height in pixels
   myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);

   myTFT.TFTInitPCBType(TFT_ST7735R_Green); // pass enum, 4 choices,see README
}
