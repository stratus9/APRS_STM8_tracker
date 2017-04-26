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

/* Private defines -----------------------------------------------------------*/
#define AX25_FLAG 0x7E
char tx_buffer[256];
char tx_buffer_head = 0;
char tx_buffer_tail = 0;



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
  enableInterrupts();
  
  Si4463_RX();
  Timer1_2200Hz();
  /* Infinite loop */
                
  while (1)
  {
    USART_SendString("loop!\r\n");
    tx_buffer_queue(0x7E);
    tx_buffer_queue(0x7E);
    tx_buffer_queue(0x7E);
    tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);
    tx_buffer_queue('{'); tx_buffer_queue('m'); tx_buffer_queue(':');
    tx_buffer_queue('V'); tx_buffer_queue('A'); tx_buffer_queue('2'); tx_buffer_queue('E'); tx_buffer_queue('P'); tx_buffer_queue('R');
    tx_buffer_queue('@');
    tx_buffer_queue('W'); tx_buffer_queue(' '); tx_buffer_queue('0'); tx_buffer_queue('7'); tx_buffer_queue('5'); tx_buffer_queue(' ');
    tx_buffer_queue('4'); tx_buffer_queue('5'); tx_buffer_queue('.'); tx_buffer_queue('8'); tx_buffer_queue('7'); tx_buffer_queue('0');
    tx_buffer_queue(',');
    tx_buffer_queue('N'); tx_buffer_queue(' '); tx_buffer_queue('4'); tx_buffer_queue('5'); tx_buffer_queue(' ');
    tx_buffer_queue('2'); tx_buffer_queue('7'); tx_buffer_queue('.'); tx_buffer_queue('3'); tx_buffer_queue('0'); tx_buffer_queue('0');
    tx_buffer_queue('/');
    tx_buffer_queue('0');
    tx_buffer_queue(',');
    tx_buffer_queue('0');
    tx_buffer_queue('}');
    tx_buffer_queue(0x7E);
    tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);
    tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);
    tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);
    tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);
    tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);
    tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);tx_buffer_queue(0x7E);
    
    
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

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(uint32_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

void GPIO_Initialization(void){
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);   //Si4463 CS
  GPIO_WriteHigh (GPIOD, GPIO_PIN_2);
  
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);   //
  
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);   //
  GPIO_WriteHigh (GPIOD, GPIO_PIN_4);
}

/**
  * @brief USART Init function
  *     UART1 configured as follow:
  *     - Word Length = 8 Bits
  *     - 1 Stop Bit
  *     - No parity
  *     - BaudRate = 9600 baud
  *     - UART1 Clock enabled
  *     - Polarity Low
  *     - Phase Middle
  *     - Last Bit enabled
  *      Receive and transmit enabled 
  * @param none
  * @retrival none
  */
void USART_Initialization(void){
  UART1_DeInit();
  
  UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, 
              UART1_SYNCMODE_CLOCK_DISABLE,
              UART1_MODE_TXRX_ENABLE);
  UART1_Cmd(ENABLE);
}

void USART_SendString(char * value){
  while(*value){
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET){}
    UART1_SendData8(*value++);
  }
}

void USART_SendChar(char value){
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET){}
  UART1_SendData8(value);
}

/**
  * @brief SPI Init function
  * @param none
  * @retrival none
  */
void SPI_Initialization(void){
  SPI_DeInit();
  /* Initialize SPI in Slave mode  */
  SPI_Init(SPI_FIRSTBIT_MSB, 
           SPI_BAUDRATEPRESCALER_64, 
           SPI_MODE_MASTER, 
           SPI_CLOCKPOLARITY_LOW,
           SPI_CLOCKPHASE_1EDGE, 
           SPI_DATADIRECTION_2LINES_FULLDUPLEX, 
           SPI_NSS_SOFT,
           (uint8_t)0x07);
  /* SPI Enable */
  SPI_Cmd(ENABLE);
}

void SPI_wait(void){
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET){}
}

