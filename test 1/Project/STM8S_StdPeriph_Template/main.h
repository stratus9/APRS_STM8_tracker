#ifndef __MAIN_H
#define __MAIN_H

typedef struct {
  uint8_t data[512];		// Data
  uint16_t size;		// Packet size in bits
  uint16_t max_size;	        // Max. Packet size in bits (size of modem packet)
  uint16_t crc;			// CRC
  uint16_t point;             // tx pointer
  uint8_t end;
} ax25_t;

typedef enum {
	TEL_SATS,
	TEL_TTFF,
	TEL_VBAT,
	TEL_VSOL,
	TEL_PBAT,
	TEL_ISOL,
	TEL_PRESS,
	TEL_TEMP,
	TEL_HUM
} telemetry_t;

/**
 * =============================================================================
 *                         COMMAND DEFINITIONS
 * =============================================================================
 */

/**
 * Si Boot Commands
 */
enum {
  SI_CMD_POWER_UP		= 0x02,
};
/**
 * Si Common Commands
 */
enum {
  SI_CMD_NOP			= 0x00,
  SI_CMD_PART_INFO		= 0x01,
  SI_CMD_FUNC_INFO		= 0x10,
  SI_CMD_SET_PROPERTY		= 0x11,
  SI_CMD_GET_PROPERTY		= 0x12,
  SI_CMD__PIN_CFG		= 0x13,
  SI_CMD_FIFO_INFO		= 0x15,
  SI_CMD_GET_INT_STATUS		= 0x20,
  SI_CMD_REQUEST_DEVICE_STATE	= 0x33,
  SI_CMD_CHANGE_STATE		= 0x34,
  SI_CMD_READ_CMD_BUFF		= 0x44,
  SI_CMD_FRR_A_READ		= 0x50,
  SI_CMD_FRR_B_READ		= 0x51,
  SI_CMD_FRR_C_READ		= 0x53,
  SI_CMD_FRR_D_READ		= 0x57,
};
/**
 * Si Tx Commands
 */
enum {
  SI_CMD_START_TX		= 0x31,
  SI_CMD_WRITE_TX_FIFO		= 0x66,
};
/**
 * Si Rx Commands
 */
enum {
  SI_CMD_PACKET_INFO		= 0x16,
  SI_CMD_GET_MODEM_STATUS	= 0x22,
  SI_CMD_START_RX		= 0x32,
  SI_CMD_RX_HOP			= 0x36,
  SI_CMD_READ_RX_FIFO		= 0x77,
};
/**
 * Si Advanced Commands
 */
enum {
  SI_CMD_GET_ADC_READING	= 0x14,
  SI_CMD_PROTOCOL_CFG		= 0x18,
  SI_CMD_GET_PH_STATUS		= 0x21,
  SI_CMD_GET_CHIP_STATUS	= 0x23,
};

/**
 * =============================================================================
 *                         COMMAND ARGUMENTS
 * =============================================================================
 */

/**
 * Si Power Up
 */
enum {
  SI_POWER_UP_FUNCTION		= 0x01,
  SI_POWER_UP_XTAL		= 0x00,
  SI_POWER_UP_TCXO		= 0x01,
};

/**
 * Si GPIO configuration
 */
