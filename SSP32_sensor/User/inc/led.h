#ifndef __LED_H
#define __LED_H

#ifndef  _LED_GLOBAL
#define  LED_EXT  extern 
#else
#define  LED_EXT 
#endif

#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED1_Toggle		(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_9))?(GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET)):(GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET))
#define LED2_Toggle		(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_10))?(GPIO_WriteBit(GPIOC,GPIO_Pin_10,Bit_RESET)):(GPIO_WriteBit(GPIOC,GPIO_Pin_10,Bit_SET))
#define LED3_Toggle		(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_11))?(GPIO_WriteBit(GPIOC,GPIO_Pin_11,Bit_RESET)):(GPIO_WriteBit(GPIOC,GPIO_Pin_11,Bit_SET))
#define LED4_Toggle		(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_12))?(GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_RESET)):(GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET))


LED_EXT void LED_Init(void);//��ʼ��

		 				    
#endif