void SPI_sendByte(uint8_t value){
  SPI_wait();
  SPI_SendData(value);
}

uint8_t SPI_ReadByte(void){
  uint8_t Data = 0;

  /* Wait until the transmit buffer is empty */
  while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
  /* Send the byte */
  SPI_SendData(0x00);

  /* Wait until a data is received */
  while (SPI_GetFlagStatus(SPI_FLAG_BSY)== SET);
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  /* Get the received data */
  Data = SPI_ReceiveData();

  /* Return the shifted data */
  return Data;
}

uint8_t SPI_RWByte(uint8_t value){
  uint8_t Data = 0;

  /* Wait until the transmit buffer is empty */
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET){}
  /* Send the byte */
  SPI_SendData(value);
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  Data = SPI_ReceiveData();
  
  SPI_ClearRXBuffer();
  /* Return the shifted data */
  return Data;
}

void SPI_ClearRXBuffer(){
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == SET){
    SPI_ReceiveData();
  }
}

//========================================================================================
//                      SI4463
//========================================================================================
uint8_t Si4463_CTS(){
  SPI_ClearRXBuffer();
  Delay(100);
  Si4463_CS(1);
  SPI_RWByte(0x44);   //CTS status
  uint8_t tmp1 = SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  Si4463_CS(0);

  return tmp1;
}

void Si4463_CS(uint8_t value){
  SPI_wait();
  Delay(100);           //problem z CS!!
  if(value) GPIO_WriteLow(GPIOD, GPIO_PIN_2);
  else GPIO_WriteHigh(GPIOD, GPIO_PIN_2);
}

void Si4463_Init(void){
  Si4463_RF_POWER_UP();
  Si4463_GPIO_PIN_CFG();
  Si4463_RF_GLOBAL_XO_TUNE_2();
  Si4463_RF_GLOBAL_CONFIG_1();
  Si4463_RF_INT_CTL_ENABLE_1();
  Si4463_RF_FRR_CTL_A_MODE_4();
  Si4463_RF_MODEM_MOD_TYPE_12();
  Si4463_RF_MODEM_FREQ_DEV_0_1();
  Si4463_RF_MODEM_TX_RAMP_DELAY_8();
  Si4463_RF_MODEM_BCR_OSR_1_9();
  Si4463_RF_MODEM_AFC_GEAR_7();
  Si4463_RF_MODEM_AGC_CONTROL_1();
  Si4463_RF_MODEM_AGC_WINDOW_SIZE_9();
  Si4463_RF_MODEM_OOK_CNT1_8();
  Si4463_RF_MODEM_RSSI_COMP_1();
  Si4463_RF_MODEM_CLKGEN_BAND_1();
  Si4463_RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12();
  Si4463_RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12();
  Si4463_RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12();
  Si4463_RF_PA_MODE_4();
  Si4463_RF_SYNTH_PFDCP_CPFF_7();
  Si4463_RF_FREQ_CONTROL_INTE_8();
}

void Si4463_ON(){
  GPIO_WriteLow (GPIOD, GPIO_PIN_4);
}

void Si4463_OFF(){
  GPIO_WriteHigh (GPIOD, GPIO_PIN_4);
}

uint8_t Si4463_PartInfo(void){
  SPI_ClearRXBuffer();
  
  Si4463_CS(1);
  SPI_RWByte(0x01);   //PART_INFO Command  
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
  
  SPI_ClearRXBuffer();
  Si4463_CS(1);
  SPI_RWByte(0x44);   //CTS status
  uint8_t tmp1 = SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  Si4463_CS(0);
  
  return 0;
}

uint8_t Si4463_FuncInfo(void){
  SPI_ClearRXBuffer();
  
  Si4463_CS(1);
  SPI_RWByte(0x10);   //PART_INFO Command  
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
  
  SPI_ClearRXBuffer();
  Si4463_CS(1);
  SPI_RWByte(0x44);   //CTS status
  uint8_t tmp1 = SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  SPI_RWByte(0);
  Si4463_CS(0);
  
  return 0;
}

