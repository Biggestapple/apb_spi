 /*
 * @Author: DaExp 
 * @Date: 2025-01-09 16:00
 * @LastEditTime: 2025-01-09 17:00
 * @Description: IO 口的定义，还有一些宏
 * @Copyright (c) 2025 by DaExp, All Rights Reserved. 
 */

#ifndef __IO_H
#define __IO_H

#include "define.h"

#define IO_BASE (0x40006000U)


/* =========================================================================================================================== */
/* ================                                           IO                                            ================ */
/* =========================================================================================================================== */


/**
  * @brief IO PORT (IO)
  */

typedef struct {                                /*!< (@ 0x40006000) IO Structure                                             */
  
  union {
    __IOM u32 reg;                         /*!< (@ 0x00000000) IO Output Port                                             */
    
    struct {
      __IOM u32 ODR0       : 1;            /*!< [0..0] Port output data (y = 0..15)                                       */
      __IOM u32 ODR1       : 1;            /*!< [1..1] Port output data (y = 0..15)                                       */
      __IOM u32 ODR2       : 1;            /*!< [2..2] Port output data (y = 0..15)                                       */
      __IOM u32 ODR3       : 1;            /*!< [3..3] Port output data (y = 0..15)                                       */
      __IOM u32 ODR4       : 1;            /*!< [4..4] Port output data (y = 0..15)                                       */
      __IOM u32 ODR5       : 1;            /*!< [5..5] Port output data (y = 0..15)                                       */
      __IOM u32 ODR6       : 1;            /*!< [6..6] Port output data (y = 0..15)                                       */
      __IOM u32 ODR7       : 1;            /*!< [7..7] Port output data (y = 0..15)                                       */
      __IOM u32 ODR8       : 1;            /*!< [8..8] Port output data (y = 0..15)                                       */
      __IOM u32 ODR9       : 1;            /*!< [9..9] Port output data (y = 0..15)                                       */
      __IOM u32 ODR10      : 1;            /*!< [10..10] Port output data (y = 0..15)                                     */
      __IOM u32 ODR11      : 1;            /*!< [11..11] Port output data (y = 0..15)                                     */
      __IOM u32 ODR12      : 1;            /*!< [12..12] Port output data (y = 0..15)                                     */
      __IOM u32 ODR13      : 1;            /*!< [13..13] Port output data (y = 0..15)                                     */
      __IOM u32 ODR14      : 1;            /*!< [14..14] Port output data (y = 0..15)                                     */
      __IOM u32 SPI_CS     : 1;            /*!< [15..15] Port output data (y = 0..15)                                     */
            u32            : 16;
    } bit;
  } OUT;
  
  union {
    __IOM u32 reg;                         /*!< (@ 0x00000004) IO Input Port                                              */
    
    struct {
      __IOM u32 IDR0       : 1;            /*!< [0..0] Port In                                                            */
      __IOM u32 IDR1       : 1;            /*!< [1..1] Port In                                                            */
      __IOM u32 IDR2       : 1;            /*!< [2..2] Port In                                                            */
      __IOM u32 IDR3       : 1;            /*!< [3..3] Port In                                                            */
      __IOM u32 IDR4       : 1;            /*!< [4..4] Port In                                                            */
      __IOM u32 IDR5       : 1;            /*!< [5..5] Port In                                                            */
      __IOM u32 IDR6       : 1;            /*!< [6..6] Port In                                                            */
      __IOM u32 IDR7       : 1;            /*!< [7..7] Port In                                                            */
            u32            : 24;
    } bit;
  } IN;
    union {
    __IOM u32 reg;                         /*!< (@ 0x00000008) interrupt control                                          */
    
    struct {
      __IOM u32 EN         : 1;            /*!< [0..0] Enable                                                             */
            u32            : 31;
    } bit;
  } INT_CR;
} IO_Type;

#define IO ((IO_Type*)IO_BASE)

#define MULTI_LED_EN_MASK 0x01   // LED彩条使能

#define PS2_EN_MASK     (1 << 2)   // PS2使能掩码，
#define PS2_CS_MASK     (1 << 3)


#define PS2_ENABLE_ONCE (IO->Controller_Sel |= PS2_EN_MASK)   // 为1时生成clk
#define PS2_DISABLE (IO->Controller_Sel &= ~PS2_EN_MASK)   // 为1时生成clk
#define PS2_Reading     (vu8)((IO->Controller_Sel & PS2_EN_MASK) >> 2)





#define SET_IO_BIT(port, bit)   (*(__IO u32*)(port) |= (1 << (bit)))
#define CLEAR_IO_BIT(port, bit) (*(__IO u32*)(port) &= ~(1 << (bit)))
#define READ_IO_BIT(port, bit)  (*(__IO u32*)(port) &= (1 << (bit)))

#define P0_Out(a)   IO->OUT.bit.ODR0=a
#define P1_Out(a)   IO->OUT.bit.ODR1=a
#define P2_Out(a)   IO->OUT.bit.ODR2=a
#define P3_Out(a)   IO->OUT.bit.ODR3=a
#define P4_Out(a)   IO->OUT.bit.ODR4=a
#define P5_Out(a)   IO->OUT.bit.ODR5=a
#define P6_Out(a)   IO->OUT.bit.ODR6=a
#define P7_Out(a)   IO->OUT.bit.ODR7=a
#define P8_Out(a)   IO->OUT.bit.ODR8=a
#define P9_Out(a)   IO->OUT.bit.ODR9=a
#define P10_Out(a)  IO->OUT.bit.ODR10=a
#define P11_Out(a)  IO->OUT.bit.ODR11=a
#define P12_Out(a)  IO->OUT.bit.ODR12=a
#define P13_Out(a)  IO->OUT.bit.ODR13=a
#define P14_Out(a)  IO->OUT.bit.ODR14=a
#define P15_Out(a)  IO->OUT.bit.SPI_CS=a

#define P0_In()     IO->IN.bit.IDR0
#define P1_In()     IO->IN.bit.IDR1
#define P2_In()     IO->IN.bit.IDR2
#define P3_In()     IO->IN.bit.IDR3
#define P4_In()     IO->IN.bit.IDR4
#define P5_In()     IO->IN.bit.IDR5
#define P6_In()     IO->IN.bit.IDR6
#define P7_In()     IO->IN.bit.IDR7

#define IO_Interrupt(a)   IO->INT_CR.bit.EN=a


#endif
