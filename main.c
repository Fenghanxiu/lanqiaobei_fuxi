#include <STC15F2K60S2.H>
#include "hardware.h"
#include "ds1302.h"
#include "iic.h"
#include "uart.h"
#include "echo.h"
#include "onewire.h"

#include <stdio.h>
#include <string.h>

void main()
{
	unsigned char xdata screen=0;
	unsigned char xdata work_flag=0;
	
	unsigned int xdata data_buf=0;
	unsigned int xdata data_buf_bak=65530;
	unsigned int xdata data_buf_bak1=65535;
	unsigned int xdata data_len=0;
	unsigned char xdata data_low=0;
	unsigned char xdata data_high=0;
	
	unsigned char xdata write_flag=0;
	unsigned char xdata write_buf[4]={0};
	unsigned char xdata clear_flag=0;
	
	
	unsigned int xdata distance=0;
	unsigned int xdata tempratraure=0;
	unsigned char xdata adc_read[4]={100,0,0,100};
	unsigned int xdata adc_conver[4]={0};
	
	char xdata uart_tx_buf[50] = {0};
	write_ds1302();
	
	Timer1Init();
	UartInit();
	freqInit();
	
	while(1)
	{
		if(timer_10ms>=10)
		{
			timer_10ms=0;
			/*input*/
			key_pad_scan();
			/*process*/
			/*high*/
			if(timer_100ms>1000)
			{
				timer_100ms=0;
//				sprintf(uart_tx_buf, "#%ucm %.2fC %uHZ\r\n", (unsigned int)distance , (float)tempratraure / 100.0f, (unsigned int)freq);
//				send_string(uart_tx_buf);
//				sprintf(uart_tx_buf, "#%.2fV %.2fV %.2fV\r\n", (float)adc_conver[0] / 100.0f, (float)adc_conver[1] / 100.0f,(float)adc_conver[3] / 100.0f);
//				send_string(uart_tx_buf);
				
				
			}
			
			if(timer_50ms>=50)
			{
				timer_50ms=0;
				
				if(work_flag==0)
				{
					read_ds1302();
				}
				
				if(work_flag==1)
				{
					if(write_flag==1)
					{
						write_flag=0;
						write_eeprom(0x00,write_buf,4);
					}
				}
				if(work_flag==5)
				{
					distance=read_echo();
				}
				
				if(work_flag==3)
				{
					tempratraure=read_temp();
				}
				if(work_flag==4)
				{
					adc_read[1]=read_adc(0x01);
					adc_conver[1]=(float)adc_read[1]*1.96;
				}
				if(work_flag==2)
				{
					adc_read[3]=read_adc(0x03);
					adc_conver[3]=(float)adc_read[3]*1.96;
				}
				if(work_flag==6)
				{
					write_dac(adc_read[0]);
					adc_conver[0]=(float)adc_read[0]*1.96;
				}
				if(work_flag==7)
				{
						if(uart_rx_flag==1)
					{
						uart_rx_flag=0;
						uart_rx_cnt=0;
						
						if(strcmp(uart_rx_buf,"#ST\r\n")==0)
						{
							sprintf(uart_tx_buf, "114514\r\n");
							send_string(uart_tx_buf);
							uart_tx_buf[0]=0;
						}
				}
					
				}
				if(++work_flag>7)work_flag=0;
			}
			/*key*/
			if(key_state==255)
			{
				if(key_value==4)
				{
					if(++screen>7)screen=0;
					
					if(screen==2)
					{
						
						write_buf[0]=time_10[0];
						write_buf[1]=time_10[1];
						write_buf[2]=(unsigned char )(data_buf>>8);
						write_buf[3]=(unsigned char )data_buf;
						
						data_buf_bak1=data_buf_bak;
						data_buf_bak=data_buf;
						data_len=0;
						data_buf=0;
						
						write_flag=1;
					}
				}
				if(screen==1)
				{
				if(key_value==5)
				{
					data_len=0;
					data_buf=0;
				}
				if(key_value==6)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+0;
						data_len++;
					}
				}
				
				if(key_value==10)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+1;
						data_len++;
					}
				}
				if(key_value==14)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+2;
						data_len++;
					}
				}
				if(key_value==18)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+3;
						data_len++;
					}
				}
				if(key_value==9)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+4;
						data_len++;
					}
				}
				if(key_value==13)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+5;
						data_len++;
					}
				}
				if(key_value==17)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+6;
						data_len++;
					}
				}
				if(key_value==8)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+7;
						data_len++;
					}
				}
				if(key_value==12)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+8;
						data_len++;
					}
				}
				if(key_value==16)
				{
					if(data_len<4)
					{
						data_buf=data_buf*10+9;
						data_len++;
					}
				}
				}
				
				
			}
			
			/*output*/
			/*dig*/
			digs=dig_buf[screen];
			
			dig_buf[0][0]=time_10[0]/10;
			dig_buf[0][1]=time_10[0]%10;
			
			dig_buf[0][3]=time_10[1]/10;
			dig_buf[0][4]=time_10[1]%10;
			
			dig_buf[0][6]=time_10[2]/10;
			dig_buf[0][7]=time_10[2]%10;
			
			dig_buf[1][4]=data_buf>=1000?data_buf/1000%10:16;
			dig_buf[1][5]=data_buf>=100?data_buf/100%10:16;
			dig_buf[1][6]=data_buf>=10?data_buf/10%10:16;
			dig_buf[1][7]=data_buf>0?data_buf%10:16;
			
			dig_buf[2][3]=write_buf[0]/10;
			dig_buf[2][4]=write_buf[0]%10;
			
			dig_buf[2][6]=write_buf[1]/10;
			dig_buf[2][7]=write_buf[1]%10;
			
			dig_buf[3][4]=distance>=1000?distance/1000%10:16;
			dig_buf[3][5]=distance>=100?distance/100%10:16;
			dig_buf[3][6]=distance>=10?distance/10%10:16;
			dig_buf[3][7]=distance%10;
			
			dig_buf[4][4]=tempratraure>=1000?tempratraure/1000%10:16;
			dig_buf[4][5]=tempratraure>=100?tempratraure/100%10+32:16;
			dig_buf[4][6]=tempratraure>=10?tempratraure/10%10:16;
			dig_buf[4][7]=tempratraure%10;
			
			dig_buf[5][3]=freq>=10000?freq/10000%10:16;
			dig_buf[5][4]=freq>=1000?freq/1000%10:16;
			dig_buf[5][5]=freq>=100?freq/100%10:16;
			dig_buf[5][6]=freq>=10?freq/10%10:16;
			dig_buf[5][7]=freq%10;
			
			
			dig_buf[6][1]=adc_conver[1]/100%10+32;
			dig_buf[6][2]=adc_conver[1]/10%10;
			dig_buf[6][3]=adc_conver[1]%10;
			
			dig_buf[6][5]=adc_conver[3]/100%10+32;
			dig_buf[6][6]=adc_conver[3]/10%10;
			dig_buf[6][7]=adc_conver[3]%10;
			
			dig_buf[7][5]=adc_conver[0]/100%10+32;
			dig_buf[7][6]=adc_conver[0]/10%10;
			dig_buf[7][7]=adc_conver[0]%10;
			/*led*/
			
			if(screen==0)
				led_buf|=0x01;
			else
				led_buf&=~0x01;
			
			if(screen==1)
				led_buf|=0x02;
			else
				led_buf&=~0x02;
			
			if(screen==2)
				led_buf|=0x04;
			else
				led_buf&=~0x04;
			
			if(data_buf_bak>data_buf_bak1)
				led_buf|=0x08;
			else
				led_buf&=~0x08;
		}
		
		
	}
}