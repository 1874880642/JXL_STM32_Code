/************************************************
 ����:STM32F103ZET6����GC9A01��ʾͼ��
 
 ����˵��
	GC9A01		stm32F10x
	
	3V3				VIN
	GND				GND
	SCL				PA5
	SDA				PA4
	RES				PA6
	DC				PA7
	CS				PB6
	BLK				PB7
 
 ���ߣ�JXL
************************************************/

#include "pbdata.h"
#include "lcd_init.h"
#include "lcd.h"

#include "image.h"

int main(void)
{
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);   //���Ϊ��ɫ����ɫ
	while(1)
	{
		LCD_ShowPicture(0,0,LCD_W,LCD_H,YXDZ_logo);   //��ʾһ��ͼƬ
	}
}