void Si4463_RF_POWER_UP(void){
  Si4463_CS(1);
  SPI_sendByte(0x02);   //POWER_UP Command
  SPI_sendByte(0x01);   //Boot main application image
  SPI_sendByte(0x00);   //TCXO not used, XTAL used
  SPI_sendByte(0x01);   //30MHz
  SPI_sendByte(0xC9);
  SPI_sendByte(0xC3);
  SPI_sendByte(0x80);
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_GPIO_PIN_CFG(){
  Si4463_CS(1);
  SPI_sendByte(0x13);   //GPIO_PIN_CFG Command
  SPI_sendByte(0x00);   //GPIO0 - Do not modify the behavior of this pin, Disable pullup
  SPI_sendByte(0x04);   //GPIO1 - CMOS input, Disable pullup
  SPI_sendByte(0x03);   //GPIO2 - CMOS output driven high, Disable pullup
  SPI_sendByte(0x02);   //GPIO3 - CMOS output driven low, Disable pullup
  SPI_sendByte(0x00);   //Do not modify the behavior of this pin, Disable pullup
  SPI_sendByte(0x00);   //Do not modify the behavior of this pin, Disable pullup
  SPI_sendByte(0x00);   //Do not modify the behavior of this pin, Disable pullup
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_GLOBAL_XO_TUNE_2(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x00);   //GROUP - group 0x00
  SPI_sendByte(0x02);   //NUM_PROPS - 2 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x52);   //frequency adjustment
  SPI_sendByte(0x00);
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_GLOBAL_CONFIG_1(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x00);   //GROUP - group 0x00
  SPI_sendByte(0x01);   //NUM_PROPS - 1 proporties to write
  SPI_sendByte(0x03);   //START_PROP - start from 0x03
  SPI_sendByte(0x60);
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_INT_CTL_ENABLE_1(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x01);   //GROUP - group 0x01
  SPI_sendByte(0x01);   //NUM_PROPS - 1 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x00);   //interrupts dissabled
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_FRR_CTL_A_MODE_4(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x02);   //GROUP - group 0x02
  SPI_sendByte(0x04);   //NUM_PROPS - 2 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x0A);   //by³o 0x00
  SPI_sendByte(0x00);
  SPI_sendByte(0x00);
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_MOD_TYPE_12(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x0C);   //NUM_PROPS - 12 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x2B);   //0x2000 - TX direct mode uses gpio1, Modulation source is PIN, GFSK2 modulation 0x2B?
  SPI_sendByte(0x80);   //0x2001 - Disable Manchester coding 
  SPI_sendByte(0x07);   //0x2002 - MODEM_DSM_CTRL
  SPI_sendByte(0x01);   //0x2003 - MODEM_DATA_RATE 0x05
  SPI_sendByte(0xD4);   //0x2004 - MODEM_DATA_RATE 0x86
  SPI_sendByte(0xC0);   //0x2005 - MODEM_DATA_RATE 0xA0
  SPI_sendByte(0x00);   //0x2006 - MODEM_TX_NCO_MODE_3 0x01
  SPI_sendByte(0x2D);   //0x2007 - MODEM_TX_NCO_MODE_2 0xC9
  SPI_sendByte(0xC6);   //0x2008 - MODEM_TX_NCO_MODE_1 0xC3
  SPI_sendByte(0xC0);   //0x2009 - MODEM_TX_NCO_MODE_0 0x80
  SPI_sendByte(0x00);   //0x200A - MODEM_FREQ_DEV_2
  SPI_sendByte(0x00);   //0x200B - MODEM_FREQ_DEV_1
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_FREQ_DEV_0_1(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x01);   //NUM_PROPS - 1 proporties to write
  SPI_sendByte(0x0C);   //START_PROP - start from 0x0C
  SPI_sendByte(0xEA);   //17-bit unsigned TX frequency deviation word 0x1F
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_TX_RAMP_DELAY_8(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x08);   //NUM_PROPS - 8 proporties to write
  SPI_sendByte(0x18);   //START_PROP - start from 0x18
  SPI_sendByte(0x01);   //0x2018 - TX ramp-down delay setting
  SPI_sendByte(0x00);   //0x2019 - MDM control
  SPI_sendByte(0x08);   //0x201A - Selects Fixed-IF, Scaled-IF, or Zero-IF mode of RX Modem operation
  SPI_sendByte(0x02);   //0x201B - the IF frequency setting (an 18-bit signed number)
  SPI_sendByte(0x80);   //0x201C - the IF frequency setting (an 18-bit signed number)
  SPI_sendByte(0x00);   //0x201D - the IF frequency setting (an 18-bit signed number)
  SPI_sendByte(0x00);   //0x201E - Specifies three decimator ratios for the Cascaded Integrator Comb (CIC) filter
  SPI_sendByte(0x10);   //0x201F - Specifies three decimator ratios for the Cascaded Integrator Comb (CIC) filter
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_BCR_OSR_1_9(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x09);   //NUM_PROPS - 9 proporties to write
  SPI_sendByte(0x22);   //START_PROP - start from 0x18
  SPI_sendByte(0x00);   //0x2018 - RX BCR/Slicer oversampling rate (12-bit unsigned number)
  SPI_sendByte(0x53);   //0x2019 - RX BCR/Slicer oversampling rate (12-bit unsigned number)
  SPI_sendByte(0x06);   //0x201A - RX BCR NCO offset value (an unsigned 22-bit number)
  SPI_sendByte(0x24);   //0x201B - RX BCR NCO offset value (an unsigned 22-bit number)
  SPI_sendByte(0xDD);   //0x201C - RX BCR NCO offset value (an unsigned 22-bit number)
  SPI_sendByte(0x07);   //0x201D - The unsigned 11-bit RX BCR loop gain value
  SPI_sendByte(0xFF);   //0x201E - The unsigned 11-bit RX BCR loop gain value
  SPI_sendByte(0x02);   //0x201F - RX BCR loop gear control
  SPI_sendByte(0x00);   //0x201F - Miscellaneous control bits for the RX BCR loop
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_AFC_GEAR_7(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x07);   //NUM_PROPS - 7 proporties to write
  SPI_sendByte(0x2C);   //START_PROP - start from 0x2C
  SPI_sendByte(0x00);   //0x202C - RX AFC loop gear control
  SPI_sendByte(0x23);   //0x202D - RX AFC loop wait time control
  SPI_sendByte(0x8F);   //0x202E - Sets the gain of the PLL-based AFC acquisition loop, and provides miscellaneous control bits for AFC functionality
  SPI_sendByte(0xFF);   //0x202F - Sets the gain of the PLL-based AFC acquisition loop, and provides miscellaneous control bits for AFC functionality
  SPI_sendByte(0x00);   //0x2030 - Set the AFC limiter value
  SPI_sendByte(0xB5);   //0x2031 - Set the AFC limiter value
  SPI_sendByte(0xA0);   //0x2032 - Specifies miscellaneous AFC control bits
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_AGC_CONTROL_1(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x01);   //NUM_PROPS - 1 proporties to write
  SPI_sendByte(0x35);   //START_PROP - start from 0x35
  SPI_sendByte(0xE2);   //0x2035 - Miscellaneous control bits for the Automatic Gain Control (AGC) function in the RX Chain
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_AGC_WINDOW_SIZE_9(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x09);   //NUM_PROPS - 9 proporties to write
  SPI_sendByte(0x38);   //START_PROP - start from 0x38
  SPI_sendByte(0x11);   //0x2038 - Specifies the size of the measurement and settling windows for the AGC algorithm
  SPI_sendByte(0x12);   //0x2039 - Sets the decay time of the RF peak detectors
  SPI_sendByte(0x12);   //0x203A - Sets the decay time of the IF peak detectors
  SPI_sendByte(0x00);   //0x203B - Specifies the gain factor of the secondary branch in 4(G)FSK ISI-suppression
  SPI_sendByte(0x02);   //0x203C - Specifies the gain factor of the primary branch in 4(G)FSK ISI-suppression
  SPI_sendByte(0x06);   //0x203D - 16 bit 4(G)FSK slicer threshold
  SPI_sendByte(0x00);   //0x203E - 16 bit 4(G)FSK slicer threshold
  SPI_sendByte(0x00);   //0x203F - 4(G)FSK symbol mapping code
  SPI_sendByte(0x28);   //0x2040 - Configures the attack and decay times of the OOK Peak Detector
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_OOK_CNT1_8(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x08);   //NUM_PROPS - 8 proporties to write
  SPI_sendByte(0x42);   //START_PROP - start from 0x42
  SPI_sendByte(0x84);   //0x2042 - OOK control
  SPI_sendByte(0x03);   //0x2043 - Selects the detector(s) used for demodulation of an OOK signal, or for demodulation of a (G)FSK signal when using the asynchronous demodulator
  SPI_sendByte(0xD6);   //0x2044 - Defines and controls the search period length for the Moving Average and Min-Max detectors
  SPI_sendByte(0x03);   //0x2045 - Defines gain and enable controls for raw / nonstandard mode
  SPI_sendByte(0x00);   //0x2046 - 11 bit eye-open detector threshold
  SPI_sendByte(0x26);   //0x2047 - 11 bit eye-open detector threshold
  SPI_sendByte(0x01);   //0x2048 - Antenna diversity mode settings
  SPI_sendByte(0x80);   //0x2049 - Specifies controls for the Antenna Diversity algorithm
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_RSSI_COMP_1(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x01);   //NUM_PROPS - 1 proporties to write
  SPI_sendByte(0x4E);   //START_PROP - start from 0x4E
  SPI_sendByte(0x40);   //0x204E - RSSI compensation value
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_CLKGEN_BAND_1(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x01);   //NUM_PROPS - 1 proporties to write
  SPI_sendByte(0x51);   //START_PROP - start from 0x51
  SPI_sendByte(0x0D);   //0x2051 - Select PLL Synthesizer output divider ratio as a function of frequency band by³o 0x0A
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x21);   //GROUP - group 0x21
  SPI_sendByte(0x0C);   //NUM_PROPS - 12 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x23);   //0x2100 - Filter coefficients for the first set of RX filter coefficients
  SPI_sendByte(0x17);   
  SPI_sendByte(0xF4);   
  SPI_sendByte(0xC2);   
  SPI_sendByte(0x88);   
  SPI_sendByte(0x50);   
  SPI_sendByte(0x21);   
  SPI_sendByte(0xFF);   
  SPI_sendByte(0xEC);   
  SPI_sendByte(0xE6);   
  SPI_sendByte(0xE8);   
  SPI_sendByte(0xEE);   
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x21);   //GROUP - group 0x21
  SPI_sendByte(0x0C);   //NUM_PROPS - 12 proporties to write
  SPI_sendByte(0x0C);   //START_PROP - start from 0x0C
  SPI_sendByte(0xF6);   //0x210C - Filter coefficients for the first set of RX filter coefficients
  SPI_sendByte(0xFB);   
  SPI_sendByte(0x05);   
  SPI_sendByte(0xC0);   
  SPI_sendByte(0xFF);   
  SPI_sendByte(0x0F);   
  SPI_sendByte(0x23);   //0x2112 - Filter coefficients for the second set of RX filter coefficients
  SPI_sendByte(0x17);   
  SPI_sendByte(0xF4);   
  SPI_sendByte(0xC2);   
  SPI_sendByte(0x88);   
  SPI_sendByte(0x50);   
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x21);   //GROUP - group 0x21
  SPI_sendByte(0x0C);   //NUM_PROPS - 12 proporties to write
  SPI_sendByte(0x18);   //START_PROP - start from 0x18
  SPI_sendByte(0x21);   //0x2118 - Filter coefficients for the first set of RX filter coefficients
  SPI_sendByte(0xFF);   
  SPI_sendByte(0xEC);   
  SPI_sendByte(0xE6);   
  SPI_sendByte(0xE8);   
  SPI_sendByte(0xEE);   
  SPI_sendByte(0xF6);   
  SPI_sendByte(0xFB);   
  SPI_sendByte(0x05);   
  SPI_sendByte(0xC0);   
  SPI_sendByte(0xFF);   
  SPI_sendByte(0x0F);   
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_PA_MODE_4(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x22);   //GROUP - group 0x22
  SPI_sendByte(0x04);   //NUM_PROPS - 4 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x08);   //0x2200 - Selects the PA operating mode, and selects resolution of PA power adjustment (i.e., step size)
  SPI_sendByte(0x01);   //0x2201 - Configuration of PA output power level
  SPI_sendByte(0x00);   //0x2202 - Configuration of the PA Bias and duty cycle of the TX clock source
  SPI_sendByte(0x3D);   //0x2203 - Configuration of PA ramping parameters
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_SYNTH_PFDCP_CPFF_7(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x23);   //GROUP - group 0x23
  SPI_sendByte(0x07);   //NUM_PROPS - 7 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x2C);   //0x2300 - Feed forward charge pump current selection
  SPI_sendByte(0x0E);   //0x2301 - Integration charge pump current selection
  SPI_sendByte(0x0B);   //0x2302 - Gain scaling factors (Kv) for the VCO tuning varactors on both the integrated-path and feed forward path
  SPI_sendByte(0x04);   //0x2303 - Value of resistor R2 in feed-forward path of loop filter
  SPI_sendByte(0x0C);   //0x2304 - Value of capacitor C2 in feed-forward path of loop filter
  SPI_sendByte(0x73);   //0x2305 - Value of capacitors C1 and C3 in feed-forward path of loop filter
  SPI_sendByte(0x03);   //0x2306 - Bias current of the active amplifier in the feed-forward loop filter
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_RF_FREQ_CONTROL_INTE_8(){
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x40);   //GROUP - group 0x40
  SPI_sendByte(0x08);   //NUM_PROPS - 8 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x38);   //0x4000 - Frac-N PLL Synthesizer integer divide number
  SPI_sendByte(0x0D);   //0x4001 - Frac-N PLL fraction number
  SPI_sendByte(0xDD);   //0x4002 - Frac-N PLL fraction number
  SPI_sendByte(0xDD);   //0x4003 - Frac-N PLL fraction number
  SPI_sendByte(0x44);   //0x4004 - EZ Frequency Programming channel step size
  SPI_sendByte(0x44);   //0x4005 - EZ Frequency Programming channel step size
  SPI_sendByte(0x20);   //0x4006 - Set window gating period (in number of crystal reference clock cycles) for counting VCO frequency during calibration
  SPI_sendByte(0xFE);   //0x4007 - Adjust target count for VCO calibration in RX mode
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_ActivateTX(){
  Si4463_CS(1);
  SPI_sendByte(0x31);   //START_TX Command
  SPI_sendByte(0x00);   //CHANNEL
  SPI_sendByte(0x30);   //CONDITION
  SPI_sendByte(0x00);   //TX_LEN
  SPI_sendByte(0x00);   //TX_LEN
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_ActivateRX(){
  Si4463_CS(1);
  SPI_sendByte(0x32);   //START_TX Command
  SPI_sendByte(0x00);   //CHANNEL
  SPI_sendByte(0x00);   //CONDITION
  SPI_sendByte(0x00);   //TX_LEN
  SPI_sendByte(0x00);   //TX_LEN
  SPI_sendByte(0x00);   
  SPI_sendByte(0x00);   
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_ClearInt(){
  Si4463_CS(1);
  SPI_sendByte(0x20);   
  SPI_sendByte(0x00);   
  SPI_sendByte(0x00);   
  SPI_sendByte(0x00);   
  Si4463_CS(0);
  Delay(1000);
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_TX(){
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  Si4463_OFF();
  Delay(1000);
  Si4463_ON();
  Delay(1000);
  Si4463_Init();
  Si4463_ActivateTX();
}

void Si4463_RX(){
  TIM2_ITConfig(TIM2_IT_UPDATE, DISABLE);
  Si4463_OFF();
  Delay(1000);
  Si4463_ON();
  Delay(1000);
  Si4463_Init();
  Si4463_ActivateRX();
  
}


//==========================================================================================================
//                              Timer
//==========================================================================================================

void Timer1_Init(){
  TIM1_DeInit();
  /* Time Base configuration */
  /*
  TIM1_Period = 4095
  TIM1_Prescaler = 0
  TIM1_CounterMode = TIM1_COUNTERMODE_UP
  TIM1_RepetitionCounter = 0
  */
  TIM1_TimeBaseInit(1221, TIM1_COUNTERMODE_UP, 5, 0);
  
  /*
  TIM1_OCMode = TIM1_OCMODE_PWM2
  TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
  TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
  TIM1_Pulse = CCR1_Val
  TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
  TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
  TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
  TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
  */
  TIM1_OC3Init(TIM1_OCMODE_PWM1, 
               TIM1_OUTPUTSTATE_ENABLE, 
               TIM1_OUTPUTNSTATE_ENABLE,
               3, 
               TIM1_OCPOLARITY_LOW, 
               TIM1_OCNPOLARITY_HIGH, 
               TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_RESET); 
  
  //TIM1_CCPreloadControl(ENABLE);
  TIM1_ITConfig(TIM1_IT_COM, DISABLE);

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);
  
  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
}

void Timer1_1200Hz(){
  uint16_t TIM1_Prescaler = 2221;
  /* Set the Prescaler value */
  TIM1->PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
  TIM1->PSCRL = (uint8_t)(TIM1_Prescaler);
}

void Timer1_2200Hz(){
  uint16_t TIM1_Prescaler = 1221;
  /* Set the Prescaler value */
  TIM1->PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
  TIM1->PSCRL = (uint8_t)(TIM1_Prescaler);
}

void Timer2_Init(){
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_2, 13333);
  
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);

  /* TIM1 counter enable */
  TIM2_Cmd(ENABLE);
}

