#include "uart.h"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xC7;		//���ö�ʱ��ʼֵ
	T2H = 0xFE;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	ES=1;
	EA=1;
}

bit busy=0;
unsigned char xdata uart_rx_buf[rx_cnt]={0};
unsigned char xdata uart_rx_cnt=0;
unsigned char xdata uart_rx_flag = 0; //Ϊ1����Ϊ���յ���һ��ָ��
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
		if (++uart_rx_cnt >= rx_cnt)	//����һ�½��մ�С
		{
			uart_rx_cnt = uart_rx_flag = uart_rx_buf[0] = 0;
		}
	}
}
