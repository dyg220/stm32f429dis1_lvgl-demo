#include "lcd_st7789v.h"

/********************* 延时函数（简易版，无需SysTick） *********************/
// CubeMX可配置SysTick，此处为快速适配，若需高精度可替换为HAL_Delay()
// static void LCD_Delay_ms(uint32_t ms)
//{
//    uint32_t i, j;
//    for (i = 0; i < ms; i++)
//        for (j = 0; j < 18000; j++)
//            ; // F429 180MHz下，该循环约1ms
//}

/********************* SPI写单字节（调用HAL库SPI函数） *********************/
// 阻塞式发送，适配SPI仅写模式，无需接收
static void SPI_Write_Byte(SPI_HandleTypeDef *hspi, uint8_t data)
{
    // 等待SPI发送缓冲区空
    while (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET);
    // 发送数据
    hspi->Instance->DR = data;
    // 等待SPI发送完成
    while (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_BSY) == SET) ;
}

void LCD_delay(int time)
{
    uint32_t ticks = time * (SystemCoreClock / 1000000); // SystemCoreClock=180MHz
    ticks /= 8;                                          // Cortex-M4 SysTick分频8，需修正
    while (ticks--);
}
static void WriteComm(u16 CMD) // 写命令
{
    LCD_RS_LOW();
    uint8_t cmd_byte = (uint8_t)CMD; // 取低8位，若需高8位则改为(CMD >> 8)
    HAL_SPI_Transmit(&hspi1, &cmd_byte, 1, HAL_MAX_DELAY);
}

static void WriteData(u16 tem_data) // 写数据
{
    LCD_RS_HIGH();
    uint8_t data_byte = (uint8_t)tem_data; // 取低8位，需高8位则改为(tem_data >> 8)
    HAL_SPI_Transmit(&hspi1, &data_byte, 1, HAL_MAX_DELAY);
}

