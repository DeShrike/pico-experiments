#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "st7735/ST7735_TFT.hpp"

#define WIDTH	128
#define HEIGHT  160

#define DELAY1 1000
#define DELAY2 50
#define DELAY5 5000
#define FADEDELAY 20
#define LOOPDELAY 5

ST7735_TFT myTFT;

void MeasureText(const char* text, uint8_t size, uint8_t* width, uint8_t* height);
void DrawTextCentered(const char* text, uint8_t y, uint8_t size);
void FadeInTextCentered(const char* text, uint8_t y, uint8_t size, uint8_t r, uint8_t g, uint8_t b);
void FadeOutTextCentered(const char* text, uint8_t y, uint8_t size, uint8_t r, uint8_t g, uint8_t b);

void init();

#define OVERFLOW 63		// Max 63
#define ROW_SIZE (WIDTH + (OVERFLOW * 2))
uint16_t liney;
uint8_t prev_row[ROW_SIZE] = {0};
uint8_t curr_row[ROW_SIZE] = {0};
uint8_t mode = 0;   // 0 = titlescreen / 1 = rule30
uint8_t* curr;
uint8_t* prev;
uint8_t scroll_pos = 0;
const uint8_t LINE_OFFSET = 0, TOP_FIXED = 0, BOTTOM_FIXED = 0;
bool scrolling = 0;

void setup()
{
   liney = 0;
   scrolling = 0;
   scroll_pos = 0;

   curr = curr_row;
   prev = prev_row;
   for (uint8_t i = 0; i < ROW_SIZE; i++)
   {
     curr[i] = 0;
   }

   curr[ROW_SIZE / 2] = 1;
}

void update()
{
  if (mode == 0)
  {
  }
  else
  {
    uint8_t p, q, r;
    uint8_t* t = curr;
    curr = prev;
    prev = t;
    for (uint8_t i = 1; i < ROW_SIZE - 1; i++)
    {
      p = prev[i - 1];
      q = prev[i];
      r = prev[i + 1];
      curr[i] = p ^ (q | r);
    }

    liney++;
    if (liney > 500)
    {
       TFT_MILLISEC_DELAY(DELAY1);
       TFT_MILLISEC_DELAY(DELAY1);	
       TFT_MILLISEC_DELAY(DELAY1);
       mode = 0;
       myTFT.TFTchangeMode(TFT_Normal_mode);

       setup();
    }
  }
}

void draw_title()
{
   char title[] = "Rule 30";
   char subtitle1[] = "Raspberry Pi";
   char subtitle2[] = "Pico 2W";
   char subtext[] = "ST7735";

   myTFT.TFTfillScreen(ST7735_BLACK);
   myTFT.TFTFontNum(TFTFont_Default);

   TFT_MILLISEC_DELAY(DELAY1);
   FadeInTextCentered(title, 50, 2, 255, 255, 0);

   TFT_MILLISEC_DELAY(DELAY1);
   FadeInTextCentered(subtitle1, 80, 1, 0, 0, 255);
   FadeInTextCentered(subtitle2, 90, 1, 0, 0, 255);

   TFT_MILLISEC_DELAY(DELAY1);
   FadeInTextCentered(subtext, 110, 1, 255, 0, 0);

   TFT_MILLISEC_DELAY(DELAY1);
   FadeOutTextCentered(subtext, 110, 1, 255, 0, 0);
   FadeOutTextCentered(subtitle2, 90, 1, 0, 0, 255);
   FadeOutTextCentered(subtitle1, 80, 1, 0, 0, 255);

   TFT_MILLISEC_DELAY(DELAY1);
   FadeOutTextCentered(title, 50, 2, 255, 255, 0);

   TFT_MILLISEC_DELAY(DELAY1);
   mode = 1;
}

uint8_t scroll1()
{
   if (scroll_pos >= HEIGHT - TOP_FIXED - BOTTOM_FIXED)
   {
     return 0;
   }

   if (scrolling == 0)
   {
     myTFT.TFTsetScrollDefinition(TOP_FIXED, BOTTOM_FIXED, 1);  // bottom-to-top
     scrolling = 1;
   }

   myTFT.TFTVerticalScroll(scroll_pos + TOP_FIXED);
   uint8_t yy = (HEIGHT - 2 + scroll_pos) % HEIGHT;

   TFT_MILLISEC_DELAY(10);
   scroll_pos++;
   scroll_pos = scroll_pos % HEIGHT;
   return yy;
   //myTFT.TFTchangeMode(TFT_Normal_mode);
}

void draw()
{
   if (mode == 0)
   {
      draw_title();
      return;
   }

   uint8_t y = liney;
   if (liney > HEIGHT)
   {
     y = scroll1();
   }

   for (uint8_t i = OVERFLOW; i < OVERFLOW + WIDTH; i++)
   {
      myTFT.TFTdrawPixel(i - OVERFLOW, y, curr[i] == 1 ? ST7735_WHITE : ST7735_BLACK);
   }
}

int main(void)
{
  init();
  setup();
  while (true)
  {
    update();
    draw();

    TFT_MILLISEC_DELAY(LOOPDELAY);
  }

  TFT_MILLISEC_DELAY(DELAY5);
  myTFT.TFTPowerDown();

  return 0;
}

void init(void)
{
   TFT_MILLISEC_DELAY(DELAY1);

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

void MeasureText(const char* text, uint8_t size, uint8_t* width, uint8_t* height)
{
   uint16_t l = strlen(text); 
   *width = l * size * myTFT.CurrentFontWidth() + l;
   *height = size * myTFT.CurrentFontHeight();
}

uint16_t Color565(uint16_t r, uint16_t g, uint16_t b)
{
    return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
}

uint16_t AlphaBlend(uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t br, uint8_t bg, uint8_t bb)
{
   float alpha = a / 255.0f;
   float oneminusalpha = 1.0f - alpha;

   uint8_t newR = ((r * alpha) + (oneminusalpha * br));
   uint8_t newG = ((g * alpha) + (oneminusalpha * bg));
   uint8_t newB = ((b * alpha) + (oneminusalpha * bb));

   return Color565(newR, newG, newB);
}

void DrawTextCentered(const char* text, uint8_t y, uint8_t size, uint16_t color)
{
  uint8_t w, h;
  MeasureText(text, size, &w, &h);
  uint8_t x = (WIDTH - w) / 2;
  myTFT.TFTdrawText(x, y, (char*)text, color, ST7735_BLACK, size);
}

void FadeInTextCentered(const char* text, uint8_t y, uint8_t size, uint8_t r, uint8_t g, uint8_t b)
{
  for (uint8_t a = 0; a < 255; a += 5)
  {
     uint16_t color = AlphaBlend(r, g, b, a, 0, 0, 0); 
     DrawTextCentered(text, y, size, color);
     TFT_MILLISEC_DELAY(FADEDELAY);
  }
}

void FadeOutTextCentered(const char* text, uint8_t y, uint8_t size, uint8_t r, uint8_t g, uint8_t b)
{
  for (int16_t a = 255; a >= 0; a -= 5)
  {
     uint16_t color = AlphaBlend(r, g, b, (uint8_t)a, 0, 0, 0); 
     DrawTextCentered(text, y, size, color);
     TFT_MILLISEC_DELAY(FADEDELAY);
  }
}
