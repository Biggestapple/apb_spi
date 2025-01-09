
#ifndef __DEFINE_H
#define __DEFINE_H



#define __O  volatile /*!< defines 'write only' permissions     */
#define __IO volatile /*!< defines 'read / write' permissions   */

#define SET   1
#define RESET 0

#define ENABLE  1
#define DISABLE 0

typedef int s32;
typedef short s16;
typedef signed char  s8;

typedef const int sc32; /*!< Read Only */
typedef const short sc16; /*!< Read Only */
typedef const signed char  sc8;  /*!< Read Only */

typedef __IO int vs32;
typedef __IO short vs16;
typedef __IO signed char  vs8;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef const unsigned int uc32; /*!< Read Only */
typedef const unsigned short uc16; /*!< Read Only */
typedef const unsigned char  uc8;  /*!< Read Only */

typedef __IO unsigned int vu32;
typedef __IO unsigned short vu16;
typedef __IO unsigned char  vu8;

/************单位操作************/
#define SET_BIT(port, bit)   (*(__IO unsigned int*)(port) |= (1 << (bit)))
#define CLEAR_BIT(port, bit) (*(__IO unsigned int*)(port) &= ~(1 << (bit)))
/***********比较方便的访问地址***********/
#define u8_reg(addr)   *(u8*)(addr)
#define vu8_reg(addr)  *(vu8*)(addr)
#define u16_reg(addr)   *(u16*)(addr)
#define vu16_reg(addr)  *(vu16*)(addr)
#define u32_reg(addr)  *(u32*)(addr)
#define vu32_reg(addr) *(vu32*)(addr)

#define NUM2VAR(type,addr)   (*((type*)(addr)))//强制把地址转换为type类型的变量
/*****************一些宏操作*******************/
#define int12_2_int16(src ,dst) dst = (src & 0x7ff)|((src &0x800)?0xf800:0)//有符号12位转为16位


#define Image_ADDR (0x20020000)
#ifdef __480P_
#define Image_Size 307200
#define Image_Sector_Size 600
#define X_Size 640
#define Y_Size 480
#endif

#ifdef __720P_
#define Image_Size 786432
#define Image_Sector_Size 1536
#endif

#ifdef __600P_
#define Image_Size 442368
#define Image_Sector_Size 864
#define X_Size 768
#define Y_Size 576
#endif
#define New_Image_ADDR (Image_ADDR+Image_Size)

#define Code_Base  0x20000000UL
#define Code_Size_Addr 0x000F0000UL //神秘代码，由来在readme

#endif
