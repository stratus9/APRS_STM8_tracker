/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include <stdlib.h>
#include "SPI.h"

//==========================================================================================================
//                              SPI
//==========================================================================================================
void SPI_Initialization(){
  //SPI_DeInit();       //25 B
  SPI->CR1 = 0x28 | 0x04;      //MSB first, x64 prescaler, polarity Low, Master
  SPI->CR2 = 0x02 | 0x01;      //Full duplex, Software slave management enabled, Master
  /* SPI Enable */
  SPI->CR1 |= SPI_CR1_SPE; /* Enable the SPI peripheral*/
}

void SPI_wait(){
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET){}  
}

void SPI_sendByte(uint8_t value){
  //SPI_wait();
  while (!(SPI->SR & SPI_FLAG_TXE)){} 
  SPI->DR = value;
}

uint8_t SPI_ReadByte(){
  uint8_t Data = 0;

  /* Wait until the transmit buffer is empty */
  while (!(SPI->SR & SPI_FLAG_TXE));
  /* Send the byte */
  SPI->DR = 0x00;

  /* Wait until a data is received */
  while ((SPI->SR & SPI_FLAG_BSY));
  while (!(SPI->SR & SPI_FLAG_RXNE));
  /* Get the received data */
  Data = SPI->DR;

  /* Return the shifted data */
  return Data;
}

uint8_t SPI_RWByte(uint8_t value){
  uint8_t Data = 0;

  /* Wait until the transmit buffer is empty */
  while (!(SPI->SR & SPI_FLAG_TXE)){}
  /* Send the byte */
  SPI->DR = value;
  while (!(SPI->SR & SPI_FLAG_RXNE));
  Data = SPI->DR;
  
  SPI_ClearRXBuffer();
  /* Return the shifted data */
  return Data;
}

void SPI_ClearRXBuffer(){
  while ((SPI->SR & SPI_FLAG_RXNE)) SPI->DR;
}