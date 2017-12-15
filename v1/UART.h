#ifndef __UART_H
#define __UART_H

void UART1_Init_Fast();
void UART1_DeInit_Fast();
void USART_Initialization(void);
void USART_SendString(uint8_t * value);
void USART_SendChar(uint8_t value);


#endif