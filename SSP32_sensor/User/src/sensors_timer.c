
#define _SENSORS_TIMER_GLOBAL

#include "includes.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "sensors.h"



GP2Y1023_value_t GP2Y1023_value;




/*-----------------------------------------------------------------------
	1����ʼ������Ϊ��������
------------------------------------------------------------------------*/
void GP2Y1023_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;         		/* PA7 */                                   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//���Ƶ�����50MHz
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//�������������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	  
}


/*-----------------------------------------------------------------------
	2�������жϷ��飨NVIC��,����ʹ���ж�
------------------------------------------------------------------------*/
void GP2Y1023_NVIC_config(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;	//����ṹ��

//	NVIC_Configuration();	//ǰ���Ѿ��������жϷ���2
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 		 //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //��ռ���ȼ� 1�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; 	//�����ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	//ʹ���ⲿ�ж�ͨ�� 

	NVIC_Init(&NVIC_InitStructure); 		//���ݽṹ����Ϣ�������ȼ���ʼ�� 

}




/*-----------------------------------------------------------------------
	3����ʼ������Ϊ��������
------------------------------------------------------------------------*/
void GP2Y1023_timer_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_DeInit(TIM3); //��ʼ��TIM3Ϊȱʡֵ 0

	TIM_TimeBaseStructure.TIM_Period = 0xffff; 
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);



 	//�����Ƕ� TIM_ICInitStructure������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    
	TIM_ICInitStructure.TIM_ICFilter = 0x0;      

	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);  
		
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI2FP2);  		/* Select the TIM2 Input Trigger: TI2FP2 */  
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);  	/* Select the slave Mode: Reset Mode */  
	TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);  
	
	/* TIM enable counter */  
	TIM_Cmd(TIM3, ENABLE);	
	
	/* Enable the CC2 Interrupt Request */	
	TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE); 


}


/*-----------------------------------------------------------------------
	4���жϷ�����
------------------------------------------------------------------------*/
void TIM3_IRQHandler(void)
{

	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2); 	//���TIM3���жϴ�����λ
	
	GP2Y1023_value.IC2Value = TIM_GetCapture2(TIM3); //��ȡIC2����Ĵ�����ֵ����ΪPWM���ڵļ���ֵ
	GP2Y1023_value.IC1Value = TIM_GetCapture1(TIM3); //��ȡIC2����Ĵ�����ֵ����ΪPWM���ڵļ���ֵ

	if(GP2Y1023_value.IC2Value != 0){
		

		GP2Y1023_value.dutyCycle = (float)GP2Y1023_value.IC1Value / GP2Y1023_value.IC2Value; //��ȡIC1����Ĵ�����ֵ��������ռ�ձ�
		GP2Y1023_value.frequency = GP2Y1023_value.timerFreq / GP2Y1023_value.IC2Value; //����PWMƵ�ʡ�

//		printf("\n GP2Y1023 IC2Value:%d	IC1Value:%d	dutyCycle:%lf	frequency:%d", 
//			GP2Y1023_value.IC2Value, TIM_GetCapture1(TIM3), GP2Y1023_value.dutyCycle, GP2Y1023_value.frequency);


	}else{
	
		GP2Y1023_value.dutyCycle = 0;
		GP2Y1023_value.frequency = 0;

	}
	
	
}


/*-----------------------------------------------------------------------
	��ʼ��
------------------------------------------------------------------------*/
void GP2Y1023_pwm_init(void)
{
	RCC_ClocksTypeDef	RCC_Clocks;

	GP2Y1023_gpio_config();

	GP2Y1023_NVIC_config();

	GP2Y1023_timer_config();

	RCC_GetClocksFreq(&RCC_Clocks);  
	
	GP2Y1023_value.timerFreq = RCC_Clocks.SYSCLK_Frequency / 720;
	printf("\n RCC_Clocks HCLK:%dHz SYSCLK:%dHz GP2Y1023_value.timerFreq:%dHz", RCC_Clocks.HCLK_Frequency, 
			RCC_Clocks.SYSCLK_Frequency, GP2Y1023_value.timerFreq);
	
}