void Timer2_ISR(){
  /* byte to send (LSB first) */
  static unsigned char bits = 0;

  /* number of bits in 'bits' sent */
  static unsigned char bit_count = 0;

  /* count the 1's sent -- used for AX.25 but stuffing */
  static unsigned char ones_count = 0;

  /* Does 'bits' contain the AX.25 Flag? Used to avoid bit stuffing in that case. */
  static unsigned char sending_ax25_flag;

  /* index in the tones array */
  static unsigned char tones_index = 0;

  /* Is the buffer 'bits' empty? If so, get more bits */
  if (bit_count == 0) {

          /* Do we have any bytes that need to be sent? */
          if (tx_buffer_empty()) {

                  /* The buffer is empty. We can stop sending now.
                   * The loop in main() will detect an empty buffer
                   * and disable this interrupt for us.
                   */
                  return;
          }

          bits = tx_buffer_dequeue();
          bit_count = 8;
          sending_ax25_flag = (bits == AX25_FLAG);
  }

  if (!(bits & 0x01)) { /* is current bit 0? */

          /* if the current bit is a 0, then toggle */
          tones_index = !tones_index;
          if(tones_index == 0) Timer1_1200Hz();
          else Timer1_2200Hz();
          ones_count = 0;

          bits >>= 1;
          bit_count--;

  } else if (ones_count++ >= 5 && !sending_ax25_flag) {

          /* bit stuff */
          tones_index = !tones_index;
          if(tones_index == 0) Timer1_1200Hz();
          else Timer1_2200Hz();
          ones_count = 0;

  } else { /* current bit is 1 (don't toggle) */

          bits >>= 1;
          bit_count--;
  }
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
