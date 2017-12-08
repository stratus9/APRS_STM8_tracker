#ifndef __SPI_H
#define __SPI_H

void SPI_Initialization();
void SPI_wait();
void SPI_sendByte(uint8_t value);
uint8_t SPI_ReadByte();
uint8_t SPI_RWByte(uint8_t value);
void SPI_ClearRXBuffer();

#endif