void Lcd_Initialize(void)
{
    LCD_CS_HIGH();
    LCD_CS_LOW();
    LCD_delay(100);

    // PASSWORD
    WriteComm(0xDF); // Password
    WriteData(0x98);
    WriteData(0x51);
    WriteData(0xE9);
    //---------------- PAGE0 --------------
    WriteComm(0xDE);
    WriteData(0x00);
    // VGMP,VGSP,VGMN,VGSN
    WriteComm(0xB7);
    WriteData(0x31);
    WriteData(0x88);
    WriteData(0x31);
    WriteData(0x10);

    // Set_R_GAMMA
    WriteComm(0xC8);
    WriteData(0x3F); // RPA
    WriteData(0x3C);
    WriteData(0x34);
    WriteData(0x2D);
    WriteData(0x2D);
    WriteData(0x2E);
    WriteData(0x2A);
    WriteData(0x2C);
    WriteData(0x2C);
    WriteData(0x2D);
    WriteData(0x2D);
    WriteData(0x2A);
    WriteData(0x2A);
    WriteData(0x2A);
    WriteData(0x28);
    WriteData(0x0E);
    WriteData(0x3F); // RPA
    WriteData(0x3C);
    WriteData(0x34);
    WriteData(0x2D);
    WriteData(0x2D);
    WriteData(0x2E);
    WriteData(0x2A);
    WriteData(0x2C);
    WriteData(0x2C);
    WriteData(0x2D);
    WriteData(0x2D);
    WriteData(0x2A);
    WriteData(0x2A);
    WriteData(0x2A);
    WriteData(0x28);
    WriteData(0x0E);

    // POW_CTRL
    WriteComm(0xB9);
    WriteData(0x33);
    WriteData(0x28);
    WriteData(0xCC);

    // DCDC_SEL
    WriteComm(0xBB);
    WriteData(0x06);
    WriteData(0x7A);
    WriteData(0x30);
    WriteData(0x30);
    WriteData(0x6C);
    WriteData(0x60);
    WriteData(0x50);
    WriteData(0x70);

    // VDDD_CTRL
    WriteComm(0xBC);
    WriteData(0x38);
    WriteData(0x3C);

    // SETSTBA
    WriteComm(0xC0);
    WriteData(0x31);
    WriteData(0x20);

    // SETPANEL(default)
    WriteComm(0xC1);
    WriteData(0x16);

    // SETRGBCYC
    WriteComm(0xC3);
    WriteData(0x08);
    WriteData(0x00);
    WriteData(0x0A);
    WriteData(0x10);
    WriteData(0x08);
    WriteData(0x54);
    WriteData(0x45);
    WriteData(0x71);
    WriteData(0x2C);

    // SETRGBCYC(default)
    WriteComm(0xC4);
    WriteData(0x00);
    WriteData(0xA0);
    WriteData(0x79);
    WriteData(0x0E);
    WriteData(0x0A);
    WriteData(0x16);
    WriteData(0x79);
    WriteData(0x0E);
    WriteData(0x0A);
    WriteData(0x16);
    WriteData(0x79);
    WriteData(0x0E);
    WriteData(0x0A);
    WriteData(0x16);
    WriteData(0x82);
    WriteData(0x00);
    WriteData(0x03);

    // SET_GD(default)
    WriteComm(0xD0);
    WriteData(0x04);
    WriteData(0x0C);
    WriteData(0x6B);
    WriteData(0x0F);
    WriteData(0x07);
    WriteData(0x03);

    // RAMCTRL(default)
    WriteComm(0xD7);
    WriteData(0x00);
    WriteData(0x00);

    //---------------- PAGE2 --------------
    WriteComm(0xDE);
    WriteData(0x02);
    LCD_delay(1);

    // DCDC_SET
    WriteComm(0xB8);
    WriteData(0x19);
    WriteData(0xA0);
    WriteData(0x2F);
    WriteData(0x04);
    WriteData(0x33);

    // SETRGBCYC2
    WriteComm(0xC1);
    WriteData(0x10);
    WriteData(0x66);
    WriteData(0x66);
    WriteData(0x01);

    //---------------- PAGE0 --------------
    WriteComm(0xDE);
    WriteData(0x00);

    // sleep out
    // SSD_Start();//CSn pull low
    WriteComm(0x11); // SLPOUT
    LCD_delay(120);

    //---------------- PAGE2 --------------
    WriteComm(0xDE);
    WriteData(0x02);
    LCD_delay(1);

    // OSCM
    WriteComm(0xC5);
    WriteData(0x01); // 10MHz
    WriteData(0x00);
    WriteData(0x00);
    LCD_delay(1);

    // SETMIPI_2
    WriteComm(0xCA);
    WriteData(0x10);
    WriteData(0x20);
    WriteData(0xF4);
    LCD_delay(1);

    //---------------- PAGE0 --------------
    WriteComm(0xDE);
    WriteData(0x00);
    LCD_delay(1);
    WriteComm(0x3a);
    WriteData(0x05);
    // display on
    WriteComm(0x29); // SLPOUT

    LCD_delay(1);
    WriteComm(0x21);

    LCD_delay(1);
}

void BlockWrite(unsigned int Xstart, unsigned int Xend, unsigned int Ystart, unsigned int Yend)
{
    WriteComm(0x2a);
    WriteData(Xstart >> 8);
    WriteData(Xstart & 0xff);
    WriteData(Xend >> 8);
    WriteData(Xend & 0xff);

    WriteComm(0x2b);
    WriteData(Ystart >> 8);
    WriteData(Ystart & 0xff);
    WriteData(Yend >> 8);
    WriteData(Yend & 0xff);

    WriteComm(0x2c);
}

void Lcd_ColorBox(u16 xStart, u16 yStart, u16 xLong, u16 yLong, u32 Color)
{
    u32 temp;

    BlockWrite(xStart, xStart + xLong - 1, yStart, yStart + yLong - 1);
    LCD_RS_HIGH();
    for (temp = 0; temp < xLong * yLong; temp++)
    {
        uint8_t color_h = (Color >> 8);   // 拆分高8位
        uint8_t color_l = (uint8_t)Color; // 拆分低8位
        // 1. 发送高8位：等价于原SPI_I2S_SendData(SPI1, Color >> 8)
        HAL_SPI_Transmit(&hspi1, &color_h, 1, HAL_MAX_DELAY);
        // 2. 发送低8位：等价于原SPI_I2S_SendData(SPI1, Color)
        HAL_SPI_Transmit(&hspi1, &color_l, 1, HAL_MAX_DELAY);
    }
}

