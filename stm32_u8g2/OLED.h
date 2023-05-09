//
// Created by jackt on 2023/5/3.
//

#ifndef STM32F103_OLED_H
#define STM32F103_OLED_H

#include "i2c.h"
#define OLED_ADDR 0x78
#define COM 0x00
#define DAT 0x40

void WriteCmd(uint8_t I2C_Command);
void WriteDat(uint8_t I2C_Data);
void OLED_Init(void );
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_Fill(uint8_t fill_Data);
void OLED_CLS(void );
void OLED_ON(void );
void OLED_OFF(void );
void OLED_ShowStr(uint8_t x, uint8_t y, const uint8_t ch[], uint8_t TextSize);
void OLED_ShowCN(uint8_t x, uint8_t y, uint8_t N);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
void OLED_ShowCN_Str(uint8_t x, uint8_t y, uint8_t begin, uint8_t num);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
uint32_t OLED_Pow(uint8_t m, uint8_t n);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2);
void OLED_DrawGIF(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1, uint8_t k, int m, uint8_t GIF[][m]);



#endif //STM32F103_OLED_H
