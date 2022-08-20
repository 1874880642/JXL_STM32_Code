#ifndef _stm32f10x_oled_H
#define _stm32f10x_oled_H

#include "pbdata.h"



void RCC_Configuration(void);//��ʼ��дOLEDҪʹ�ö˿ڵ�ʱ��

void IIC_Start(void);//IIC��ʼ�κ���

void IIC_Stop(void);//IICֹͣ�κ���

void IIC_Wait_Ack(void);//�ȴ�ȷ��

void Write_IIC_Byte(unsigned char IIC_Byte);//��λд��һ���ֽ�

void Write_IIC_Command(unsigned char IIC_Command);//дָ��

void Write_IIC_Data(unsigned char IIC_Data);//д����

void OLED_WR_Byte(unsigned dat,unsigned cmd);//ָ��д�����ָ�������

void fill_picture(unsigned char fill_Data);//����ͼƬ

void OLED_Set_Pos(unsigned char x, unsigned char y);//��������

void OLED_Display_On(void);//����OLED��ʾ

void OLED_Display_Off(void);//�ر�OLED��ʾ

void OLED_Clear(void);//��������

void OLED_On(void);

/*
		��ָ��λ����ʾһ���ַ�,���������ַ�
		x:0~127
		y:0~63
		mode:0,������ʾ;1,������ʾ				 
		size:ѡ������ 16/12 
*/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);

u32 oled_pow(u8 m,u8 n);//m^n����

/*
		��ʾ2������
		x,y :�������	 
		len :���ֵ�λ��
		size:�����С
		mode:ģʽ	0,���ģʽ;1,����ģʽ
		num:��ֵ(0~4294967295)
*/
void OLED_ShowNum(u8 x,u8 y,signed int num,u8 len,u8 size2);

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);//��ʾһ���ַ���

void OLED_ShowCHinese(u8 x,u8 y,u8 no);//��ʾ����

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void OLED_Init(void);//��ʼ��ssd1306


#endif
