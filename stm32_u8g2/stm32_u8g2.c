//
// Created by jackt on 2023/5/3.
//

#include "stm32_u8g2.h"
#include "tim.h"
#include "i2c.h"
#include "OLED.h"

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buffer[128];
    static uint8_t buf_idx;
    uint8_t *data;

    switch (msg)
    {
        case U8X8_MSG_BYTE_INIT:
        {
            /* add your custom code to init i2c subsystem */
            MX_I2C2_Init();
        }
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
        {
            buf_idx = 0;
        }
            break;

        case U8X8_MSG_BYTE_SEND:
        {
            data = (uint8_t *)arg_ptr;

            while (arg_int > 0)
            {
                buffer[buf_idx++] = *data;
                data++;
                arg_int--;
            }
        }
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
        {
            if (HAL_I2C_Master_Transmit(&hi2c2, OLED_ADDR, buffer, buf_idx, 1000) != HAL_OK)
                return 0;
        }
            break;

        default:
            return 0;
    }

    return 1;
}



uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
            HAL_Delay(1);
            break;
    }
    return 1;
}
void u8g2Init(u8g2_t *u8g2)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_gpio_and_delay);
    u8g2_InitDisplay(u8g2);
    u8g2_SetPowerSave(u8g2, 0);
    u8g2_ClearBuffer(u8g2);
}


void draw(u8g2_t *u8g2)
{
    u8g2_ClearBuffer(u8g2);

    u8g2_SetFontMode(u8g2, 1);
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 0, 20, "U");

    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21,8,"8");

    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51,30,"g");
    u8g2_DrawStr(u8g2, 67,30,"\xb2");

    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);

    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");

    u8g2_SendBuffer(u8g2);
    HAL_Delay(1000);
}


void testDrawPixelToFillScreen(u8g2_t *u8g2)
{
    int t = 1000;
    u8g2_ClearBuffer(u8g2);

    for (int j = 0; j < 64; j++)
    {
        for (int i = 0; i < 128; i++)
        {
            u8g2_DrawPixel(u8g2,i, j);
        }
    }
    HAL_Delay(t);
}
