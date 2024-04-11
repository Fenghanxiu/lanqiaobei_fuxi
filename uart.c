#include "uart.h"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xC7;		//设置定时初始值
	T2H = 0xFE;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	ES=1;
	EA=1;
}

bit busy=0;
unsigned char xdata uart_rx_buf[rx_cnt]={0};
unsigned char xdata uart_rx_cnt=0;
unsigned char xdata uart_rx_flag = 0; //为1定义为接收到了一个指令
void send_data(unsigned char dat)
{
	while(busy);
	SBUF=dat;
	busy=1;
}

void send_string(unsigned char *dat)
{
	while(*dat)
	{
		send_data(*dat);
		dat++;
	}
}

void Uart(void) interrupt 4
{
	if(TI)
	{
		TI=0;
		busy=0;
	}
	
	if(RI)
	{
		RI=0;
		uart_rx_buf[uart_rx_cnt]=SBUF;
		if(uart_rx_buf[uart_rx_cnt-1]=='\r'&&uart_rx_buf[uart_rx_cnt]=='\n')
		{
			uart_rx_flag=1;
			uart_rx_cnt++;
			uart_rx_buf[uart_rx_cnt]=0;
			uart_rx_cnt=0;
		}
		if (++uart_rx_cnt >= rx_cnt)	//限制一下接收大小
		{
			uart_rx_cnt = uart_rx_flag = uart_rx_buf[0] = 0;
		}
	}
}
