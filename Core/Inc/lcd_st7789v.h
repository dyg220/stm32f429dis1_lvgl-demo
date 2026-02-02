#ifndef __LCD_ST7789V_H
#define __LCD_ST7789V_H

#include "main.h"
#include "spi.h"
#include "gpio.h"
#include <stdint.h>
#include "ASCII.h"
#include "GB1616.h" 	//16*16汉字字模

/********************* LCD参数宏定义 *********************/
#define LCD_Width    240    // 屏幕宽度
#define LCD_Height   240    // 屏幕高度
#define LCD_PIXEL    (LCD_Width * LCD_Height) // 总像素数

/********************* 引脚操作宏定义（适配你的Pin定义）*********************/
// 直接调用HAL库函数，与你提供的Pin定义完全对应
#define LCD_CS_LOW()     HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_HIGH()    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET)
#define LCD_RS_LOW()     HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET)  // DC低=发送命令
#define LCD_RS_HIGH()    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)    // DC高=发送数据
#define LCD_BL_ON()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)               // 示例：PB0作为背光控制
#define LCD_BL_OFF()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)             // 示例：PB0作为背光控制

/********************* ST7789V命令宏定义 *********************/
#define ST7789_SLPOUT    0x11  // 退出睡眠模式
#define ST7789_COLMOD    0x3A  // 设置颜色格式
#define ST7789_MADCTL    0x36  // 设置显示方向/扫描模式
#define ST7789_CASET     0x2A  // 设置列地址范围
#define ST7789_RASET     0x2B  // 设置行地址范围
#define ST7789_RAMWR     0x2C  // 写显存数据
#define ST7789_DISPON    0x29  // 开启显示
#define ST7789_RGB565    0x55  // 16位RGB565格式（主流选择）

/********************* 函数声明 *********************/
#define u8 uint8_t 
#define u16 uint16_t 
#define u32 uint32_t 
void LCD_delay(int time);
void Lcd_Initialize(void);
//Lcd高级控制函数
void Lcd_ColorBox(u16 x,u16 y,u16 xLong,u16 yLong,u32 Color);
void DrawPixel(u16 x, u16 y, u32 Color);
void draw_Bat(u16 x,u16 y,u8 power_per);
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void LCD_PutString(unsigned short x, unsigned short y, char *s, unsigned int fColor, unsigned int bColor,unsigned char flag);

// RGB565颜色宏定义（直接调用）
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define WHITE   0xFFFF
#define BLACK   0x0000
#define YELLOW  0xFFE0
#define PURPLE  0xF81F


#endif
