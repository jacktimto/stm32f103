//
// Created by jackt on 2023/5/3.
//

#ifndef STM32F103_STM32_U8G2_H
#define STM32F103_STM32_U8G2_H

#include "main.h"
#include "u8g2.h"

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
void draw(u8g2_t *u8g2);
void testDrawPixelToFillScreen(u8g2_t *u8g2);



#endif //STM32F103_STM32_U8G2_H
