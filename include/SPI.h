/* 
 * @Author: DaExp 
 * @Date: 2025-01-09 13:00
 * @LastEditTime: 2025-01-09 17:00
 * @Description: 本文件是SPI底层驱动的头文件，定义了SPI结构
 * @Copyright (c) 2025 by DaExp, All Rights Reserved. 
 */

#ifndef __SPI_H__
#define __SPI_H__
#include "define.h"
#include "io.h"

#define SPI_BASE_ADDR 0X40002000

//-----------------------------------------------------------------
typedef struct
{
    vu32 TXE  : 1;    // SPI发送缓冲区空，发送完成，软件置一
    vu32 RXNE : 1;    // SPI接收缓冲区非空，接收完成，软件清零
    vu32      : 30;   // 保留
} SPI_SR;
typedef union
{
    vu8  U8_TYPE : 4;
    vu32 U32_TYPE;
} SPI_BR;

typedef struct
{
    union
    {
        struct
        {
            vu32 EN         : 1;    // SPI使能
            vu32 CPOL       : 1;    // 时钟极性
            vu32 CPHA       : 1;    // 时钟相位
            vu32 DFF        : 1;    // 数据帧格式,8位还是16位
            vu32 LSBFIRST   : 1;    // 低位在前
            vu32 RXNEIE     : 1;    // 接收中断使能
            vu32 TXNEIE     : 1;    // 发送中断使能
            vu32 OVRINIT_EN : 1;    // 从机模式（别管他）
            vu32            : 24;   // 保留
        } bit;
        vu8 byte;
    } CR;        // 0x00
    SPI_SR SR;   // 0x04
    SPI_BR BR;   // 0x08
    vu32   TXDR : 8;
    vu32        : 24; /*!< (@ 0x0000000C) send data region                                           */
    vu32 RXDR   : 8;
    vu32        : 24; /*!< (@ 0x00000010) receive data region                                        */
} SPI_TypeDef;

#define SPI2_BASE_ADDR (0X40003000)
#define SPI1           ((SPI_TypeDef*)SPI_BASE_ADDR)
#define SPI2           ((SPI_TypeDef*)SPI2_BASE_ADDR)
#define SPI_EN         NUM2VAR(SPI_REG_TYPE, SPI_BASE_ADDR).CR.BIT.EN = 1;
#define SPI_DISABLE    NUM2VAR(SPI_REG_TYPE, SPI_BASE_ADDR).CR.BIT.EN = 0;

#define SET_GPIO_BIT(port, bit)   (*(__IO uint32_t*)(port) |= (1 << (bit)))
#define CLEAR_GPIO_BIT(port, bit) (*(__IO uint32_t*)(port) &= ~(1 << (bit)))
#define READ_GPIO_BIT(port, bit)  (*(__IO uint32_t*)(port) &= (1 << (bit)))

#define SPI_CS_H() P15_Out(1)
#define SPI_CS_L() P15_Out(0)

#define SPI_DO_H()  P11_Out(1)
#define SPI_DO_L()  P11_Out(0)
#define SPI_DI()    P7_In()
#define SPI_CLK_H() P12_Out(1)
#define SPI_CLK_L() P12_Out(0)

/// @brief 初始化SPI
/// @param spi 你自己造的SPI结构体指针
/// @param baud 波特率
void SPI_Init(SPI_TypeDef* spi, u8 baud);

#endif
