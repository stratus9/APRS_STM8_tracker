#ifndef __STRUCT_H
#define __STRUCT_H

typedef struct {
  int32_t lon;
  int32_t lon_dmh;
  int32_t lat;
  int32_t lat_dmh;
  int32_t alti;
  uint8_t sats;
  
} gps_t;

typedef struct {
  int16_t temperature;
  uint16_t voltage;
} sensors_t;
  
typedef struct {
  uint8_t data_buffer[128];
  uint8_t aprs_buffer[128];
  uint8_t aprs_buffer_length;
  uint8_t aprs_buffer_sent;
  uint16_t crc;
  uint8_t tmp;
} APRS_t;

#endif