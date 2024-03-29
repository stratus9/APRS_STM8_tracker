#ifndef __FLASH_H
#define __FLASH_H

#define TX_DEVIATION_RTTY	26 
#define TX_DEVIATION_APRS	688
#define TX_DEVIATION_APRS_1200	324						// manual pre-emphasis
#define TX_DEVIATION_APRS_2200	589						// manual pre-emphasis

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

void Si4463_Init_AFSK();
void Si4463_MODULATION();
void Si4463_PREAMBLE_DISABLE();
void Si4463_NOSYNC_WORD();
void Si4463_NCO_MODULO();
void Si4463_NCO_DATA_RATE_APRS();
void Si4463_AFSK_FILTER();
void Si4463_FREQ();
void Si4463_DEVIATION();
void Si4463_XO_TUNE();

void SI4060_filter_coeffs();
#endif