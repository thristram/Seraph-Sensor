
#define _SENSORS_GPIO_GLOBAL

#include "includes.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "sensors.h"




/*-------------------------------------------------------------------------------------------


STM32��ÿ��IO�ڶ�������Ϊ�ж����룬Ҫ��IO����Ϊ�ⲿ�ж����룬��һ�¼������裺
��һ�� ��ʼ��IO����Ϊ����
	������Ҫ��Ϊ�ⲿ�ж������IO�ڵ�״̬����������Ϊ����/�������룬Ҳ��������Ϊ�������룬
���Ǹ��յ�ʱ��һ��Ҫ�����������������裬������ܵ����жϲ�ͣ�Ĵ������ڸ��Žϴ�ĵط���
����ʹ��������/������Ҳ����ʹ���ⲿ����/�������裬��������һ���̶ȷ�ֹ�ⲿ���Ŵ�����Ӱ�졣

�ڶ��� ����IO�ڸ���ʱ�ӣ�����IO�����ж��ߵ�ӳ���ϵ
	STM32��IO�����ж��ߵĶ�Ӧ��ϵ��Ҫ�����ⲿ�жϼĴ�����EXTICR��,��������Ҫ�ȿ�������ʱ�ӣ�
Ȼ������IO�����ж��ߵĶ�Ӧ��ϵ�����ܰ��ⲿ�ж����ж�������������

������ �������IO����Ե������ж�/�¼������ô�������
	����Ҫ�����жϵĲ�����������STM32�������ó������ش������½��ش��������������ƽ������
���ǲ������óɸߵ�ƽ���ߵ͵�ƽ��������������Լ���ʵ����������á�
ע���ڶ����������������Ϊһ������������

���Ĳ� �����жϷ��飨NVIC��,����ʹ���ж�
	��һ�������������жϷ��飬�Լ�ʹ�ܡ���STM32���ж���˵��ֻ��������NVIC�����ã����������ܱ�
ִ�У������ǲ���ִ�е��жϷ���������ȥ�ġ�

���岽 ��д�жϷ�����
	�����ж����õ����һ�����жϷ��������Ǳز����ٵģ���������￪�����жϣ�����û��д�жϷ���
�������Ϳ�������Ӳ�����󣬴Ӷ����³�������������ڿ�����ĳ���жϺ�һ��Ҫ�ǵ�Ϊ���жϱ�д����
���������жϷ������б�д��Ҫִ�е��жϺ�Ĳ�����

----------------------------------------------------------------------------------------------*/




/*-----------------------------------------------------------------------
	1����ʼ������Ϊ��������
------------------------------------------------------------------------*/
void PM25_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;         		/* PA4 */                                   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//���Ƶ�����50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//�������������

	GPIO_Init(GPIOA, &GPIO_InitStructure);
      
}

/*-----------------------------------------------------------------------
	2��ʹ��AFIO����ʱ�ӹ���
------------------------------------------------------------------------*/
void PM25_AFIO_config(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//ʹ��ʱ��

}

/*-----------------------------------------------------------------------
	3����GPIO�����ж���ӳ������ 
	�������IO����Ե������ж�/�¼������ô�������
------------------------------------------------------------------------*/
void PM25_EXTILine_config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	 
	EXTI_ClearITPendingBit(EXTI_Line4);		//����жϱ�־

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);		//ѡ���жϹܽ�PA4
	 
	EXTI_InitStructure.EXTI_Line = EXTI_Line4; 						//ѡ���ж���·4
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 			//����Ϊ�ж����󣬷��¼�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 	//�����жϴ�����ʽΪ���½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						//�ⲿ�ж�ʹ��
	
	EXTI_Init(&EXTI_InitStructure);

}

/*-----------------------------------------------------------------------
	4�������жϷ��飨NVIC��,����ʹ���ж�
------------------------------------------------------------------------*/
void PM25_NVIC_config(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;	//����ṹ��

//	NVIC_Configuration();	//ǰ���Ѿ��������жϷ���2

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 		//ʹ���ⲿ�ж����ڵ�ͨ��

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ� 2�� 

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; 	//�����ȼ� 2

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	//ʹ���ⲿ�ж�ͨ�� 

	NVIC_Init(&NVIC_InitStructure); 		//���ݽṹ����Ϣ�������ȼ���ʼ�� 


}







/*-----------------------------------------------------------------------
	5���жϷ�����
------------------------------------------------------------------------*/
void EXTI4_IRQHandler(void)
{
	
	printf("\n EXTI4_IRQHandler");
	   
	EXTI_ClearITPendingBit(EXTI_Line4);	//��� LINE �ϵ��жϱ�־λ
	
}


/*-----------------------------------------------------------------------
	��ʼ��
------------------------------------------------------------------------*/
void PM25_EXTI_init(void)
{
	PM25_gpio_config();
	PM25_AFIO_config();
	PM25_EXTILine_config();
	PM25_NVIC_config();
}







