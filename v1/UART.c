/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include <stdlib.h>
#include "UART.h"

void UART1_Init_Fast(){
  #define UART1_baud 115200
  #define UART1_BBR1 0x08
  #define UART1_BBR2 0x0A

  UART1->CR1 = 0x00;           //set 8-bit word length, no parity, UART enabled (change for low power)
  UART1->CR3 = 0x00;           //set 1 bit Stop

  UART1->BRR2 = 0x0a;  
  UART1->BRR1 = 0x08; 

   
  UART1->CR2 |= 0x08 | 0x04;    //RX i TX enabled
}

void UART1_DeInit_Fast(){
  /* Clear the Idle Line Detected bit in the status register by a read
  to the UART1_SR register followed by a Read to the UART1_DR register */
  
  (void)UART1->SR;    //3B
  (void)UART1->DR;    //3B
  
  UART1->BRR2 = 0x00;  // Set UART1_BRR2 to reset value 0x00    //4B
  UART1->BRR1 = 0x00;  // Set UART1_BRR1 to reset value 0x00    //4B
  
  UART1->CR1 = 0x00;  // Set UART1_CR1 to reset value 0x00    //4B
  UART1->CR2 = 0x00;  // Set UART1_CR2 to reset value 0x00    //4B
  UART1->CR3 = 0x00;  // Set UART1_CR3 to reset value 0x00    //4B
  UART1->CR4 = 0x00;  // Set UART1_CR4 to reset value 0x00    //4B
  UART1->CR5 = 0x00;  // Set UART1_CR5 to reset value 0x00    //4B
  
  UART1->GTR = 0x00;    //4B
  UART1->PSCR = 0x00;   //4B
}

void USART_Initialization(void){
  UART1_DeInit();
  
  UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, 
              UART1_SYNCMODE_CLOCK_DISABLE,
              UART1_MODE_TXRX_ENABLE);
  UART1_Cmd(ENABLE);
}

void USART_SendString(uint8_t * value){
  while(*value){
    while (!(UART1->SR  & 0x80)){}
    UART1->DR = (*value++);
  }
}

void USART_SendChar(uint8_t value){
  while (!(UART1->SR  & 0x80)){}
  UART1->DR = (value);
}