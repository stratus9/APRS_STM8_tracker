#ifndef __MAIN_H
#define __MAIN_H

/* Private function prototypes -----------------------------------------------*/
void Delay(uint32_t);
void USART_Initialization(void);
void USART_SendString(char *);
void USART_SendChar(char);
void SPI_Initialization(void);
void SPI_wait(void);
void SPI_sendByte(uint8_t);
uint8_t SPI_ReadByte(void);
void SPI_ClearRXBuffer();
uint8_t SPI_RWByte(uint8_t value);
void GPIO_Initialization(void);
uint8_t Si4463_PartInfo();
uint8_t Si4463_FuncInfo();
void Si4463_CS(uint8_t);
uint8_t Si4463_CTS();
void Si4463_ClearInt();
void Si4463_ON();
void Si4463_OFF();
void Si4463_Init();
void Si4463_RF_POWER_UP();
void Si4463_GPIO_PIN_CFG();
void Si4463_RF_GLOBAL_XO_TUNE_2();
void Si4463_RF_GLOBAL_CONFIG_1();
void Si4463_RF_INT_CTL_ENABLE_1();
void Si4463_RF_FRR_CTL_A_MODE_4();
void Si4463_RF_MODEM_MOD_TYPE_12();
void Si4463_RF_MODEM_FREQ_DEV_0_1();
void Si4463_RF_MODEM_TX_RAMP_DELAY_8();
void Si4463_RF_MODEM_BCR_OSR_1_9();
void Si4463_RF_MODEM_AFC_GEAR_7();
void Si4463_RF_MODEM_AGC_CONTROL_1();
void Si4463_RF_MODEM_AGC_WINDOW_SIZE_9();
void Si4463_RF_MODEM_OOK_CNT1_8();
void Si4463_RF_MODEM_RSSI_COMP_1();
void Si4463_RF_MODEM_CLKGEN_BAND_1();
void Si4463_RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12();
void Si4463_RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12();
void Si4463_RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12();
void Si4463_RF_PA_MODE_4();
void Si4463_RF_SYNTH_PFDCP_CPFF_7();
void Si4463_RF_FREQ_CONTROL_INTE_8();
void Si4463_ActivateTX();
void Si4463_ActivateRX();
void Si4463_TX();
void Si4463_RX();
void Timer1_Init();
void Timer1_1200Hz();
void Timer1_2200Hz();
void Timer2_Init();
void Timer2_ISR();
char tx_buffer_dequeue();
void tx_buffer_queue(char);
char tx_buffer_empty(void);

#endif /* __MAIN_H */