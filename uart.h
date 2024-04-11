#ifndef __UART_H__
#define __UART_H__
#include <STC15F2K60S2.H>
#include "intrins.h"
void send_string(unsigned char *dat);
#define rx_cnt 24
extern unsigned char xdata uart_rx_buf[rx_cnt];
extern unsigned char xdata uart_rx_cnt;
extern unsigned char xdata uart_rx_flag ; //为1定义为接收到了一个指令
void UartInit(void);
#endif