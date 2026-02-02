
#ifndef __TOUCH_CTP_H__
#define __TOUCH_CTP_H__
#include "stdio.h"
#include "stm32f4xx.h"
#include "lcd_st7789v.h"

#define I2C1_SCL(x)  \
    if(x) \
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); \
    else \
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		
#define I2C1_SDA(x)  \
    if(x) \
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); \
    else \
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

#define I2C1_SDA_Read() SDA_read_Bit()

#define CTP_ADDR 0x2A // 地址为0x38要移一位

void Touch_GPIO_Config(void);
int SDA_read_Bit(void);
void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_Ack(void);
void I2C1_NoAck(void);

void I2C1_Send_Byte(uint8_t dat);
uint8_t I2C1_Read_Byte(uint8_t ack);
uint8_t I2C1_WaitAck(void);

static uint8_t CTP_Write_Reg(uint8_t startaddr, uint8_t *pbuf, uint32_t len);
uint8_t CTP_Read_Reg(uint8_t *pbuf, uint32_t len);

int GUI_TOUCH_X_MeasureX(void);
int GUI_TOUCH_X_MeasureY(void);

void Touch_Test(void);

#endif
