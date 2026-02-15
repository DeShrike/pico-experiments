#ifndef _UTILS_H_
#define _UTILS_H_

#define FADEDELAY 10

void MeasureText(const char* text, uint8_t size, uint8_t* width, uint8_t* height);
void DrawTextCentered(const char* text, uint8_t y, uint8_t size);
void FadeInTextCentered(const char* text, uint8_t y, uint8_t size, uint8_t r, uint8_t g, uint8_t b);
void FadeOutTextCentered(const char* text, uint8_t y, uint8_t size, uint8_t r, uint8_t g, uint8_t b);
uint16_t Color565(uint16_t r, uint16_t g, uint16_t b);
uint16_t Color565_Reversed(uint16_t r, uint16_t g, uint16_t b);
uint16_t AlphaBlend(uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t br, uint8_t bg, uint8_t bb);

#endif

