#include "SPI.h"
#include "GPIO.h"




u8 SPI_Write_Read_Byte(u8 data){
    SPI_CS_L();  
    while(SPI1->SR.TXE==0);
    SPI1->TXDR=data;
    while(SPI1->SR.RXNE==0);
    return SPI1->RXDR;
    SPI_CS_H();   

}

void SPI_Init(SPI_TypeDef* spi,u8 baud){
    spi->CR.bit.EN=1;
    spi->BR.U8_TYPE=baud;//默认16分频
    
}