typedef uint8_t si_gpio_t;
enum {
  SI_GPIO_PIN_CFG_PULL_ENABLE 			= 0x40, /* enable or disable pull-up resistor */
  SI_GPIO_PIN_CFG_GPIO_MODE_DONOTHING		= 0x00, /* pin behaviour is not changed */
  SI_GPIO_PIN_CFG_GPIO_MODE_TRISTATE		= 0x01, /* input and output drivers are disabled */
  SI_GPIO_PIN_CFG_GPIO_MODE_DRIVE0		= 0x02, /* CMOS output "low" */
  SI_GPIO_PIN_CFG_GPIO_MODE_DRIVE1		= 0x03, /* CMOS output "high" */
  SI_GPIO_PIN_CFG_GPIO_MODE_INPUT		= 0x04, /* GPIO is input, for TXDATA etc, function is not configured here */
  SI_GPIO_PIN_CFG_GPIO_MODE_32K_CLK		= 0x05, /* outputs the 32kHz CLK when selected in CLK32_CLK_SEL */
  SI_GPIO_PIN_CFG_GPIO_MODE_BOOT_CLK		= 0x06, /* outputs boot clock when SPI_ACTIVE */
  SI_GPIO_PIN_CFG_GPIO_MODE_DIV_CLK		= 0x07, /* outputs divided xtal clk */
  SI_GPIO_PIN_CFG_GPIO_MODE_CTS			= 0x08, /* output, '1' when device is ready to accept new command */
  SI_GPIO_PIN_CFG_GPIO_MODE_INV_CNT		= 0x09, /* output, inverted CTS */
  SI_GPIO_PIN_CFG_GPIO_MODE_CMD_OVERLAP		= 0x0a, /* output, '1' if a command was issued while not ready */
  SI_GPIO_PIN_CFG_GPIO_MODE_SDO			= 0x0b, /* output, serial data out for SPI */
  SI_GPIO_PIN_CFG_GPIO_MODE_POR			= 0x0c, /* output, '0' while in POR state */
  SI_GPIO_PIN_CFG_GPIO_MODE_CAL_WUT		= 0x0d, /* output, '1' on expiration of wake up timer */
  SI_GPIO_PIN_CFG_GPIO_MODE_WUT			= 0x0e, /* wake up timer output */
  SI_GPIO_PIN_CFG_GPIO_MODE_EN_PA		= 0x0f, /* output, '1' when PA is enabled */
  SI_GPIO_PIN_CFG_GPIO_MODE_TX_DATA_CLK		= 0x10, /* data clock output, for TX direct sync mode */
  SI_GPIO_PIN_CFG_GPIO_MODE_TX_DATA		= 0x11, /* data output from TX FIFO, for debugging purposes */
  SI_GPIO_PIN_CFG_GPIO_MODE_IN_SLEEP		= 0x12, /* output, '0' when in sleep state */
  SI_GPIO_PIN_CFG_GPIO_MODE_TX_STATE		= 0x13, /* output, '1' when in TX state */
  SI_GPIO_PIN_CFG_GPIO_MODE_TX_FIFO_EMPTY	= 0x14, /* output, '1' when FIFO is empty */
  SI_GPIO_PIN_CFG_GPIO_MODE_LOW_BATT		= 0x15, /* output, '1' if low battery is detected */
  SI_GPIO_PIN_CFG_NIRQ_MODE_DONOTHING		= 0x00,
  SI_GPIO_PIN_CFG_SDO_MODE_DONOTHING		= 0x00,
  SI_GPIO_PIN_CFG_DRV_STRENGTH_HIGH		= (0x00 << 5),
  SI_GPIO_PIN_CFG_DRV_STRENGTH_MED_HIGH		= (0x01 << 5),
  SI_GPIO_PIN_CFG_DRV_STRENGTH_MED_LOW		= (0x02 << 5),
  SI_GPIO_PIN_CFG_DRV_STRENGTH_LOW		= (0x03 << 5),
};

/**
 * Si State Change
 */
enum {
  SI_STATE_CHANGE_NOCHANGE	= (0 << 8) | SI_CMD_CHANGE_STATE,
  SI_STATE_CHANGE_SLEEP		= (1 << 8) | SI_CMD_CHANGE_STATE,
  SI_STATE_CHANGE_SPI_ACTIVE	= (2 << 8) | SI_CMD_CHANGE_STATE,
  SI_STATE_CHANGE_READY		= (3 << 8) | SI_CMD_CHANGE_STATE,
  SI_STATE_CHANGE_TX_TUNE	= (5 << 8) | SI_CMD_CHANGE_STATE,
  SI_STATE_CHANGE_RX_TUNE	= (6 << 8) | SI_CMD_CHANGE_STATE,
  SI_STATE_CHANGE_TX		= (7 << 8) | SI_CMD_CHANGE_STATE,
  SI_STATE_CHANGE_RX		= (8 << 8) | SI_CMD_CHANGE_STATE,
};

/**
 * Si Get ADC reading
 */
enum {
  SI_GET_ADC_READING_TEMPERATURE	= (1 << 4),
  SI_GET_ADC_READING_BATTERY		= (1 << 3),
  SI_GET_ADC_READING_GPIO		= (1 << 2),
  SI_GET_ADC_READING_GPIO_PIN_GPIO3	= 3,
  SI_GET_ADC_READING_GPIO_PIN_GPIO2	= 2,
  SI_GET_ADC_READING_GPIO_PIN_GPIO1	= 1,
  SI_GET_ADC_READING_GPIO_PIN_GPIO0	= 0,
  SI_GET_ADC_READING_RANGE_0V8		= 0,
  SI_GET_ADC_READING_RANGE_1V6		= 4,
  SI_GET_ADC_READING_RANGE_3V2		= 5,
  SI_GET_ADC_READING_RANGE_2V4		= 8,
  SI_GET_ADC_READING_RANGE_3V6		= 9
};

/**
 * =============================================================================
 *                         PROPERTY DEFINITIONS
 * =============================================================================
 */

/**
 * Si Property Groups
 */
