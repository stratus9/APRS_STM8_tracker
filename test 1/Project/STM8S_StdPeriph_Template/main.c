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
#define CRC_POLY 0x8408
#define APRS_DEST_CALLSIGN				"APECAN" // APExxx = Pecan device
#define APRS_DEST_SSID                                  0
#define APRS_OWN_CALLSIGN				"SP5RZP"
ax25_t APRS_frame;

#define RADIO_CLK	30000000UL

uint32_t outdiv;
uint8_t initialized = 0;


/* Private functions ---------------------------------------------------------*/


void main(void)
{
  /* Select fCPU = 16MHz */
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);      //118 B
  
  /* Init UART */
  USART_Initialization();       //1182 B -> 85 B
  
  /* Init SPI */
  SPI_Initialization();         //334 B -> 78 B
  
  GPIO_Initialization();        //263 B -> 57 B
   
  Delay(10000);
  Timer1_Init();                //716 B
  Timer2_Init();                //516 B
  enableInterrupts();
  
  //inicjalizacja radia
  initAFSK();                   //2984 B
  Timer1_2200Hz();
  
  
  while(0){
    //Timer1_2200Hz();
    Delay(1000000);
    //Timer1_1200Hz();
    Delay(1000000);
  }
  
  
  
  /* Infinite loop */
  
  
  aprs_encode_message(&APRS_frame, "SP5RZP", 1, "WIDE2-2", 40, "!5225.85NS01654.50E#PHG4480 SPn,W5 Poznan Digi");
  while (1){
   
    
    
    
    //do we have any data to transmit?
    if (APRS_frame.point <= APRS_frame.size) {
      //Si4463_TX(); // enter transmit mode 
      TIM1_Cmd(ENABLE);
      // wait until everything has been sent
      while (!(APRS_frame.end)) {}
      TIM1_Cmd(DISABLE);
      Delay(500000);
      APRS_frame.point = 0;
      APRS_frame.end = 0;

    }
    
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

//======================================================================================
//                              GPIO
//======================================================================================

void GPIO_Initialization(void){
  /*
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);   //Si4463 CS   //13 B
  GPIO_WriteHigh (GPIOD, GPIO_PIN_2);         //9 B
  
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);   //13 B
  
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);   //13 B
  GPIO_WriteHigh (GPIOD, GPIO_PIN_4);           //9 B
  */
  GPIO_Init_Fast();   //57 B
}

void GPIO_Init_Fast(){
  //PD2 - SI4463 CS pin
  GPIOD->CR2 &= ~0x04;  //slow slope 2MHz
  GPIOD->ODR |= 0x04;   //Init High state
  GPIOD->DDR |= 0x04;   //Output
  GPIOD->CR1 |= 0x04;   //Push-Pull
  GPIOD->ODR |= 0x04;   //Init High state
  
  //PC3 - ??
  GPIOC->CR2 &= ~0x08;  //slow slope 2MHz
  GPIOC->DDR |= 0x08;   //Output
  GPIOC->CR1 |= 0x08;   //Push-Pull
  
  //PD4 - ??
  GPIOD->CR2 &= ~0x10;  //slow slope 2MHz
  GPIOD->ODR |= 0x10;   //Init High state
  GPIOD->DDR |= 0x10;   //Output
  GPIOD->CR1 |= 0x10;   //Push-Pull
  GPIOD->ODR |= 0x10;   //Init High state
}

//=======================================================================================
//                              UART
//=======================================================================================
void USART_Initialization(void){
  UART1_DeInit_Fast();     //47 B
  
  //UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE); //1163 B
  UART1_Init_Fast();    //29 B
  UART1_ENABLE();       //19 B -> 9 B
}

void UART1_Init_Fast(){
  #define UART1_baud 115200
  #define UART1_BBR1 0x08
  #define UART1_BBR2 0x0B

  UART1->CR1 = 0x00;           //set 8-bit word length, no parity, UART enabled (change for low power)
  UART1->CR3 = 0x00;           //set 1 bit Stop

  UART1->BRR1 = UART1_BBR1; 
  UART1->BRR2 = UART1_BBR2;       
   
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

void UART1_ENABLE(){
    /* UART1 Enable */
    UART1->CR1 &= (~0x20); 
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

//=======================================================================================
//                              SPI
//=======================================================================================
void SPI_Initialization(void){
  SPI_DeInit();       //25 B
  /* Initialize SPI in Slave mode  */
  //SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_64, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, (uint8_t)0x07);       //305 B
  SPI_Init_Fast();    //8 B
  /* SPI Enable */
  SPI_Cmd(ENABLE);    //29 B
}
//void SPI_Init(SPI_FirstBit_TypeDef FirstBit, SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, SPI_Mode_TypeDef Mode, SPI_ClockPolarity_TypeDef ClockPolarity, SPI_ClockPhase_TypeDef ClockPhase, SPI_DataDirection_TypeDef Data_Direction, SPI_NSS_TypeDef Slave_Management, uint8_t CRCPolynomial)

void SPI_Init_Fast(){
  SPI->CR1 = 0x28 | 0x04;      //MSB first, x64 prescaler, polarity Low, Master
  SPI->CR2 = 0x02 | 0x01;      //Full duplex, Software slave management enabled, Master
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

void Si4463_CS(uint8_t value){
  SPI_wait();
  Delay(100);           //problem z CS!!
  if(value) GPIO_WriteLow(GPIOD, GPIO_PIN_2);
  else GPIO_WriteHigh(GPIOD, GPIO_PIN_2);
}

void Si4463_ON(){
  GPIO_WriteLow (GPIOD, GPIO_PIN_4);
}

void Si4463_OFF(){
  GPIO_WriteHigh (GPIOD, GPIO_PIN_4);
}

void _si_trx_cs_enable(){
  Si4463_CS(1);
}

void _si_trx_cs_disable(){
  Si4463_CS(0);
}

uint8_t spi_bitbang_transfer(uint8_t tmp){
  uint8_t wynik = SPI_RWByte(tmp);
  return wynik;
}

void _si_trx_transfer_nocts(int tx_count, int rx_count, uint8_t *data){
  /* Unused */
  (void)rx_count;

  /* Send command */
  Si4463_CS(1);

  for (int i = 0; i < tx_count; i++) {
    SPI_RWByte(data[i]);
  }

  Si4463_CS(0);
}

void _si_trx_transfer(int tx_count, int rx_count, uint8_t *data){
  uint8_t response;

  /* Send command */
  _si_trx_cs_enable();

  for (int i = 0; i < tx_count; i++) {
    spi_bitbang_transfer(data[i]);
  }

  _si_trx_cs_disable();

  /**
   * Poll CTS. From the docs:
   *
   * READ_CMD_BUFF is used to poll the CTS signal via the SPI bus. The
   * NSEL line should be pulled low, followed by sending the
   * READ_CMD_BUFF command on SDI. While NSEL remains asserted low, an
   * additional eight clock pulses are sent on SCLK and the CTS
   * response byte is read on SDO. If the CTS response byte is not
   * 0xFF, the host MCU should pull NSEL high and repeat the polling
   * procedure.
   */

  do {
    for (int i = 0; i < 200; i++); /* Approx. 20µS */
    _si_trx_cs_enable();

    /* Issue READ_CMD_BUFF */
    spi_bitbang_transfer(SI_CMD_READ_CMD_BUFF);
    response = spi_bitbang_transfer(0xFF);

    /* If the reply is 0xFF, read the response */
    if (response == 0xFF) break;

    /* Otherwise repeat the procedure */
    _si_trx_cs_disable();

  } while (1); /* TODO: Timeout? */

  /**
   * Read response. From the docs:
   *
   * If the CTS response byte is 0xFF, the host MCU should keep NSEL
   * asserted low and provide additional clock cycles on SCLK to read
   * out as many response bytes (on SDO) as necessary. The host MCU
   * should pull NSEL high upon completion of reading the response
   * stream.
   */
  for (int i = 0; i < rx_count; i++) {
    data[i] = spi_bitbang_transfer(0xFF);
  }

  _si_trx_cs_disable();
}

void Si4464_write(uint8_t *data, int tx_count){
  _si_trx_transfer(tx_count, 0, data);
}

void Si4464_read(uint8_t *data, int tx_count){
  _si_trx_transfer(tx_count, 0, data);
}

void initAFSK() {
  // Initialize radio and tune
  Si4464_Init();
  setModemAFSK();
  radioTune(433000000, 0, 1, 0);
  
}

void Si4464_Init(void) {
	// Reset radio)
	radioShutdown();
	Delay(100000);  //>10ms

	// Power up transmitter
	radioTurnOn();	                        // Radio SDN low (power up transmitter)
	Delay(100000);		                // Wait for transmitter to power up
        
        /* Poll for part number */
        while (si_trx_get_part_info() != 17507);

	// Power up (transmits oscillator type)
	uint8_t x3 = (RADIO_CLK >> 24) & 0x0FF;
	uint8_t x2 = (RADIO_CLK >> 16) & 0x0FF;
	uint8_t x1 = (RADIO_CLK >>  8) & 0x0FF;
	uint8_t x0 = (RADIO_CLK >>  0) & 0x0FF;
	uint8_t init_command[] = {0x02, 0x01, 0x01, x3, x2, x1, x0};
	Si4464_write(init_command, 7);
	Delay(200000);

	// Set transmitter GPIOs
	uint8_t gpio_pin_cfg_command[] = {
		0x13,	// Command type = GPIO settings
		0x00,	// GPIO0        0 - PULL_CTL[1bit] - GPIO_MODE[6bit]
		0x04,	// GPIO1        0 - PULL_CTL[1bit] - GPIO_MODE[6bit]
		0x03,	// GPIO2        0 - PULL_CTL[1bit] - GPIO_MODE[6bit]
		0x02,	// GPIO3        0 - PULL_CTL[1bit] - GPIO_MODE[6bit]
		0x00,	// NIRQ
		0x00,	// SDO
		0x00	// GEN_CONFIG
	};
	Si4464_write(gpio_pin_cfg_command, 8);
	Delay(200000);

	initialized = 1;
}

uint16_t si_trx_get_part_info(void){
  uint8_t buffer[3];

  buffer[0] = SI_CMD_PART_INFO;

  _si_trx_transfer(1, 3, buffer);

  return (buffer[1] << 8) | buffer[2];
}

void setFrequency(uint32_t freq, uint16_t shift){
  // Set the output divider according to recommended ranges given in Si4464 datasheet
  uint32_t band = 0;
  if(freq < 705000000UL) {outdiv = 6;  band = 1;};
  if(freq < 525000000UL) {outdiv = 8;  band = 2;};
  if(freq < 353000000UL) {outdiv = 12; band = 3;};
  if(freq < 239000000UL) {outdiv = 16; band = 4;};
  if(freq < 177000000UL) {outdiv = 24; band = 5;};

  // Set the band parameter
  uint32_t sy_sel = 8;
  uint8_t set_band_property_command[] = {0x11, 0x20, 0x01, 0x51, (band + sy_sel)};
  Si4464_write(set_band_property_command, 5);
        
  // Set the PLL parameters
  uint32_t f_pfd = 2 * RADIO_CLK / outdiv;
  uint32_t n = ((uint32_t)(freq / f_pfd)) - 1;
  float ratio = (float)freq/ (float)f_pfd;
  float rest  = ratio - (float)n;

  uint32_t m = (uint32_t)(rest * 524288UL);
  uint32_t m2 = m >> 16;
  uint32_t m1 = (m - m2 * 0x10000) >> 8;
  uint32_t m0 = (m - m2 * 0x10000 - (m1 << 8));
  
  uint32_t channel_increment = 524288 * outdiv * shift / (2 * RADIO_CLK);
  uint8_t c1 = channel_increment / 0x100;
  uint8_t c0 = channel_increment - (0x100 * c1);
  
  uint8_t set_frequency_property_command[] = {0x11, 0x40, 0x04, 0x00, n, m2, m1, m0, c1, c0};
  Si4464_write(set_frequency_property_command, 10);
  
  uint32_t x = ((524288.0 * outdiv * 1300.0)/((2*RADIO_CLK))*2.0);
  uint8_t x2 = (x >> 16) & 0xFF;
  uint8_t x1 = (x >>  8) & 0xFF;
  uint8_t x0 = (x >>  0) & 0xFF;
  uint8_t set_deviation[] = {0x11, 0x20, 0x03, 0x0a, x2, x1, x0};
  Si4464_write(set_deviation, 7);
}

void setShift(uint16_t shift) {
	if(!shift)
		return;

	float units_per_hz = (( 0x40000 * outdiv ) / (float)RADIO_CLK);

	// Set deviation for 2FSK
	uint32_t modem_freq_dev = (uint32_t)(units_per_hz * shift / 2.0 );
	uint8_t modem_freq_dev_0 = 0xFF & modem_freq_dev;
	uint8_t modem_freq_dev_1 = 0xFF & (modem_freq_dev >> 8);
	uint8_t modem_freq_dev_2 = 0xFF & (modem_freq_dev >> 16);

	uint8_t set_modem_freq_dev_command[] = {0x11, 0x20, 0x03, 0x0A, modem_freq_dev_2, modem_freq_dev_1, modem_freq_dev_0};
	Si4464_write(set_modem_freq_dev_command, 7);
}

void setModemAFSK(void) {
	// Disable preamble
	uint8_t disable_preamble[] = {0x11, 0x10, 0x01, 0x00, 0x00};
	Si4464_write(disable_preamble, 5);

	// Do not transmit sync word
	uint8_t no_sync_word[] = {0x11, 0x11, 0x01, 0x00, (0x01 << 7)};
	Si4464_write(no_sync_word, 5);

	// Setup the NCO modulo and oversampling mode
	uint32_t s = RADIO_CLK / 10;
	uint8_t f3 = (s >> 24) & 0xFF;
	uint8_t f2 = (s >> 16) & 0xFF;
	uint8_t f1 = (s >>  8) & 0xFF;
	uint8_t f0 = (s >>  0) & 0xFF;
	uint8_t setup_oversampling[] = {0x11, 0x20, 0x04, 0x06, f3, f2, f1, f0};
	Si4464_write(setup_oversampling, 8);

	// Setup the NCO data rate for APRS
        uint32_t speed = 4400;
	//uint8_t setup_data_rate[] = {0x11, 0x20, 0x03, 0x03, (uint8_t)(speed >> 16), (uint8_t)(speed >> 8), (uint8_t)speed};
	uint8_t setup_data_rate[] = {0x11, 0x20, 0x03, 0x03, 0x00, 0x11, 0};
        Si4464_write(setup_data_rate, 7);

	// Use 2GFSK from async GPIO1
	uint8_t use_2gfsk[] = {0x11, 0x20, 0x01, 0x00, 0x2B};   //2B
	Si4464_write(use_2gfsk, 5);

	// Set AFSK filter
	uint8_t coeff[] = {0x81, 0x9f, 0xc4, 0xee, 0x18, 0x3e, 0x5c, 0x70, 0x76};
	uint8_t i;
	for(i=0; i<sizeof(coeff); i++) {
		uint8_t msg[] = {0x11, 0x20, 0x01, 0x17-i, coeff[i]};
		Si4464_write(msg, 5);
	}
}

void setModemOOK(void) {
	// Use OOK from async GPIO1
	uint8_t use_ook[] = {0x11, 0x20, 0x01, 0x00, 0xA9};
	Si4464_write(use_ook, 5);
}

void setModem2FSK(void) {
	// use 2FSK from async GPIO1
	uint8_t use_2fsk[] = {0x11, 0x20, 0x01, 0x00, 0xAA};
	Si4464_write(use_2fsk, 5);
}

void setModem2GFSK(uint32_t speed) {
	// Disable preamble
	uint8_t disable_preamble[] = {0x11, 0x10, 0x01, 0x00, 0x00};
	Si4464_write(disable_preamble, 5);

	// Do not transmit sync word
	uint8_t no_sync_word[] = {0x11, 0x11, 0x01, 0x00, (0x01 << 7)};
	Si4464_write(no_sync_word, 5);

	// Setup the NCO modulo and oversampling mode
	uint32_t s = RADIO_CLK / 10;
	uint8_t f3 = (s >> 24) & 0xFF;
	uint8_t f2 = (s >> 16) & 0xFF;
	uint8_t f1 = (s >>  8) & 0xFF;
	uint8_t f0 = (s >>  0) & 0xFF;
	uint8_t setup_oversampling[] = {0x11, 0x20, 0x04, 0x06, f3, f2, f1, f0};
	Si4464_write(setup_oversampling, 8);

	// Setup the NCO data rate for 2GFSK
	uint8_t setup_data_rate[] = {0x11, 0x20, 0x03, 0x03, (uint8_t)(speed >> 16), (uint8_t)(speed >> 8), (uint8_t)speed};
	Si4464_write(setup_data_rate, 7);

	// Use 2GFSK from async GPIO1
	uint8_t use_2gfsk[] = {0x11, 0x20, 0x01, 0x00, 0x2B};
	Si4464_write(use_2gfsk, 5);
}

void setPowerLevel(int8_t level) {
	// Set the Power
	uint8_t set_pa_pwr_lvl_property_command[] = {0x11, 0x22, 0x01, 0x01, dBm2powerLvl(level)};
	Si4464_write(set_pa_pwr_lvl_property_command, 5);
}

void startTx(uint16_t size) {
	uint8_t change_state_command[] = {0x31, 0x00, 0x30, (size >> 8) & 0x1F, size & 0xFF};
	Si4464_write(change_state_command, 5);
}

void stopTx(void) {
	uint8_t change_state_command[] = {0x34, 0x03};
	Si4464_write(change_state_command, 2);
}

uint8_t dBm2powerLvl(int32_t dBm) {
	if(dBm < -35) {
		return 0;
	} else if(dBm < -7) {
		return (uint8_t)((2*dBm+74)/15);
	} else if(dBm < 2) {
		return (uint8_t)((2*dBm+26)/3);
	} else if(dBm < 8) {
		return (uint8_t)((5*dBm+20)/3);
	} else if(dBm < 13) {
		return (uint8_t)(3*dBm-4);
	} else if(dBm < 18) {
		return (uint8_t)((92*dBm-1021)/5);
	} else {
		return 127;
	}
}

void radioShutdown(void) {
	GPIO_WriteHigh (GPIOD, GPIO_PIN_4);
	initialized = 0;
}

void radioTurnOn(void) {
	GPIO_WriteLow (GPIOD, GPIO_PIN_4);
}

/**
 * Tunes the radio and activates transmission.
 * @param frequency Transmission frequency in Hz
 * @param shift Shift of FSK in Hz
 * @param level Transmission power level in dBm
 */
uint8_t radioTune(uint32_t frequency, uint16_t shift, int8_t level, uint16_t size) {
	if(!inRadioBand(frequency)) return 0;

	setFrequency(frequency, shift);	// Set frequency
	setShift(shift);		// Set shift
	setPowerLevel(level);		// Set power level

	startTx(size);
	return 1;
}

int8_t Si4464_getTemperature(void) {
  /*
	uint8_t txData[2] = {0x14, 0x10};
	uint8_t rxData[8];
	Si4464_read(txData, 2, rxData, 8);
	uint16_t adc = rxData[7] | ((rxData[6] & 0x7) << 8);
	return (899*adc)/4096 - 293;
  */
  return 0;
}


//===============================================================================================
//                              APRS
//===============================================================================================

void ax25_init(ax25_t *packet){
  packet->max_size = 500;
  packet->size = 0;
  packet->point = 0;
}

void ax25_add_header(ax25_t *packet, uint8_t *callsign, uint8_t ssid, uint8_t *path, uint16_t preamble){
  uint8_t i, j;
  uint8_t tmp[8];
  packet->size = 0;

  // Send preamble ("a bunch of 0s")
  for(i=0; i<preamble; i++) ax25_add_sync(packet);

  // Send flag
  //for(uint8_t i=0; i<preamble; i++) ax25_add_flag(packet);
  ax25_add_flag(packet);

  ax25_add_path(packet, APRS_DEST_CALLSIGN, APRS_DEST_SSID, 0);	// Destination callsign
  ax25_add_path(packet, callsign, ssid, path[0] == 0 || path == 0);	// Source callsign

  // Parse path
  for(i=0, j=0; (path[i-1] != 0 || i == 0) && path != 0; i++) {
    if(path[i] == ',' || path[i] == 0) { // Found block in path
      if(!j) // Block empty
              break;

      // Filter Path until '-'
      tmp[j] = 0;
      uint8_t p[8];
      uint8_t t;
      for(t=0; t<j && tmp[t] != '-'; t++)
              p[t] = tmp[t];
      p[t] = 0;

      // Filter TTL
      uint8_t s = ((tmp[t] == '-' ? tmp[++t] : tmp[--t])-48) & 0x7;

      if(s != 0)
              ax25_add_path(packet, p, s, path[i] == 0);
      j = 0;

    } else {
      tmp[j++] = path[i];
    }
  }

  // Control field: 3 = APRS-UI frame
  ax25_add_byte(packet, 0x03);

  // Protocol ID: 0xf0 = no layer 3 data
  ax25_add_byte(packet, 0xf0);
}

void ax25_add_footer(ax25_t *packet){
  // Save the crc so that it can be treated it atomically
  uint16_t final_crc = ax25_CRC(packet);

  // Send CRC
  ax25_add_byte(packet, ~(final_crc & 0xff));
  final_crc >>= 8;
  ax25_add_byte(packet, ~(final_crc & 0xff));

  // Signal the end of frame
  ax25_add_flag(packet);
}

uint16_t ax25_CRC(ax25_t *packet){              //--------------NIEPEWNE
  /*
  uint8_t i;
  uint8_t j;
  uint16_t crc_reg = 0xFFFF;

  // No data so return zeros 
  if (packet->size == 0)
      return (~crc_reg);

  // Loop through each byte 
  for (i = 0; i < packet->size; i++) {
    // Loop through each bit 
    for (j=0; j < 8; j++, packet->data[i] >>= 1) {
      if ((crc_reg & 0x0001) ^ (packet->data[i] & 0x0001)) crc_reg = (crc_reg >> 1) ^ CRC_POLY;
       else  crc_reg >>= 1;
    }
  }
  // Return 1's complement
  packet->crc = (~crc_reg);
  return (~crc_reg);
  */
  return 0xffff;
}

void ax25_add_sync(ax25_t *packet){
  if(packet->size >= packet->max_size) return; // Prevent buffer overrun
  packet->data[packet->size++] = 0x00;
}

void ax25_add_flag(ax25_t *packet){
  if(packet->size >= packet->max_size) return; // Prevent buffer overrun
  packet->data[packet->size++] = AX25_FLAG;
}

void ax25_add_string(ax25_t *packet, uint8_t *string){
  int i;
  for(i=0; string[i]; i++) {
    ax25_add_byte(packet, string[i]);
  }
}

void ax25_add_path(ax25_t *packet, uint8_t *callsign, uint8_t ssid, uint8_t last){
  uint8_t j;

  // Transmit callsign
  for(j = 0; callsign[j]; j++) ax25_add_byte(packet, callsign[j] << 1);

  // Transmit pad
  for( ; j < 6; j++) ax25_add_byte(packet, ' ' << 1);

  // Transmit SSID. Termination signaled with last bit = 1
  ax25_add_byte(packet, ('0' + ssid) << 1 | (last & 0x1));
}

void ax25_add_byte(ax25_t *packet, uint8_t byte){
  if(packet->size >= packet->max_size) return; // Prevent buffer overrun
  packet->data[packet->size++] = byte;
}

uint32_t aprs_encode_message(ax25_t* buffer, uint8_t * callsign, uint8_t ssid, uint8_t * path, uint16_t preamble, uint8_t *text){
  ax25_init(buffer);

  // Encode APRS header
  ax25_add_header(buffer, callsign, ssid, path, preamble);
  ax25_add_byte(buffer, ':');              //identyfikator typu wiadomoœci
  ax25_add_byte(buffer, ':');
  ax25_add_string(buffer, "         ");
  ax25_add_byte(buffer, ':');
  ax25_add_string(buffer, text);
  ax25_add_byte(buffer, '{');
  ax25_add_byte(buffer, '1');

  // Send footer
  ax25_add_footer(buffer);

  return buffer->size;
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
  TIM1_TimeBaseInit(1032, TIM1_COUNTERMODE_UP, 6, 0);
  
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
               3,    //by³o 3
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
  uint16_t TIM1_Prescaler = 1896;
  /* Set the Prescaler value */
  TIM1->PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
  TIM1->PSCRL = (uint8_t)(TIM1_Prescaler);
}

void Timer1_2200Hz(){
  uint16_t TIM1_Prescaler = 1044;
  /* Set the Prescaler value */
  TIM1->PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
  TIM1->PSCRL = (uint8_t)(TIM1_Prescaler);
}

void Timer2_Init(){
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_2, 6630);    //1200 = X/(2*6630) Fclk = 15.912.000 ~fcpu
  
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);

  /* TIM1 counter enable */
  TIM2_Cmd(ENABLE);
}

void Timer2_ISR(){
  //GPIOC->ODR ^= 0x08;   //Push-Pull
  //GPIO_WriteReverse(GPIOC, GPIO_PIN_3);
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
          if (APRS_frame.point >= APRS_frame.size) {
                  APRS_frame.end = 1;
                  /* The buffer is empty. We can stop sending now.
                   * The loop in main() will detect an empty buffer
                   * and disable this interrupt for us.
                   */
                  return;
          }

          bits = APRS_frame.data[APRS_frame.point++];
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
/*
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

	// since tx_buffer_tail is an unsigned char, it will roll over
        // to 0 after it gets to 255, avoiding an overflow of tx_buffer
	//
	tx_buffer[tx_buffer_tail++] = c;
}

char tx_buffer_empty(void) {

	return (tx_buffer_head == tx_buffer_tail);
}
*/


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
