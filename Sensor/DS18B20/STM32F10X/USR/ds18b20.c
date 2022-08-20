#include<ds18b20.h>


/*Ds18b20��λ*/
void Init_Ds18b20()
{	 
      u8 i=0;
	 /*����IO��Ϊ���ģʽ*/
     Gpio_Config_Mode_Out();

	 /*��������*/ 
     DS18B20_DQ_OUT_Low;

	 /*�ӳ�480us*/
	 Delay_us(480);
	 /*�ͷ�����*/
	 DS18B20_DQ_OUT_High;

	 /*����IO��Ϊ����ģʽ*/
	 Gpio_Config_FLOATING_In();

	 /*�ȴ���λ�ź� ע�ⲻ�����޵ȴ�*/
	 while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)&&(i<200))
	 i++;
}

/*дһ���ֽ�*/
void Write_one_byte(u8 a)
{
     u8 i=0;
	 u8 b=0x01;
	 u8 c=0;
	/*����IO��Ϊ���ģʽ*/
    Gpio_Config_Mode_Out();
	
	for(i=0;i<8;i++)
	{
	     c=b&a;
		 if(c)
		 {
		       /*��������*/
		     DS18B20_DQ_OUT_Low;
			 /*�ӳ�15us����дʱ϶*/
			 Delay_us(15);
			 /*�ͷ�����*/
			 DS18B20_DQ_OUT_High;
			 /*����40us*/
			 Delay_us(40);
		 }   
		 else
		 {
			 /*��������*/
		     DS18B20_DQ_OUT_Low;
			 /*���ٱ���60us�͵�ƽ*/	
			 Delay_us(60);
			 /*�ͷ�����*/
			 DS18B20_DQ_OUT_High;

		 }
		 b=b<<1;  
	}
	     /*�ͷ�����*/
	DS18B20_DQ_OUT_High;
}

/*��һ���ֽ�*/
u8 Read_one_byte()
{
	u8 a=0;
  	u8 i=0;      
	for(i=0;i<8;i++)
	{
	    /*����IO��Ϊ���ģʽ*/ 
	    Gpio_Config_Mode_Out();
	    /*��������*/
	    DS18B20_DQ_OUT_Low;
	    /*�ӳ�1us������ʱ϶*/
	    Delay_us(1);
	    /*�ͷ�����*/
	    DS18B20_DQ_OUT_High;
	    /*����IO��Ϊ��������ģʽ*/
	    Gpio_Config_FLOATING_In();

	    if(DS18B20_DQ_IN)
	    {
	      a=a>>1;
		  a=a|0x80;
	    }
	    else
	    {
	   	  a=a>>1;
	    }
		/*�ӳ�50us��֤ʱ��Ҫ��*/
		Delay_us(50);
		/*����IO��Ϊ���ģʽ*/ 
	    Gpio_Config_Mode_Out();
		/*�ͷ�����*/
	    DS18B20_DQ_OUT_High;
    }
	return(a);
}


/***************
  �¶Ȼ�ȡ�Ӻ���
  ��ڲ���: ��
  ���ڲ���: �¶�
****************/
u8 wen_du_huo_qu()
{
   u16 a1=0;
	 u8 b=0;

	 /*��λDS18B20*/
	 Init_Ds18b20();
	 /*д����ROMָ��*/
	 Write_one_byte(0xCC);
	 /*д�¶�ת��ָ��*/
	 Write_one_byte(0x44);
	 /*�ӳٵȴ�ת�����*/
	 Delay_us(20);
	 /*д���ݴ���ָ��*/
	 Write_one_byte(0xBE);
	 /*���Ͱ�λ*/
	 b=Read_one_byte();
	 /*���߰�λ*/
	 a1=Read_one_byte();
	 /*�Ѹ߰�λ�͵Ͱ�λ��ϳ�16λ*/
	 a1=a1<<8;
	 a1=a1+b;
	 a1=(u16)(a1*0.0625); 
	 return(a1);
}

