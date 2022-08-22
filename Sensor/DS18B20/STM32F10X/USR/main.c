/************************************************
 ����:STM32F103ZET6����DS18B20�ɼ��¶ȴ���1���
 
 ����˵��
	DS18B20       STM32F10X
	VCC           3V3
	GND           GND
	D0            PA0
	
	���ڲ�����96N81
 
	����δ��װ,��������ʹ��,��������ֲ
 
 ���ߣ�JXL
************************************************/

#include "pbdata.h"

void RCC_Configuration(void)//ʱ�ӳ�ʼ��
{
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
}

void GPIO_Configuration(void)//GPIO�˿�����
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*PA9,tx*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*PA10,rx*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*PA0��DS18B20ʹ�ö˿�*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
 	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;   //����IO��Ϊ��©���ģʽ*/ 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init( GPIOA,  &GPIO_InitStructure);
	
	//GPIO_InitTypeDef GPIO_InitStructure;
//	/*PA2,tx*/
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	/*PA3,rx*/
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	/*led*/
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void NVIC_Configuration(void)//NVIC�Ĵ�������
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//���÷���
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//ʹ��USART1�ж�
	
	//NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;//ʹ��USART2�ж�
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//������Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ���õ�IRQChannelͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);//��ʼ����ָ������
}

void USRAT_Configuration(void)//USART����
{
	USART_InitTypeDef USART_InitStructure;//����USART�Ľṹ�����
	USART_InitStructure.USART_BaudRate=9600;//���ò�����
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//��������λΪ8λ����
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//����ֹͣλ��ĿΪ1
	USART_InitStructure.USART_Parity=USART_Parity_No;//��������żЧ��λ��96N8��û��У��λ��
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//û��Ӳ�������ƣ�����Ӳ��������ʧ��
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//����ʧ�ܺͽ���ʧ��
	USART_Init(USART1,&USART_InitStructure);//��ʼ��Ϊ������ָ������
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//ʹ�ܻ�ʧ��ָ����USART�жϣ�����ʹ��USART�Ľ����ж�
	USART_Cmd(USART1,ENABLE);//ʹ�ܻ�ʧ��ָ����USART����
	
	USART_ClearFlag(USART1,USART_FLAG_TC/*������ɱ�־λ*/);//���USARTx�Ĵ������־λ
}

int fputc(int ch,FILE *f)//��д��stdio.h�еĺ�����
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//�ж������Ƿ������
	return ch;
}

//�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

int main(void)
{
	
	unsigned char TL;
	unsigned char TH;
	
	unsigned char zhengshu;
	unsigned char xiaoshu;
	
	RCC_Configuration();
	GPIO_Configuration();
	USRAT_Configuration();
	NVIC_Configuration();
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
	if(init18B20())
	{
		printf("δ��⵽DS18B20!");
	}
	else
	{
		printf("OK!");
	}
	
	while(1)
	{
		readwen();
		TL=read18B20();
		TH=read18B20();

		zhengshu=TH*16+TL/16;
		xiaoshu=(TL%16)*10/16;
		printf("�¶ȣ�%d.%d\r\n",zhengshu,xiaoshu);
		
		delay_ms(1000);
	}
}