enum {
  SI_PROPERTY_GROUP_GLOBAL		= 0x00,
  SI_PROPERTY_GROUP_INT_CTL		= 0x01,
  SI_PROPERTY_GROUP_FRR_CTL		= 0x02,
  SI_PROPERTY_GROUP_PREAMBLE		= 0x10,
  SI_PROPERTY_GROUP_SYNC		= 0x11,
  SI_PROPERTY_GROUP_PKT			= 0x12,
  SI_PROPERTY_GROUP_MODEM		= 0x20,
  SI_PROPERTY_GROUP_PA			= 0x22,
  SI_PROPERTY_GROUP_SYNTH		= 0x23,
  SI_PROPERTY_GROUP_FREQ_CONTROL	= 0x40,
};
/**
 * Si Interrupt Control Properties
 */
enum {
  SI_INT_CTL_ENABLE			= 0x00,
  SI_INT_CTL_PH_ENABLE			= 0x01,
  SI_INT_CTL_CHIP_ENABLE		= 0x02,
};
/**
 * Si Global Properties
 */
enum {
  SI_GLOBAL_XO_TUNE			= 0x00,
  SI_GLOBAL_CONFIG			= 0x03,
};
/**
 * Si Preamble Properties
 */
enum {
  SI_PREAMBLE_TX_LENGTH			= 0x00,
};
/**
 * Si Sync Properties
 */
enum {
  SI_SYNC_CONFIG			= 0x11,
};
/**
 * Si Modem Properties
 */
enum {
  SI_MODEM_MOD_TYPE			= 0x00,
  SI_MODEM_MOD_TYPE_CW			= 0x00,
  SI_MODEM_MOD_TYPE_OOK			= 0x01,
  SI_MODEM_MOD_TYPE_2FSK		= 0x02, /* default */
  SI_MODEM_MOD_TYPE_2GFSK		= 0x03,
  SI_MODEM_MOD_TYPE_4FSK		= 0x04,
  SI_MODEM_MOD_TYPE_4GFSK		= 0x05,
  SI_MODEM_MOD_SOURCE_PACKET		= (0x00 << 3), /* default */
  SI_MODEM_MOD_SOURCE_DIRECT		= (0x01 << 3),
  SI_MODEM_MOD_SOURCE_PSEUDO		= (0x02 << 3),
  SI_MODEM_MOD_GPIO_0			= (0x00 << 5), /* default */
  SI_MODEM_MOD_GPIO_1			= (0x01 << 5),
  SI_MODEM_MOD_GPIO_2			= (0x02 << 5),
  SI_MODEM_MOD_GPIO_3			= (0x03 << 5),
  SI_MODEM_MOD_DIRECT_MODE_SYNC		= (0x00 << 7), /* default */
  SI_MODEM_MOD_DIRECT_MODE_ASYNC	= (0x01 << 7),
  SI_MODEM_MAP_CONTROL			= 0x01,
  SI_MODEM_DSM_CTRL			= 0x02,
  SI_MODEM_DSM_CTRL_NOFORCE_DSM_LSB	= (0x00 << 2),
  SI_MODEM_DSM_CTRL_FORCE_DSM_LSB	= (0x01 << 2), /* default */
  SI_MODEM_DSM_CTRL_MASH_1_1_1		= (0x03 << 0), /* default */
  SI_MODEM_DATA_RATE			= 0x03,
  SI_MODEM_TX_NCO_MODE			= 0x06,
  SI_MODEM_TX_NCO_TXOSR_10X		= (0x00),
  SI_MODEM_TX_NCO_TXOSR_40X		= (0x4000000),
  SI_MODEM_TX_NCO_TXOSR_20X		= (0x8000000),
  SI_MODEM_FREQ_DEV			= 0x0a,
  SI_MODEM_FREQ_OFFSET			= 0x0d,
  SI_MODEM_TX_FILTER_COEFF8		= 0x0f,
  SI_MODEM_CLKGEN_BAND			= 0x51,
  SI_MODEM_CLKGEN_SY_SEL_0		= (0x00 << 3), /* low power */
  SI_MODEM_CLKGEN_SY_SEL_1		= (0x01 << 3), /* default */
  SI_MODEM_CLKGEN_FVCO_DIV_4		= 0x00, /* default */
  SI_MODEM_CLKGEN_FVCO_DIV_6		= 0x01,
  SI_MODEM_CLKGEN_FVCO_DIV_8		= 0x02, /* for 70cm ISM band */
  SI_MODEM_CLKGEN_FVCO_DIV_12		= 0x03,
  SI_MODEM_CLKGEN_FVCO_DIV_16		= 0x04,
  SI_MODEM_CLKGEN_FVCO_DIV_24		= 0x05,
  SI_MODEM_CLKGEN_FVCO_DIV_24_2		= 0x06,
  SI_MODEM_CLKGEN_FVCO_DIV_24_3		= 0x07,
};
/**
 * Si PA Properties
 */
