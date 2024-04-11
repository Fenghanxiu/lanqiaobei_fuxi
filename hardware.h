#ifndef __HARDWARE_H__
#define __HARDWARE_H__
#include <STC15F2K60S2.H>
#include "intrins.h"
/*led*/
extern unsigned char led_buf;
void led_output();
/*digs*/
extern unsigned char xdata dig_buf[][8];
extern unsigned char xdata *digs;
extern unsigned char xdata dig_switch;

extern unsigned char xdata key_state;
extern unsigned char xdata key_value;
void key_pad_scan();
/*time*/
void Timer1Init(void);		//1∫¡√Î@12.000M
extern unsigned char xdata timer_10ms;
extern unsigned char xdata timer_50ms;
extern unsigned int xdata timer_100ms;
/*555*/
void freqInit(void);
extern unsigned int xdata freq;
#endif