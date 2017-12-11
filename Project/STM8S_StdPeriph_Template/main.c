/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "radio.h"
#include "SPI.h"
#include "timer.h"
#include "UART.h"
#include "struct.h"
#include "aprs.h"

/* Private defines -----------------------------------------------------------*/
#define AX25_FLAG 0x7E
char tx_buffer[256];
char tx_buffer_head = 0;
char tx_buffer_tail = 0;
APRS_t APRS_d;
gps_t gps_d;
sensors_t sensors_d;

/* Private functions ---------------------------------------------------------*/


void main(void)
{
  /* Select fCPU = 16MHz */
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  
  /* Init UART */
  USART_Initialization();
  
  /* Init SPI */
  SPI_Initialization();
  
  GPIO_Initialization();
   
  Delay(10000);
  Timer1_Init();
  Timer2_Init();
  Timer1_2200Hz();
  enableInterrupts();
  
  Si4463_TX();
  //Si4463_TX();
  /* Infinite loop */
                
  while (1){
    //tx_buffer_string("APRS,TCPXX*,qAX,CWOP-5:@072357z4123.75N/09459.60W_218/004g008t020r000p000P000h43b10104eCumulusFO");
    //tx_buffer_queue(0x7E);
    APRS_PreparePoosition(&APRS_d, &gps_d, &sensors_d);
    APRS_PreparePacket(&APRS_d);
    tx_buffer_string(APRS_d.aprs_buffer);
    
    /* do we have any data to transmit? */
    if (!tx_buffer_empty()) {
      Si4463_TX(); /* enter transmit mode */
          /* wait until everything has been sent */
          while (!tx_buffer_empty()) {
            Delay(100);
          }
    }
    Si4463_RX();
    
    Delay(1000000); 
  }
   
}


void GPIO_Initialization(void){
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);   //Si4463 CS
  GPIO_WriteHigh (GPIOD, GPIO_PIN_2);
  
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);   //
  
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);   //
  GPIO_WriteHigh (GPIOD, GPIO_PIN_4);
}


char tx_buffer_dequeue(void) {

	char c;

	if (tx_buffer_empty()) {
		c = AX25_FLAG;
	} else {
		c = tx_buffer[tx_buffer_head++];
	}
	return c;
}

void tx_buffer_string(char * c){
  while(*c){
    tx_buffer_queue(*c++);
  }
}

void tx_buffer_queue(char c) {

	/* since tx_buffer_tail is an unsigned char, it will roll over
	 * to 0 after it gets to 255, avoiding an overflow of tx_buffer
	 */
	tx_buffer[tx_buffer_tail++] = c;
}

char tx_buffer_empty(void) {

	return (tx_buffer_head == tx_buffer_tail);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/