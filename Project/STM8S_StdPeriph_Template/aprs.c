/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "struct.h"
#include <stdlib.h>
#include "stdio.h"
#include "aprs.h"
#include "timer.h"
#include "radio.h"

#define lo8(x) ((x)&0xff)
#define hi8(x) ((x)>>8)


//Function to calculate the Frame Check Sequence (FCS) of the APRS packet. 
uint16_t crc_ccitt_update(uint16_t crc, uint8_t data){
  data ^= lo8 (crc);
  data ^= data << 4;

  return ((((uint16_t)data << 8) | hi8 (crc)) ^ (uint8_t)(data >> 4) ^ ((uint16_t)data << 3));
}


void calcDMH(long x, int8_t* degrees, uint8_t* minutes, uint8_t* h_minutes) {
    *degrees = 1;
    *minutes = 2;
    *h_minutes = 3;
}

void APRS_PreparePoosition(APRS_t * APRS, gps_t * gps, sensors_t * sensors) {
  uint8_t * buffer = (APRS->data_buffer);
  
  int8_t la_degrees, lo_degrees;
  uint8_t la_minutes, la_h_minutes, lo_minutes, lo_h_minutes;

  calcDMH(gps->lat, &la_degrees, &la_minutes, &la_h_minutes);
  calcDMH(gps->lon, &lo_degrees, &lo_minutes, &lo_h_minutes);

  static uint16_t aprs_packet_counter = 0;
  aprs_packet_counter ++;

  /*
  sprintf((char *)buffer,
          ("!%02d%02d.%02u%c/%03d%02u.%02u%cO/A=%06ld/P%dS%dT%dV%d%s"),
          abs(la_degrees), la_minutes, la_h_minutes,
          la_degrees > 0 ? 'N' : 'S',
          abs(lo_degrees), lo_minutes, lo_h_minutes,
          lo_degrees > 0 ? 'E' : 'W',
          (gps->alti) * 3280 / 1000,
          aprs_packet_counter,
          gps->sats,
          sensors->temperature,
          sensors->voltage,
          APRS_COMMENT
  );
*/
  
  uint8_t dane[40] = "!0000.00N00000.00WO/A=1234/P123";
  for(uint8_t i = 0; i < 32; i++)
                buffer[i] = dane[i];
}

void APRS_PreparePacket(APRS_t * APRS){
  uint8_t * buffer = (APRS->data_buffer);
  uint8_t * aprs_buffer = (APRS->aprs_buffer);
  
  uint8_t pos = 0;
  char APRS_destination[6] = "APRS  ";
  char APRS_callsign[6] = APRS_CALLSIGN;
  char APRS_path[6] = PATH;
  
  // Flags
        for(uint32_t i = 0 ; i < APRS_FLAGS; i++)
                aprs_buffer[pos++] = 0x7E;

  // Destination Address
        for(uint8_t i = 0; i < 6; i++)
                aprs_buffer[pos++] = APRS_destination[i];
  
  // SSID of the destination
	aprs_buffer[pos++] = 0x70 | DSSID;							// 0b0CRRSSID (C - command/response bit '1', RR - reserved '11', SSID - 0-15)
  
  // Source Address
	for(uint8_t i = 0; i < 6; i++)
		aprs_buffer[pos++] = APRS_callsign[i];
        
  // SSID to specify an APRS symbol (11 - balloon)
	aprs_buffer[pos++] = 0x30 | (SSID & 0x0F);							// 0b0CRRSSID (C - command/response bit '1', RR - reserved '11', SSID - 0-15)
        
  //Path
        for(uint8_t i = 0; i < 6; i++)
		aprs_buffer[pos++] = APRS_path[i];
        aprs_buffer[pos++] = 0x31;
  
  // Left Shifting the Address Bytes
	for(uint16_t i = APRS_FLAGS; i < pos; i++){
		aprs_buffer[i] <<= 1;
		if(i == (pos - 1)) aprs_buffer[i] |= 0x01;					// the last byte's LSB set to '1' to indicate the end of the address fields
	}      
  
  // Control Field
	aprs_buffer[pos++] = 0x03;
	
	
  // Protocol ID
	aprs_buffer[pos++] = 0xF0;
  
  //Data to send
        while(*buffer) 
              aprs_buffer[pos++] = *buffer++;
  
  // Frame Check Sequence - CRC-16-CCITT (0xFFFF)
	uint16_t crc = 0xFFFF;
	for(uint16_t i = 0; i < (pos - APRS_FLAGS); i++) crc = crc_ccitt_update(crc, aprs_buffer[APRS_FLAGS+i]);
	crc = ~crc;
	aprs_buffer[pos++] = crc & 0xFF;					        	// FCS is sent low-byte first
	aprs_buffer[pos++] = (crc >> 8) & 0xFF;							// and with the bits flipped
        
  // the end Flags
	aprs_buffer[pos++] = 0x7E;
	aprs_buffer[pos++] = 0x7E;
	aprs_buffer[pos++] = 0x7E;
        
        APRS->aprs_buffer_length = pos;
        APRS->aprs_buffer_sent = 0;
}


void APRS_SendPacket(APRS_t * APRS){
  Si4463_TX(); /* enter transmit mode */
  Delay(1000);
  
  /* wait until everything has been sent */
  while (APRS->aprs_buffer_sent < APRS->aprs_buffer_length) Delay(100);
  Si4463_RX();
}