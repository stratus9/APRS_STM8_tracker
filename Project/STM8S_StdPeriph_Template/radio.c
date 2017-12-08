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
  SPI_sendByte(0x2a);   //0x2000 - TX direct mode uses gpio1, Modulation source is PIN, GFSK2 modulation 0x2B?
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