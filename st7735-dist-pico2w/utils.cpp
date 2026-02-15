#include "pico/stdlib.h"
#include "utils.hpp"
#include "st7735/ST7735_TFT.hpp"

extern ST7735_TFT myTFT;

void MeasureText(const char* text, uint8_t size, uint8_t* width, uint8_t* height)
{
   uint16_t l = strlen(text);
   *width = l * size * myTFT.CurrentFontWidth() + l;
   *height = size * myTFT.CurrentFontHeight();
}

// RRRRRRRR GGGGGGGG BBBBBBBB > RRRRR GGGGGG BBBBB
uint16_t Color565(uint16_t r, uint16_t g, uint16_t b)
{
    return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
}

// RRRRRRRR GGGGGGGG BBBBBBBB > BBBBB GGGGGG RRRRR
uint16_t Color565_Reversed(uint16_t r, uint16_t g, uint16_t b)
{
    uint16_t c = ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
    return ((c & 0xFF00) >> 8) | ((c & 0x00FF) << 8 );
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
  uint8_t x = (myTFT.Width() - w) / 2;
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
