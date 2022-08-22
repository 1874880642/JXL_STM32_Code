#include "pbdata.h"

u8 dt=0;

void delay(u16 nCount)
{
	while(nCount--)
	{
		u32 temp;
		SysTick->LOAD=9000;
		SysTick->VAL=0x00;
		SysTick->CTRL=0x01;
		do
		{
			temp=SysTick->CTRL;
		}while((temp&&0x01)&&(!(temp&(1<<16))));
		SysTick->CTRL=0x00;
		SysTick->VAL=0x00;
	}
}

void delay_ms(u16 nCount)
{
		u32 temp;
		SysTick->LOAD=9000*nCount;
		SysTick->VAL=0x00;
		SysTick->CTRL=0x01;
		do
		{
			temp=SysTick->CTRL;
		}while((temp&&0x01)&&(!(temp&(1<<16))));
		SysTick->CTRL=0x00;
		SysTick->VAL=0x00;
}

void delay_us(u32 nCount)//us����ʱ
{
	while(nCount--)
	{
	u32 temp;
	SysTick->LOAD=9;//���س�ֵ����Ϊ1ms��ʱ��
	SysTick->VAL=0x00;//��յ�ǰֵ�Ĵ�������ռ�����
	SysTick->CTRL=0x01;//ʹ�ܣ�����0ʱ�޶����������ⲿʱ��Դ
	do
	{
		temp=SysTick->CTRL;//��ȡ��ǰ����ʱ��������ֵ
	}while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽����ȴ���־λ��0
	SysTick->CTRL=0x00;//�رռ�����
	SysTick->VAL=0x00;//��ռ�����
	}
}


/* ����GPIOΪΪ����ģʽ*/
void Gpio_Config_FLOATING_In()
{
    /*��GPIO��ʼ��*/
 	GPIO_InitTypeDef   GPIO_Init_Structure;

	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_0;

	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;

	/*����IO��Ϊ��������ģʽ*/ 
 	GPIO_Init_Structure.GPIO_Mode= GPIO_Mode_IN_FLOATING;

	GPIO_Init( GPIOA,  &GPIO_Init_Structure);
}

/* ����GPIOΪΪ���ģʽ*/
void Gpio_Config_Mode_Out()
{
    /*��GPIO��ʼ��*/
 	GPIO_InitTypeDef   GPIO_Init_Structure;

	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_0;

	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;

	/*����IO��Ϊ��©���ģʽ*/ 
 	GPIO_Init_Structure.GPIO_Mode= GPIO_Mode_Out_OD;

	GPIO_Init( GPIOA,  &GPIO_Init_Structure);
}


int init18B20(void)
{
	int temp;
	DS18B20_DQ_OUT_High;
	delay_us(10);
	DS18B20_DQ_OUT_Low;
	delay_us(700);
	DS18B20_DQ_OUT_High;
	delay_us(45);
	temp=DS18B20_DQ_IN;
	delay_us(120);
	return (temp);
}



unsigned char read18B20()
{
	unsigned char temp1,n;
	for(n=0;n<8;n++)
	{
		DS18B20_DQ_OUT_High;
		delay_us(10);
		DS18B20_DQ_OUT_Low;
		delay_us(1);
		DS18B20_DQ_OUT_High;
		delay_us(10);
		temp1>>=1;
		if(DS18B20_DQ_IN==1)
		{
			temp1|=0x80;
		}
		else
		{
			temp1|=0x00;
		}
		delay_us(10);
	}
	return(temp1);
}


void write18B20(unsigned char temp2)
{
	unsigned char n;
	for(n=0;n<8;n++)
	{
		DS18B20_DQ_OUT_High;
		delay_us(1);
		DS18B20_DQ_OUT_Low;
		if(temp2&0x01)
		{
			DS18B20_DQ_OUT_High;
		}
		else
		{
			DS18B20_DQ_OUT_Low;
		}
		
		delay_us(60);
		DS18B20_DQ_OUT_High;
		delay_us(1);
		temp2>>=1;
	}
	delay_us(10);
}



void readwen()
{
	init18B20();
	write18B20(0xcc);
	write18B20(0x44);
	delay_ms(200);
	init18B20();
	write18B20(0xcc);
	write18B20(0xbe);
}

