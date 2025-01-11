/* 
 * @Author: DaExp 
 * @Date: 2025-01-09 13:00
 * @LastEditTime: 2025-01-11 17:00
 * @Description: 
 * @Copyright (c) 2025 by DaExp, All Rights Reserved. 
 */

#ifndef __SPI_H__
#define __SPI_H__
#include "define.h"
#include "io.h"

#define SPI_BASE_ADDR 0X40002000


/**
  * @brief Serial peripheral interface (SPI1)
  */

typedef struct {                                /*!< (@ 0x40002000) SPI1 Structure                                             */
  
  union {
    __IOM u8 reg;                          /*!< (@ 0x00000000) control register                                           */
    
    struct {
      __IOM u8 EN          : 1;            /*!< [0..0] SPI EN                                                             */
      __IOM u8 CPOL        : 1;            /*!< [1..1] Clock polarity                                                     */
      __IOM u8 CPHA        : 1;            /*!< [2..2] Clock phase                                                        */
      __IOM u8 DFF         : 1;            /*!< [3..3] Data frame format                                                  */
      __IOM u8 LSBFIRST    : 1;            /*!< [4..4] Frame format                                                       */
      __IOM u8 EXNEIR      : 1;            /*!< [5..5] Receive interrupt enable                                           */
      __IOM u8 TXNEIE      : 1;            /*!< [6..6] Send interrupt enable                                              */
      __IOM u8 OVRINIT_EN  : 1;            /*!< [7..7] Slave mode enable                                                  */
    } bit;
  } CR;
  __IM  u8   RESERVED;
  __IM  u16  RESERVED1;
  
  union {
    __IOM u8 reg;                          /*!< (@ 0x00000004) status register                                            */
    
    struct {
      __OM  u8 TXE         : 1;            /*!< [0..0] Transmit buffer empty                                              */
      __OM  u8 RXNE        : 1;            /*!< [1..1] Receive buffer not empty                                           */
            u8             : 6;
    } bit;
  } SR;
  __IM  u8   RESERVED2;
  __IM  u16  RESERVED3;
  
  union {
    __IOM u8 reg;                          /*!< (@ 0x00000008) baud divider                                               */
    
    struct {
      __IOM u8 DR          : 4;            /*!< [3..0] only 4 bits can write , divider                                    */
            u8             : 4;
    } bit;
  } BR;
  __IM  u8   RESERVED4;
  __IM  u16  RESERVED5;
  __IOM u16  TXDR;                         /*!< (@ 0x0000000C) send data region                                           */
  __IM  u16  RESERVED6;
  __IOM u16  RXDR;                         /*!< (@ 0x00000010) receive data region                                        */
} SPI_TypeDef;                                    /*!< Size = 18 (0x12)                                                          */


#define SPI2_BASE_ADDR (0X40003000)
#define SPI1           ((SPI_TypeDef*)SPI_BASE_ADDR)
#define SPI2           ((SPI_TypeDef*)SPI2_BASE_ADDR)
#define SPI_EN         NUM2VAR(SPI_TypeDef, SPI_BASE_ADDR).CR.bit.EN = 1;
#define SPI_DISABLE    NUM2VAR(SPI_TypeDef, SPI_BASE_ADDR).CR.bit.EN = 0;

#define SET_GPIO_BIT(port, bit)   (*(__IO u32*)(port) |= (1 << (bit)))
#define CLEAR_GPIO_BIT(port, bit) (*(__IO u32*)(port) &= ~(1 << (bit)))
#define READ_GPIO_BIT(port, bit)  (*(__IO u32*)(port) &= (1 << (bit)))

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
