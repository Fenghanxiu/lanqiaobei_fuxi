#include "hardware.h"

/*led*/
unsigned char led_buf=0x00;
unsigned char xdata led_duty=10;
unsigned char xdata led_cnt=0;
unsigned char xdata led_period=10;
void led_output()
{
	P0=~led_buf;
	P2|=0x80;
	P2&=0x1f;
}
/*power*/
unsigned char power_buf=0x00;
void power_output()
{
	P0=power_buf;
	P2|=0xa0;
	P2&=0x1f;
}
/*digs*/
/*************  ���س�������    **************/
unsigned char code t_display[]={                       //��׼�ֿ�
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
//	 0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   -1
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //λ��

unsigned char xdata dig_buf[][8] = 
{
	{ 16, 16, 17, 16, 16, 17, 16, 16, }, //ʱ�����
	{ 12, 16, 16, 16, 16, 16, 16, 16, }, //�������
	{ 14, 16, 16, 16, 16, 17, 16, 16, }, //��¼����
	{ 13, 16, 16, 16, 16, 16, 16, 16, }, //����
	{ 26, 16, 16, 16, 16, 16, 16, 16, }, //�¶�
	{ 15, 16, 16, 16, 16, 16, 16, 16, }, //Ƶ��
	{ 16, 16, 16, 16, 16, 16, 16, 16, }, //ADC
	{ 16, 16, 16, 16, 16, 16, 16, 16, }, //DAC
};
unsigned char xdata *digs = dig_buf[0]; //��ǰ��ʾ����
unsigned char xdata dig_switch = 0xFF; 

void dig_output()
{
	static unsigned char dig_com = 0;
	
	P0 = 0x00; //��Ӱ
	P2 |= 0xC0;
	P2 &= ~0xE0;
	
	P0 = ~t_display[digs[dig_com]]; //��ѡ
	P2 |= 0xE0;
	P2 &= ~0xE0;
	
	P0 = T_COM[dig_com]; //λѡ
	P2 |= 0xC0;
	P2 &= ~0xE0;
	
	if (++dig_com >= 8)
	{
		dig_com = 0;
	}
}

/*key*/



unsigned char code key_values[4][4]=
{
	{19,18,17,16},
	{15,14,13,12},
	{11,10,9,8},
	{7,6,5,4},
};

unsigned char xdata key_state=0;
unsigned char xdata key_value=0;

void key_scan()
{
	P3=0x0f;
	
	if(~P3&0x0f)
	{
		if(++key_state>255)key_state=0;
		
		if(!P30)key_value=7;
		if(!P31)key_value=6;
		if(!P32)key_value=5;
		if(!P33)key_value=4;
	}
	else
	{
		key_value=key_state=0;
	}
}

void key_pad_scan()
{
	unsigned char C=255;
	unsigned char R=255;
	
	P3=0x0f;
	P4=0x00;
	
	if(~P3&0x0f)
	{
		if(key_state<255)key_state++;
		if(!P30)R=0;
		if(!P31)R=1;
		if(!P32)R=2;
		if(!P33)R=3;
		
		
		P3=0xf0;
		P4=0xff;
		
		if(!P34)C=0;
		if(!P35)C=1;
		if(!P42)C=2;
		if(!P44)C=3;
		
		if(C!=255&R!=255)
		{
			key_value=key_values[C][R];
		}
		else 
		{
			key_value=key_state=0;
		}
	}
	else 
	{
		if (key_value && key_state <= 250) //����������ɿ�
		{
			if (key_state < 100) //�����µ�����ʱ��û�г���1��
				key_state = 255;
			else if (key_state < 200) //�����µ�����ʱ�䳬��1���û����2��
				key_state = 254;
			else //�����µ�����ʱ�䳬��2��
				key_state = 253;
		}
		else //������ȫ�ɿ�
		{
			key_value = 0;
			key_state = 0;
		}
	}
	
}

/*555*/
unsigned int xdata freq_cnt=0;
unsigned int xdata freq=0;
void freqInit(void)		//1΢��@12.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x04;
	TL0 = 0x00;		//���ö�ʱ��ʼֵ
	TH0 = 0x00;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}


/*time*/
void Timer1Init(void)		//1����@12.000MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x50;		//���ö�ʱ��ʼֵ
	TH1 = 0xFB;		//���ö�ʱ��ʼֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	EA=1;
	ET1=1;
}
unsigned char xdata timer_1ms=0;
unsigned char xdata timer_10ms=0;
unsigned char xdata timer_50ms=0;
unsigned int xdata timer_100ms=333;


void Timer1(void) interrupt 3
{
	
	if(led_cnt<=led_duty)
	{
		P0=~led_buf;
		P2|=0x80;
		P2&=0x1f;
	}
	else
	{
		P0=0xff;
		P2|=0x80;
		P2&=0x1f;
	}
	
	if(++led_cnt>=led_period)led_cnt=0;
		
	if(++timer_1ms>10)
	{
		timer_1ms=0;
		//led_output();
		dig_output();
		power_output();
		
		timer_10ms++;
		timer_50ms++;
		timer_100ms++;
		
		if(++freq_cnt>=1000)
		{
			freq_cnt=0;
			TR0=0;
			
			freq=(unsigned int)(TH0<<8|TL0);
			TH0=0;
			TL0=0;
			
			TR0=1;
			
		}
		
		
	}
}

