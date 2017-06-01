
#define _SENSORS_ADC_GLOBAL

#include "includes.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "sensors.h"


/*-----------------------------------------------------------------------
	ADC1_ConvertedValue[0]:ADC_Channel_10/ PC0
	ADC1_ConvertedValue[1]:ADC_Channel_11/ PC1
	ADC1_ConvertedValue[2]:ADC_Channel_12/ PC2
------------------------------------------------------------------------*/
__IO uint16_t ADC1_ConvertedValue[ADC1_SCAN_CHANNEL_NUM];



/*-----------------------------------------------------------------------
	ʹ��DMA1/ADC1��ʱ��
	��ʼ��ģ������
------------------------------------------------------------------------*/
void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

	
	/* Configure PC.00/01/02	as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);				

	/* Configure PA1 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);				


	
}


/*-----------------------------------------------------------------------
	ADC1ģʽ����
------------------------------------------------------------------------*/
void ADC1_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	ADC_DeInit(ADC1); //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 		//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode =ENABLE; 			//ģ��ת��������ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 		//ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 	//�ⲿ����ת���ر�
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = ADC1_SCAN_CHANNEL_NUM; 			//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure); 					//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

	// ����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz 
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	//ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5 );

	ADC_DMACmd(ADC1, ENABLE); 	/* Enable ADC1 DMA */ 
	
	ADC_Cmd(ADC1, ENABLE); 		/* Enable ADC1 */ 
	
	ADC_ResetCalibration(ADC1); /* ��λУ׼�Ĵ��� */ 
	
	/* �ȴ�У׼�Ĵ�����λ��� */ 
	while(ADC_GetResetCalibrationStatus(ADC1)); 
	
	ADC_StartCalibration(ADC1); 			/* ADCУ׼ */ 
	
	while(ADC_GetCalibrationStatus(ADC1)); 	/* �ȴ�У׼��� */ 
	
}


/*-----------------------------------------------------------------------
	ADC1 MDA1ģʽ����
------------------------------------------------------------------------*/
void ADC1_DMA1_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA1_Channel1);  		/* DMA channel1 configuration */ 

	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; 			//DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC1_ConvertedValue; 	//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 					//�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = ADC1_SCAN_CHANNEL_NUM; 			//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 			//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 		//���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 			//������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 		//DMAͨ�� xӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 				//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��

	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 				//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

}



/*-----------------------------------------------------------------------
	ADC1�ĳ�ʼ��
	ע��:ADC��У׼Ҫ��DMA֮ǰ��DMA��ʹ��Ҫ��ADC֮ǰ������DMA���ݴ�λ
	����ͨ��֮���������?
------------------------------------------------------------------------*/
void ADC1_Init_All(void)
{

	ADC1_GPIO_Config();

	ADC1_DMA1_Config();

	/* У׼��ɣ�ADCУ׼Ҫ��DMAʹ��֮ǰ */ 	
	/*	
		ԭ������ڳ�ʼ��AD��ʱ��DMA��������һ�Σ����Ǵ�ʱ��û�в���������DMAĿ�ĵ�ַ�Ѿ��������Լӣ�
		���������һ·��ʱ������ȴ��䵽�˵ڶ�·��У׼AD��ʱ��ᴥ��DMA����ͨ����λ�����У׼AD��׼ǰ��Ҫ����DMA��
	*/
	ADC1_Mode_Config();	

	/* Enable DMA channel1,DMAʹ��Ҫ��ADC֮ǰ */ 
	DMA_Cmd(DMA1_Channel1, ENABLE); 
	
 	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 



}










