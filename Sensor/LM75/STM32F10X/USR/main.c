#include "pbdata.h"

void RCC_Configuration(void)//ʱ�ӳ�ʼ��
{
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(Pin_RCC,ENABLE);
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
	/*PB6,PB7��LM75ʹ�ö˿�*/
	GPIO_InitStructure.GPIO_Pin=SCL_Pin|SDA_Pin;
 	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;   //����IO��Ϊ��©���ģʽ*/ 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init( Pin_group,  &GPIO_InitStructure);
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
	RCC_Configuration();
	GPIO_Configuration();
	USRAT_Configuration();
	NVIC_Configuration();

	while(LM75_init_LM75())
	{
		printf("%s","��ʼ��ʧ�ܣ�δ��⵽LM75\r\n");
		delay_ms(500);
	}
	printf("%s","��ʼ���ɹ�����⵽LM75\r\n");
	LM75_write_Configuration(Low_alert|Comparator_output);
	LM75_write_Low_temperature_alarm(33);
	LM75_write_High_temperature_alarm(35);
	delay_ms(500);
	
	while(1)
	{
		float temperature;
    temperature=LM75_read_temperature()*0.125;
    printf("��ǰ�¶ȣ�%.2f\r\n",temperature);
		delay_ms(500);
	}
}


