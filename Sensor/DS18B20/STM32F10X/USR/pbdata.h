#ifndef _pbdata_H
#define _pbdata_H

#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_it.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stdio.h"



#define DS18B20_DQ_OUT_Low GPIO_ResetBits(GPIOA,GPIO_Pin_0)  //���ݶ˿�	PA0 ����͵�ƽ
#define DS18B20_DQ_OUT_High GPIO_SetBits(GPIOA,GPIO_Pin_0)  //���ݶ˿�	PA0 ����ߵ�ƽ
#define DS18B20_DQ_IN GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)   //���ݶ˿�	PA0 ��ȡ
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
	
void delay(u16 nCount);
void delay_ms(u16 nCount);
void delay_us(u32 nCount);//us����ʱ

void Gpio_Config_FLOATING_In(void);
void Gpio_Config_Mode_Out(void);

int init18B20(void);
unsigned char read18B20(void);
void write18B20(unsigned char temp2);
void readwen(void);

//�������

extern u8 dt;

#endif
