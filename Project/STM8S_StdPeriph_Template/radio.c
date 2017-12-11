/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include <stdlib.h>
#include "SPI.h"
#include "radio.h"
#include "timer.h"

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



void Si4463_Init_AFSK(){
  Si4463_OFF();
  Delay(1000);
  Si4463_ON();
  Delay(1000);
  
  Si4463_RF_POWER_UP();
  Si4463_XO_TUNE();
  Si4463_GPIO_PIN_CFG();
  
  Si4463_PREAMBLE_DISABLE();
  Si4463_NOSYNC_WORD();
  Si4463_NCO_MODULO();
  Si4463_NCO_DATA_RATE_APRS();
  Si4463_MODULATION();
  Si4463_AFSK_FILTER();
  Si4463_FREQ();
  Si4463_DEVIATION();
  Si4463_RF_PA_MODE_4();
}

void Si4463_ON(){
  GPIO_WriteLow (GPIOD, GPIO_PIN_4);
}

void Si4463_OFF(){
  GPIO_WriteHigh (GPIOD, GPIO_PIN_4);
}

void Si4463_XO_TUNE(){                  //zostaje
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x00);   //GROUP - group 0x00
  SPI_sendByte(0x02);   //NUM_PROPS - 2 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x46);   //frequency adjustment
  SPI_sendByte(0x00);
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
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

