#include "stm32f10x_oled.h"
#include "oledfront.h"

//#define OLED_RCC  

/*IIC��ʼ�κ���*/
void IIC_Start(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);//PD4=1   SCL
	GPIO_SetBits(GPIOA,GPIO_Pin_5);//PD5=1   SDA
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);//PD5=0
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//PD4=0
}

/*IICֹͣ�κ���*/
void IIC_Stop(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);//PD4=1
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);//PD5=0
	GPIO_SetBits(GPIOA,GPIO_Pin_5);//PD5=1
}

/*�ȴ�ȷ��*/
void IIC_Wait_Ack(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);//PD6=1
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//PD6=0
}

/*��λд��һ���ֽ�*/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i,m,Cache;
	Cache=IIC_Byte;
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//PD4=0
	for(i=0;i<8;i++)
	{
		m=Cache;
		m=m&0x80;//ȡ�����ݵ����λ
		if(m==0x80)//�����Ȼ����0x80���������λΪ1������Ϊ0
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_5);//PD5=1
		}
		else
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);//PD5=0
		}
		Cache=Cache<<1;//ʹ��һλ��Ϊ���Ϊ���ж���Ϊ1����0
		GPIO_SetBits(GPIOA,GPIO_Pin_4);//д������
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
}

/*дָ��*/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00);
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*д����*/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40);
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*ָ��д�����ָ�������*/
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)//1Ϊд���ݣ�0Ϊд����
	{
   Write_IIC_Data(dat);
	}
	else 
	{
   Write_IIC_Command(dat);
	}
}

/*����ͼƬ*/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);
		OLED_WR_Byte(0x00,0);
		OLED_WR_Byte(0x10,0);
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(fill_Data,1);
		}
	}
}

/*��������*/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0+y,0);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,0);
	OLED_WR_Byte((x&0x0f),0); 
}

/*����OLED��ʾ*/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,0);
	OLED_WR_Byte(0X14,0);
	OLED_WR_Byte(0XAF,0);
}

/*�ر�OLED��ʾ*/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,0);
	OLED_WR_Byte(0X10,0);
	OLED_WR_Byte(0XAE,0);
}

/*��������*/
void OLED_Clear(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,0);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,0);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,0);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,1); 
	} //������ʾ
}


void OLED_On(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,0);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,0);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,0);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,1); 
	} //������ʾ
}


/*
		��ָ��λ����ʾһ���ַ�,���������ַ�
		x:0~127
		y:0~63
		mode:0,������ʾ;1,������ʾ				 
		size:ѡ������ 16/12 
*/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],1);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],1);
		}
		else 
		{	
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],1);
		}
}

/*m^n����*/
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/*
		��ʾ2������
		x,y :�������	 
		len :���ֵ�λ��
		size:�����С
		mode:ģʽ	0,���ģʽ;1,����ģʽ
		num:��ֵ(0~4294967295)
*/
void OLED_ShowNum(u8 x,u8 y,signed int num,u8 len,u8 size2)
{
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
			else 
			{	
				enshow=1; 
			}
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
}

/*��ʾһ���ַ���*/
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if(x>120)
		{
		x=0;
		y+=2;
		}
		j++;
	}
}

/*��ʾ����*/
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
			OLED_WR_Byte(Hzk[2*no][t],1);
			adder+=1;
    }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
		{	
			OLED_WR_Byte(Hzk[2*no+1][t],1);
			adder+=1;
    }			
}

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;
  if(y1%8==0)
	{
		y=y1/8; 
	}		
  else
	{		
		y=y1/8+1;
	}
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	  {      
			OLED_WR_Byte(BMP[j++],1);	    	
	  }
	}
}

/*��ʼ��ssd1306*/
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	 //PD3,PD6�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);	//PD3,PD6 �����

 #if OLED_MODE==1
 
 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT�������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,0xFF); //PC0~7�����

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //PG13,14,15 OUT�������
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PG13,14,15 OUT  �����

 #else
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //PC0,1 OUT�������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);						 //PC0,1 OUT  �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PG15 OUT�������	  RST
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_15);						 //PG15 OUT  �����


 #endif
delay_ms(200);

OLED_WR_Byte(0xAE,0);//--display off
	OLED_WR_Byte(0x00,0);//---set low column address
	OLED_WR_Byte(0x10,0);//---set high column address
	OLED_WR_Byte(0x40,0);//--set start line address  
	OLED_WR_Byte(0xB0,0);//--set page address
	OLED_WR_Byte(0x81,0); // contract control
	OLED_WR_Byte(0xFF,0);//--128   
	OLED_WR_Byte(0xA1,0);//set segment remap 
	OLED_WR_Byte(0xA6,0);//--normal / reverse
	OLED_WR_Byte(0xA8,0);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,0);//--1/32 duty
	OLED_WR_Byte(0xC8,0);//Com scan direction
	OLED_WR_Byte(0xD3,0);//-set display offset
	OLED_WR_Byte(0x00,0);//
	
	OLED_WR_Byte(0xD5,0);//set osc division
	OLED_WR_Byte(0x80,0);//
	
	OLED_WR_Byte(0xD8,0);//set area color mode off
	OLED_WR_Byte(0x05,0);//
	
	OLED_WR_Byte(0xD9,0);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,0);//
	
	OLED_WR_Byte(0xDA,0);//set com pin configuartion
	OLED_WR_Byte(0x12,0);//
	
	OLED_WR_Byte(0xDB,0);//set Vcomh
	OLED_WR_Byte(0x30,0);//
	
	OLED_WR_Byte(0x8D,0);//set charge pump enable
	OLED_WR_Byte(0x14,0);//
	
	OLED_WR_Byte(0xAF,0);//--turn on oled panel
}
