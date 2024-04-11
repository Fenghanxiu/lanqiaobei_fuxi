#ifndef __IIC_H__
#define __IIC_H__
#include <STC15F2K60S2.H>
#include "intrins.h"
void write_eeprom(unsigned char addr,unsigned char *dat,unsigned char len);
void read_eeprom(unsigned char addr, unsigned char *dat, unsigned char len);
unsigned char read_adc(unsigned char channal);
void write_dac(unsigned char level);
void stop_dac();
unsigned char read_adc(unsigned char channal);
void write_dac(unsigned char level);
void stop_dac();

#endif