void LCD_Fill_Pic(u16 x, u16 y, u16 pic_H, u16 pic_V, const unsigned char *pic)
{
    unsigned long i;
    uint32_t data_len = (uint32_t)pic_H * pic_V * 2;
    BlockWrite(x, x + pic_H - 1, y, y + pic_V - 1);
    LCD_RS_HIGH();
    for (i = 0; i < pic_H * pic_V * 2; i++)
    {
        HAL_SPI_Transmit(&hspi1, (uint8_t *)pic, data_len, HAL_MAX_DELAY);
    }

    //	for (i = 0; i < pic_H*pic_V*2; i+=2)//把16位图变成24位图，也可以直接取模成24位图
    //	{
    //		SPI_I2S_SendData(SPI1, pic[i]|0X7);
    //		SPI_I2S_SendData(SPI1, (pic[i]<<5)|(pic[i+1]>>3)|0X3);
    //		SPI_I2S_SendData(SPI1, (pic[i+1]<<3)|0X7);
    //	}

    //	for (i = 0; i < pic_H*pic_V*3; i++)//如果直接取模成24位图，则程序改以下以形式
    //	{
    //		SPI_I2S_SendData(SPI1, pic[i]);
    //	}
}
//=============== 在x，y 坐标上打一个颜色为Color的点 ===============//
void DrawPixel(u16 x, u16 y, u32 Color)
{
    BlockWrite(x, x, y, y);
    LCD_RS_HIGH();
    uint8_t color_h = (Color >> 8);   // 高8位
    uint8_t color_l = (uint8_t)Color; // 低8位

    // 发送高8位（等价于原SPI_I2S_SendData(SPI1, Color >> 8)）
    HAL_SPI_Transmit(&hspi1, &color_h, 1, HAL_MAX_DELAY);
    // 发送低8位（等价于原SPI_I2S_SendData(SPI1, Color)）
    HAL_SPI_Transmit(&hspi1, &color_l, 1, HAL_MAX_DELAY);
}
/**********画水平线**********
x0:    水平线起始坐标
x1:    水平线结束坐标
y:     水平线y坐标
Color: 线段颜色
*****************************/
void LCD_draw_HLine(u16 x0, u16 x1, u16 y, u32 Color)
{
    u16 i;
    BlockWrite(x0, x1, y, y);
    LCD_RS_HIGH();
    for (i = 0; i < x1 - x0 + 1; i++)
    {
        uint8_t color_h = (Color >> 8);   // 高8位
        uint8_t color_l = (uint8_t)Color; // 低8位

        // 发送高8位（等价于原SPI_I2S_SendData(SPI1, Color >> 8)）
        HAL_SPI_Transmit(&hspi1, &color_h, 1, HAL_MAX_DELAY);
        // 发送低8位（等价于原SPI_I2S_SendData(SPI1, Color)）
        HAL_SPI_Transmit(&hspi1, &color_l, 1, HAL_MAX_DELAY);
    }
}
/**********画垂直线**********
x:     垂直线x坐标
y0:    垂直线起始坐标
y1:    垂直线结束坐标
Color: 线段颜色
*****************************/
void LCD_draw_VLine(u16 x, u16 y0, u16 y1, u32 Color)
{
    u16 i;
    BlockWrite(x, x, y0, y1);
    LCD_RS_HIGH();
    for (i = 0; i < y1 - y0 + 1; i++)
    {
        uint8_t color_h = (Color >> 8);   // 高8位
        uint8_t color_l = (uint8_t)Color; // 低8位

        // 发送高8位（等价于原SPI_I2S_SendData(SPI1, Color >> 8)）
        HAL_SPI_Transmit(&hspi1, &color_h, 1, HAL_MAX_DELAY);
        // 发送低8位（等价于原SPI_I2S_SendData(SPI1, Color)）
        HAL_SPI_Transmit(&hspi1, &color_l, 1, HAL_MAX_DELAY);
    }
}
/**********电池电量显示**********
x,y :      电池显示位置坐标
power_per: 电池显示的百分比，范围为0~4
*********************************/
void draw_Bat(u16 x, u16 y, u8 power_per)
{
    u8 i, j;
    LCD_draw_VLine(x, y, y + 5, BLACK);
    LCD_draw_VLine(x, y + 5 + 10, y + 5 + 10 + 5, BLACK);

    LCD_draw_HLine(x, x + 28, y, BLACK);
    LCD_draw_HLine(x, x + 28, y + 20, BLACK);

    LCD_draw_VLine(x + 28, y, y + 5, BLACK);
    LCD_draw_VLine(x + 28, y + 5 + 10, y + 5 + 10 + 5, BLACK);

    LCD_draw_HLine(x + 28, x + 28 + 3, y + 5, BLACK);
    LCD_draw_HLine(x + 28, x + 28 + 3, y + 5 + 10, BLACK);

    LCD_draw_VLine(x + 28 + 3, y + 5, y + 5 + 10, BLACK);

    for (j = 0; j < power_per; j++)
    {
        BlockWrite(x + 3, x + 3 + 4, y + 3, y + 3 + 14);
        LCD_RS_HIGH();
        for (i = 0; i < 5 * 15; i++)
        {
            uint8_t color_h = (BLACK >> 8);   // 高8位
            uint8_t color_l = (uint8_t)BLACK; // 低8位
            // 发送高8位（等价于原SPI_I2S_SendData(SPI1, Color >> 8)）
            HAL_SPI_Transmit(&hspi1, &color_h, 1, HAL_MAX_DELAY);
            // 发送低8位（等价于原SPI_I2S_SendData(SPI1, Color)）
            HAL_SPI_Transmit(&hspi1, &color_l, 1, HAL_MAX_DELAY);
        }
        x = x + 6;
    }
}
/**********8*16字体 ASCII码 显示*************
(x,y): 显示字母的起始坐标
num:   要显示的字符:" "--->"~"
fColor 前景色
bColor 背景色
flag:  有背景色(1)无背景色(0)
*********************************************/
void SPILCD_ShowChar(unsigned short x, unsigned short y, unsigned char num, unsigned int fColor, unsigned int bColor,
                     unsigned char flag)
{
    unsigned char temp;
    unsigned int pos, i, j;

    num = num - ' '; // 得到偏移后的值
    i = num * 16;
    for (pos = 0; pos < 16; pos++)
    {
        temp = nAsciiDot[i + pos]; // 调通调用ASCII字体
        for (j = 0; j < 8; j++)
        {
            if (temp & 0x80)
                DrawPixel(x + j, y, fColor);
            else if (flag)
                DrawPixel(x + j, y, bColor); // 如果背景色标志flag为1
            temp <<= 1;
        }
        y++;
    }
}

