#include "echo.h"

sbit trig=P1^0;
sbit echo=P1^1;

void Delay14us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 42;
	while (--i);
}


void send_wave()
{
	unsigned char i=0;
	
	for(i=0;i<8;i++)
	{
		trig=1;
		Delay14us();
		trig=0;
		Delay14us();
	}
}

unsigned int Get_Csb()
{
	
	unsigned int dis;
	CMOD&=0x00;   //1mhz
//	CH=0x00;      //计数高八位定时12T10001us
//	CL=0x00;      //计数低八位
	CH=0x8D;      //计数高八位定时12T10001us
	CL=0x1C;      //计数低八位
	Send_Wave();  //发送40Khz波
	CR=1;     //开始计时
	while((echo==1)&&(CF==0));//等待接收端接收到返回信号或者溢出
	CR=0; //关闭计时
	if(CF==0) //如果未超出测量范围，进行数据处理
	{
		//dis=((CH << 8 | CL)-0x8D1C) * 0.017;//cm
		dis=(unsigned int)((CH << 8 | CL)-0x8d1c) * 0.017;//cm
	}
	else  //如果超出测量范围，溢出标志位会硬件置一，我们需要在此软件清零
	{
	    CF=0;  //溢出标志位清零
		dis=0;
	}
	return  dis;
}

unsigned int read_echo()
{
	unsigned int dis=0;
	
	CMOD&=0x00;
	CH=0x00;
	CL=0x00;
	send_wave();
	CR=1;
	while(echo==1&&CF==0);
	CR=0;
	
	if(CF==0)
	{
		dis=((unsigned int)(CH<<8|CL))*0.017;
	}
	else
	{
		CF=0;
		dis=0;
		
	}
	
	return dis;
}