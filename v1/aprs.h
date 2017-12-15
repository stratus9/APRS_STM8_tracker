#ifndef __APRS_H
#define __APRS_H

/*
SSID
	-0 Your primary station usually fixed and message capable
	-1 generic additional station, digi, mobile, wx, etc
	-2 generic additional station, digi, mobile, wx, etc
	-3 generic additional station, digi, mobile, wx, etc
	-4 generic additional station, digi, mobile, wx, etc
	-5 Other networks (Dstar, Iphones, Androids, Blackberry's etc)
	-6 Special activity, Satellite ops, camping or 6 meters, etc
	-7 walkie talkies, HT's or other human portable
	-8 boats, sailboats, RV's or second main mobile
	-9 Primary Mobile (usually message capable)
	-10 internet, Igates, echolink, winlink, AVRS, APRN, etc
	-11 balloons, aircraft, spacecraft, etc
	-12 APRStt, DTMF, RFID, devices, one-way trackers*, etc
	-13 Weather stations
	-14 Truckers or generally full time drivers
	-15 generic additional station, digi, mobile, wx, etc
*/


uint16_t crc_ccitt_update(uint16_t crc, uint8_t data);

#define APRS_COMMENT "AGH SPACE"
#define APRS_FLAGS 11
#define APRS_CALLSIGN "BARTEK"							// 6 characters (fill the rest with 'space')
#define SSID 11											// APRS source address SSID
#define DSSID 1											// APRS destination address SSID
#define PATH "WIDE2-"

#include "struct.h"
void calcDMH(long x, int8_t* degrees, uint8_t* minutes, uint8_t* h_minutes);
void APRS_PreparePoosition(APRS_t * APRS, gps_t * gps, sensors_t * sensors);
void APRS_PreparePacket(APRS_t * APRS);
void APRS_SendPacket(APRS_t * APRS);

#endif