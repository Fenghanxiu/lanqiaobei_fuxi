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
//	CH=0x00;      //�����߰�λ��ʱ12T10001us
//	CL=0x00;      //�����Ͱ�λ
	CH=0x8D;      //�����߰�λ��ʱ12T10001us
	CL=0x1C;      //�����Ͱ�λ
	Send_Wave();  //����40Khz��
	CR=1;     //��ʼ��ʱ
	while((echo==1)&&(CF==0));//�ȴ����ն˽��յ������źŻ������
	CR=0; //�رռ�ʱ
	if(CF==0) //���δ����������Χ���������ݴ���
	{
		//dis=((CH << 8 | CL)-0x8D1C) * 0.017;//cm
		dis=(unsigned int)((CH << 8 | CL)-0x8d1c) * 0.017;//cm
	}
	else  //�������������Χ�������־λ��Ӳ����һ��������Ҫ�ڴ��������
	{
	    CF=0;  //�����־λ����
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