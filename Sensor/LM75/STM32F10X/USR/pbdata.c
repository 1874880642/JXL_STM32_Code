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
	nCount=2*nCount;
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



