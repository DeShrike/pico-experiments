#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "st7735/ST7735_TFT.hpp"

#define WIDTH	128
#define HEIGHT  160

#define TEST_DELAY1 1000
#define TEST_DELAY2 50
#define TEST_DELAY5 5000
#define CLOCK_DISPLAY_TIME 20

ST7735_TFT myTFT;
bool bTestFPS = false;

void setup();

void update()
{
}

void MeasureText(const char* text, uint8_t size, uint8_t* width, uint8_t* height)
{
   *width = (strlen(text)) * size * myTFT.CurrentFontWidth() + strlen(text);
   *height = size * myTFT.CurrentFontHeight();
}

void DrawTextCentered(const char* text, uint8_t y, uint8_t size)
{
  uint8_t w, h;
  MeasureText(text, size, &w, &h);
  uint8_t x = (WIDTH - w) / 2;
  myTFT.TFTdrawText(x, y, (char*)text, ST7735_WHITE, ST7735_BLACK, size);
}

void draw()
{
   char text1[] = "Coderdojo";
   char text2[] = "Rule 30";
   char text3[] = "42";

   myTFT.TFTfillScreen(ST7735_BLACK);
   myTFT.TFTFontNum(TFTFont_Default);

   // myTFT.setTextSize(1);
   // myTFT.TFTdrawText(5, 5, text1, ST7735_WHITE, ST7735_BLACK, 1);
   // myTFT.setTextSize(2);
   // myTFT.TFTdrawText(5, 25, text2, ST7735_WHITE, ST7735_BLACK, 2);

   DrawTextCentered(text1, 5, 1);
   DrawTextCentered(text2, 25, 2);
   DrawTextCentered(text3, 50, 3);
   DrawTextCentered(text3, 80, 4);

   myTFT.TFTdrawPixel(89, 9, ST7735_WHITE);
   myTFT.TFTdrawLine(0, 0, WIDTH - 1, HEIGHT - 1, ST7735_RED);
   myTFT.TFTdrawLine(WIDTH - 1, 0, 0, HEIGHT - 1, ST7735_BLUE);
   myTFT.TFTdrawFastVLine(40, 40, 40, ST7735_GREEN);
   myTFT.TFTdrawFastHLine(60, 60, 40, ST7735_YELLOW);

   myTFT.TFTdrawFastHLine(0, 25, WIDTH / 2, ST7735_YELLOW);

   myTFT.TFTdrawFastHLine(0, 100, WIDTH / 2, ST7735_WHITE);
   myTFT.TFTdrawFastHLine(WIDTH / 2, 102, WIDTH / 2, ST7735_YELLOW);

   myTFT.TFTdrawFastVLine(30, 0, HEIGHT, ST7735_BLUE);
   myTFT.TFTdrawFastVLine(WIDTH - 30, 0, HEIGHT, ST7735_BLUE);

   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);

   const uint8_t LINES = HEIGHT, LINE_SIZE = 1;
   const uint8_t LINE_OFFSET = 0, TOP_FIXED = 0, BOTTOM_FIXED = 0;
   const uint8_t TIMES = 4;
   myTFT.TFTsetScrollDefinition(TOP_FIXED, BOTTOM_FIXED, 1);  // bottom-to-top

   for (uint8_t p = 0; p < TIMES; p++)
   {
      uint8_t pos = LINE_OFFSET;
      for (uint8_t i = 0; i < LINES; i++)
      {
         myTFT.TFTVerticalScroll(pos + TOP_FIXED);

         uint8_t yy = (HEIGHT - 2 + pos) % HEIGHT;
         myTFT.TFTdrawPixel(89 + (p * 2), yy, ST7735_WHITE);

         TFT_MILLISEC_DELAY(10);
         pos++;

         // check pos if necessary: 
         // must be < TFT_HEIGHT - TOP_FIXED - BOTTOM_FIXED
         if (pos >= HEIGHT - TOP_FIXED - BOTTOM_FIXED)
         {
            break;
         }
      }
   }

   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);

   myTFT.TFTchangeMode(TFT_Normal_mode);

   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);

   myTFT.TFTfillScreen(ST7735_BLACK);

   TFT_MILLISEC_DELAY(TEST_DELAY1);

   myTFT.TFTdrawText(5, 5, "Red", ST7735_RED, ST7735_BLACK, 1);
   myTFT.TFTdrawText(5, 15, "Green", ST7735_GREEN, ST7735_BLACK, 1);
   myTFT.TFTdrawText(5, 25, "Blue", ST7735_BLUE, ST7735_BLACK, 1);
   myTFT.TFTdrawText(5, 35, "Yellow", ST7735_YELLOW, ST7735_BLACK, 1);
   myTFT.TFTdrawText(5, 45, "Cyan", ST7735_CYAN, ST7735_BLACK, 1);
   myTFT.TFTdrawText(5, 55, "Magenta", ST7735_MAGENTA, ST7735_BLACK, 1);

   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);
   TFT_MILLISEC_DELAY(TEST_DELAY1);
}

int main(void)
{
  setup();
  while (true)
  {
    update();
    draw();
  }

  TFT_MILLISEC_DELAY(TEST_DELAY5);
  myTFT.TFTPowerDown();

  return 0;
}

void setup(void)
{
   TFT_MILLISEC_DELAY(TEST_DELAY1);

   bool bhardwareSPI = true; // true for hardware spi,

   if (bhardwareSPI == true)
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