/**********写一个16x16的汉字*****************
(x,y): 显示汉字的起始坐标
c[2]:  要显示的汉字
fColor 前景色
bColor 背景色
flag:  有背景色(1)无背景色(0)
*********************************************/
void PutGB1616(unsigned short x, unsigned short y, unsigned char c[2], unsigned int fColor, unsigned int bColor, unsigned char flag)
{
    unsigned int i, j, k;
    unsigned short m;
    for (k = 0; k < 64; k++)
    { // 64标示自建汉字库中的个数，循环查询内码
        if ((codeGB_16[k].Index[0] == c[0]) && (codeGB_16[k].Index[1] == c[1]))
        {
            for (i = 0; i < 32; i++)
            {
                m = codeGB_16[k].Msk[i];
                for (j = 0; j < 8; j++)
                {
                    if ((m & 0x80) == 0x80)
                    {
                        DrawPixel(x + j, y, fColor);
                    }
                    else
                    {
                        if (flag)
                            DrawPixel(x + j, y, bColor);
                    }
                    m = m << 1;
                }
                if (i % 2)
                {
                    y++;
                    x = x - 8;
                }
                else
                    x = x + 8;
            }
        }
    }
}
/**********显示一串字*****************
(x,y): 字符串的起始坐标
*s:    要显示的字符串指针
fColor 前景色
bColor 背景色
flag:  有背景色(1)无背景色(0)
*********************************************/
void LCD_PutString(unsigned short x, unsigned short y, char *s, unsigned int fColor, unsigned int bColor,
                   unsigned char flag)
{
    unsigned char l = 0;
    while (*s)
    {
        if ((unsigned char)*s < 0x80)
        {
            SPILCD_ShowChar(x + l * 8, y, *s, fColor, bColor, flag);
            s++;
            l++;
        }
        else
        {
            PutGB1616(x + l * 8, y, (unsigned char *)s, fColor, bColor, flag);
            s += 2;
            l += 2;
        }
    }
}