void Si4463_RF_POWER_UP(void){          //zostaje       
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

void Si4463_GPIO_PIN_CFG(){             //zostaje
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

void Si4463_MODULATION(){               //zostaje
#define TX_DIRECT_MODE_TYPE_SYNC 0x00
#define TX_DIRECT_MODE_TYPE_ASYNC 0x80
#define TX_DIRECT_MODE_GPIO_0 0x00<<5
#define TX_DIRECT_MODE_GPIO_1 0x01<<5
#define TX_DIRECT_MODE_GPIO_2 0x02<<5
#define TX_DIRECT_MODE_GPIO_3 0x03<<5
#define MOD_SOURCE_FIFO 0x00<<3
#define MOD_SOURCE_PIN 0x01<<3
#define MOD_SOURCE_RAND 0x02<<3  
#define MOD_TYPE_CW 0x00 
#define MOD_TYPE_OOK 0x01 
#define MOD_TYPE_2FSK 0x02 
#define MOD_TYPE_2GFSK 0x03 
#define MOD_TYPE_4FSK 0x04 
#define MOD_TYPE_4GFSK 0x05 
  
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x01);   //NUM_PROPS - 12 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(TX_DIRECT_MODE_TYPE_SYNC | TX_DIRECT_MODE_GPIO_1 | MOD_SOURCE_PIN | MOD_TYPE_2FSK);   //0x2000 - TX direct mode uses gpio1, Modulation source is PIN, GFSK2 modulation 0x2B?
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_PREAMBLE_DISABLE(){         //zostaje
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x10);   //GROUP - group 0x20
  SPI_sendByte(0x01);   //NUM_PROPS - 12 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x00);
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_NOSYNC_WORD(){              //zostaje
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x11);   //GROUP - group 0x20
  SPI_sendByte(0x01);   //NUM_PROPS - 12 proporties to write
  SPI_sendByte(0x11);   //START_PROP - start from 0x00
  SPI_sendByte(0x01 << 7);
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_NCO_MODULO(){               //zostaje
  //oversampling
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x04);   //NUM_PROPS - 12 proporties to write
  SPI_sendByte(0x06);   //START_PROP - start from 0x00
  SPI_sendByte(0x00);
  SPI_sendByte(0x2D);
  SPI_sendByte(0xC6);
  SPI_sendByte(0xC0);
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_NCO_DATA_RATE_APRS(){       //zostaje
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x20
  SPI_sendByte(0x03);   //NUM_PROPS - 12 proporties to write
  SPI_sendByte(0x03);   //START_PROP - start from 0x00
  SPI_sendByte(0x00);
  SPI_sendByte(0x11);
  SPI_sendByte(0x30);
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}


/*
uint8_t filter[9] = {0x01,0x03,0x08,0x11,0x21,0x36,0x4D,0x60,0x67};			// default
	uint8_t filter[9] = {0x81,0x9F,0xC4,0xEE,0x18,0x3E,0x5C,0x70,0x76};			// UTRAK - 6dB@1200 Hz, 4400 Hz
	uint8_t filter[9] = {0x1d,0xe5,0xb8,0xaa,0xc0,0xf5,0x36,0x6b,0x7f};			// UTRAK - 6dB@1200 Hz, 2400 Hz
	uint8_t filter[9] = {0x07,0xde,0xbf,0xb9,0xd4,0x05,0x40,0x6d,0x7f};			// UTRAK - 3db@1200 Hz, 2400 Hz
	uint8_t filter[9] = {0xfa,0xe5,0xd8,0xde,0xf8,0x21,0x4f,0x71,0x7f};			// UTRAK - LP only, 2400 Hz
	uint8_t filter[9] = {0xd9,0xf1,0x0c,0x29,0x44,0x5d,0x70,0x7c,0x7f};			// UTRAK - LP only, 4800 Hz
	uint8_t filter[9] = {0xd5,0xe9,0x03,0x20,0x3d,0x58,0x6d,0x7a,0x7f};			// UTRAK - LP only, 4400 Hz
	uint8_t filter[9] = {0x19,0x21,0x07,0xC8,0x8E,0x9A,0xFB,0x75,0xAD};			// UBSEDS (FIR python)
	uint8_t filter[9] = {4, 8, 0, 233, 219, 254, 95, 208, 258};				// UBSEDS (FIR python) Low-Pass
	uint8_t filter[9] = {7, 10, 2, 238, 218, 222, 255, 40, 59};				// MY FILTER (FIR python) - no pre-emphasis
	uint8_t filter[9] = {6, 10, 6, 244, 224, 224, 251, 32, 50};				// MY FILTER (FIR python) - 6.14dB ripple
	uint8_t filter[9] = {253, 24, 24, 216, 109, 83, 224, 195, 307};				// MY FILTER (FIR python) - 16000Hz-900Hz-3025Hz
	uint8_t filter[9] = {0xE4,0xC8,0xAD,0xA7,0xC4,0x01,0x4F,0x90,0xAA};			// MY FILTER (FIR python) - experimental
        uint8_t filter[9] = {0x81, 0x9f, 0xc4, 0xee, 0x18, 0x3e, 0x5c, 0x70, 0x76};           //pecan
*/
void Si4463_AFSK_FILTER(){              //zostaje
  uint8_t filter[9] = {0x01,0x03,0x08,0x11,0x21,0x36,0x4D,0x60,0x67};
  //uint8_t filter[9] = {0x19,0x21,0x07,0xC8,0x8E,0x9A,0xFB,0x75,0xAD};			// UBSEDS (FIR python)
  
  Si4463_CS(1);
  SPI_sendByte(0x11);															// SET_PROPERTY (cmd)
  SPI_sendByte(0x20);															// MODEM (group)
  SPI_sendByte(0x09);															// 9 (num_props)
  SPI_sendByte(0x0F);															// MODEM_TX_FILTER_COEFF (start_prop)
  SPI_sendByte(filter[8]);													// (data) - MODEM_TX_FILTER_COEFF_8
  SPI_sendByte(filter[7]);													// (data) - MODEM_TX_FILTER_COEFF_7
  SPI_sendByte(filter[6]);													// (data) - MODEM_TX_FILTER_COEFF_6
  SPI_sendByte(filter[5]);													// (data) - MODEM_TX_FILTER_COEFF_5
  SPI_sendByte(filter[4]);													// (data) - MODEM_TX_FILTER_COEFF_4
  SPI_sendByte(filter[3]);													// (data) - MODEM_TX_FILTER_COEFF_3
  SPI_sendByte(filter[2]);													// (data) - MODEM_TX_FILTER_COEFF_2
  SPI_sendByte(filter[1]);													// (data) - MODEM_TX_FILTER_COEFF_1
  SPI_sendByte(filter[0]);													// (data) - MODEM_TX_FILTER_COEFF_0;
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_FREQ(){                     //zostaje
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x40
  SPI_sendByte(0x01);   //NUM_PROPS - 8 proporties to write
  SPI_sendByte(0x51);   //START_PROP - start from 0x00
  SPI_sendByte(2+8);   //BAND

  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
  
  
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x40);   //GROUP - group 0x40
  SPI_sendByte(0x04);   //NUM_PROPS - 8 proporties to write
  SPI_sendByte(0x00);   //START_PROP - start from 0x00
  SPI_sendByte(0x38);   //0x4000 - Frac-N PLL Synthesizer integer divide number
  SPI_sendByte(0x0D);   //0x4001 - Frac-N PLL fraction number
  SPI_sendByte(0xDD);   //0x4002 - Frac-N PLL fraction number
  SPI_sendByte(0xDD);   //0x4003 - Frac-N PLL fraction number
  Si4463_CS(0);
  
  while(Si4463_CTS() != 0xFF){}
}

void Si4463_DEVIATION(){                //zostaje
#define DEVIATION ((((uint32_t)1 << 19) * 8 * 1300.0)/(2*30000000UL))*2
  Si4463_CS(1);
  SPI_sendByte(0x11);   //SET_PROPERTY Command
  SPI_sendByte(0x20);   //GROUP - group 0x40
  SPI_sendByte(0x03);   //NUM_PROPS - 8 proporties to write
  SPI_sendByte(0x0a);   //START_PROP - start from 0x00
  SPI_sendByte(0x00);   
  SPI_sendByte(0x00);   
  SPI_sendByte(0xB5);     
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
  SPI_sendByte(0x07);   //0x2201 - Configuration of PA output power level
  SPI_sendByte(0x00);   //0x2202 - Configuration of the PA Bias and duty cycle of the TX clock source
  SPI_sendByte(0x3D);   //0x2203 - Configuration of PA ramping parameters
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


void Si4463_TX(){
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  Si4463_OFF();
  Delay(1000);
  Si4463_ON();
  Delay(1000);
  Si4463_Init_AFSK();
  Si4463_ActivateTX();
}

void Si4463_RX(){
  TIM2_ITConfig(TIM2_IT_UPDATE, DISABLE);
  Si4463_OFF();
  Delay(1000);
  Si4463_ON();
  Delay(1000);
  Si4463_Init_AFSK();
  Si4463_ActivateRX();
  
}