enum {
  SI_PA_MODE				= 0x00,
  SI_PA_PWR_LVL				= 0x01,
  SI_PA_BIAS_CLKDUTY			= 0x02,
  SI_PA_BIAS_CLKDUTY_SIN_25		= (0x03 << 6), /* for si4060 */
  SI_PA_BIAS_CLKDUTY_DIFF_50		= (0x00 << 6), /* for si4063 */
};
/**
 * Si Synthesizer Properties
 */
enum {
  SI_SYNTH_PFDCP_CPFF			= 0x00,
  SI_SYNTH_PFDCP_CPINT			= 0x01,
  SI_SYNTH_VCO_KV			= 0x02,
  SI_SYNTH_LPFILT3			= 0x03,
  SI_SYNTH_LPFILT2			= 0x04,
  SI_SYNTH_LPFILT1			= 0x05,
  SI_SYNTH_LPFILT0			= 0x06,
  SI_SYNTH_VCO_KVCAL			= 0x07,
};
/**
 * Si Frequency Control Properties
 */
enum {
  SI_FREQ_CONTROL_INTE			= 0x00,
  SI_FREQ_CONTROL_FRAC			= 0x01,
  SI_FREQ_CONTROL_CHANNEL_STEP_SIZE	= 0x04,
  SI_FREQ_CONTROL_W_SIZE		= 0x06,
};

typedef struct {
	uint32_t speed;
} gfsk_conf_t;

#define RADIO_MIN_FREQ				140000000
#define RADIO_MAX_FREQ				448000000
#define inRadioBand(freq) 	(RADIO_MIN_FREQ <= (freq) && (freq) <= RADIO_MAX_FREQ)

/* Private function prototypes -----------------------------------------------*/
void Delay(uint32_t);
void USART_Initialization(void);
void UART1_Init_Fast();
void UART1_DeInit_Fast();
void UART1_ENABLE();
void USART_SendString(char *);
void USART_SendChar(char);

void SPI_Initialization(void);
void SPI_Init_Fast();
void SPI_wait(void);
void SPI_sendByte(uint8_t);
uint8_t SPI_ReadByte(void);
void SPI_ClearRXBuffer();
uint8_t SPI_RWByte(uint8_t value);

void GPIO_Initialization(void);
void GPIO_Init_Fast();

void _si_trx_cs_enable();
void _si_trx_cs_disable();
uint8_t spi_bitbang_transfer(uint8_t);
void _si_trx_transfer_nocts(int tx_count, int rx_count, uint8_t *data);
void _si_trx_transfer(int tx_count, int rx_count, uint8_t *data);

void Si4464_write(uint8_t *data, int tx_count);
void Si4464_read(uint8_t *data, int tx_count);

void Si4464_Init(void);
void initAFSK();

uint16_t si_trx_get_part_info(void);
void setFrequency(uint32_t freq, uint16_t shift);
void setShift(uint16_t shift);
void setModemAFSK(void);
void setModemOOK(void);
void setModem2FSK(void);
void setModem2GFSK(uint32_t speed);
void setPowerLevel(int8_t level);
void startTx(uint16_t size);
void stopTx(void);
uint8_t dBm2powerLvl(int32_t dBm);
void radioShutdown(void);
void radioTurnOn(void);
uint8_t radioTune(uint32_t frequency, uint16_t shift, int8_t level, uint16_t size);
int8_t Si4464_getTemperature(void);
void Si4463_CS(uint8_t);

void ax25_init(ax25_t *packet);
void ax25_add_header(ax25_t *packet, uint8_t *callsign, uint8_t ssid, uint8_t *path, uint16_t preamble);
void ax25_add_footer(ax25_t *packet);
uint16_t ax25_CRC(ax25_t *packet);
void ax25_add_sync(ax25_t *packet);
void ax25_add_flag(ax25_t *packet);
void ax25_add_string(ax25_t *packet, uint8_t *string);
void ax25_add_path(ax25_t *packet, uint8_t *callsign, uint8_t ssid, uint8_t last);
void ax25_add_byte(ax25_t *packet, uint8_t byte);

uint32_t aprs_encode_message(ax25_t* buffer, uint8_t * callsign, uint8_t ssid, uint8_t * path, uint16_t preamble, uint8_t *text);



void Timer1_Init();
void Timer1_1200Hz();
void Timer1_2200Hz();
void Timer2_Init();
void Timer2_ISR();
char tx_buffer_dequeue();
void tx_buffer_queue(char);
char tx_buffer_empty(void);

uint8_t dBm2powerLvl(int32_t dBm);


#endif /* __MAIN_H */