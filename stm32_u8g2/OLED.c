//
// Created by jackt on 2023/5/3.
//

#include "OLED.h"
#include "asc.h"
#include "main.h"

void WriteCmd(uint8_t I2C_Command)
{
    HAL_I2C_Mem_Write(&hi2c2,OLED_ADDR,COM,I2C_MEMADD_SIZE_8BIT,&I2C_Command,1,100);
}

void WriteDat(uint8_t I2C_Data)
{
    HAL_I2C_Mem_Write(&hi2c2,OLED_ADDR,DAT,I2C_MEMADD_SIZE_8BIT,&I2C_Data,1,100);
}
void OLED_Init(void)
{
    HAL_Delay(100);

    WriteCmd(0xAE); //display off
    WriteCmd(0x20); //Set Memory Addressing Mode
    WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8); //Set COM Output Scan Direction
    WriteCmd(0x00); //---set low column address
    WriteCmd(0x10); //---set high column address
    WriteCmd(0x40); //--set start line address
    WriteCmd(0x81); //--set contrast control register
    WriteCmd(0xff); //???? 0x00~0xff
    WriteCmd(0xa1); //--set segment re-map 0 to 127
    WriteCmd(0xa6); //--set normal display
    WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F); //
    WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3); //-set display offset
    WriteCmd(0x00); //-not offset
    WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0); //--set divide ratio
    WriteCmd(0xd9); //--set pre-charge period
    WriteCmd(0x22); //
    WriteCmd(0xda); //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb); //--set vcomh
    WriteCmd(0x20); //0x20,0.77xVcc
    WriteCmd(0x8d); //--set DC-DC enable
    WriteCmd(0x14); //
    WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(uint8_t x, uint8_t y)
{
    WriteCmd(0xb0+y);
    WriteCmd(((x&0xf0)>>4)|0x10);
    WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(uint8_t fill_Data)
{
    uint8_t m,n;
    for(m=0;m<8;m++)
    {
        WriteCmd(0xb0+m);		//page0-page1
        WriteCmd(0x00);		//low column start address
        WriteCmd(0x10);		//high column start address
        for(n=0;n<128;n++)
        {
            WriteDat(fill_Data);
        }
    }
}


void OLED_CLS(void)
{
    OLED_Fill(0x00);
}

void OLED_ON(void)
{
    WriteCmd(0X8D);
    WriteCmd(0X14);
    WriteCmd(0XAF);
}

void OLED_OFF(void)
{
    WriteCmd(0X8D);
    WriteCmd(0X10);
    WriteCmd(0XAE);
}


void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize)
{
    uint8_t c = 0,i = 0,j = 0;
    switch(TextSize)
    {
        case 1:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 126)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<6;i++)
                    WriteDat(F6x8[c][i]);
                x += 6;
                j++;
            }
        }break;
        case 2:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i]);
                OLED_SetPos(x,y+1);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i+8]);
                x += 8;
                j++;
            }
        }break;
        default:
        break;
    }
}


void OLED_ShowCN(uint8_t x, uint8_t y, uint8_t N)
{
    uint8_t wm=0;
    unsigned int  adder=32*N;
    OLED_SetPos(x , y);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x,y + 1);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
}


void OLED_ShowCN_Str(uint8_t x , uint8_t y , uint8_t begin , uint8_t num)
{
    uint8_t i;
    for(i=0;i<num;i++){OLED_ShowCN(i*16+x,y,i+begin);}
}


void OLED_DrawBMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t BMP[])
{
    unsigned int j=0;
    uint8_t x,y;

    if(y1%8==0)
        y = y1/8;
    else
        y = y1/8 + 1;
    for(y=y0;y<y1;y++)
    {
        OLED_SetPos(x0,y);
        for(x=x0;x<x1;x++)
        {
            WriteDat(BMP[j++]);
        }
    }
}

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{
    uint8_t c=0,i=0;
    c=chr-' ';//???????
    if(x>128-1){x=0;y=y+2;}
    if(Char_Size ==16)
    {
        OLED_SetPos(x,y);
        for(i=0;i<8;i++)
            WriteDat(F8X16[c*16+i]);
        OLED_SetPos(x,y+1);
        for(i=0;i<8;i++)
            WriteDat(F8X16[c*16+i+8]);
    }
    else {
        OLED_SetPos(x,y);
        for(i=0;i<6;i++)
            WriteDat(F6x8[c][i]);

    }
}
uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}


void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            }else enshow=1;
        }
        OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
}




void OLED_DrawGIF(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1, uint8_t k, int m, uint8_t GIF[][m])
{
    unsigned int j=0;
    uint8_t x,y,i;

    if(y1%8==0) y=y1/8;
    else y=y1/8+1;
    for (i=0;i<k;i++)
    {
        j = 0;
        for(y=y0;y<y1;y++)
        {
            OLED_SetPos(x0,y);

            for(x=x0;x<x1;x++)
            {

                WriteDat(GIF[i][j++]);
            }
        }


    }
}

