
#ifndef __SENSORS_H
#define __SENSORS_H

#ifndef  _SENSORS_GLOBAL
#define  _SENSORS_GLOBAL  extern 
#else
#define  _SENSORS_GLOBAL 
#endif
#include "sys.h"
#include "sensors_adc.h"
#include "sensors_timer.h"
#include "sensors_gpio.h"
#include "sensors_uart.h"


typedef struct
{
	u32	co_tgs5342;
	u32	voc_tgs2600;
	u32 nis05;
	

}ADC1_value_t;



typedef enum 
{
	VOC_TGS2600_LEVEL_1 = 1,
	VOC_TGS2600_LEVEL_2 = 2,
	VOC_TGS2600_LEVEL_3,
	VOC_TGS2600_LEVEL_4,
	VOC_TGS2600_LEVEL_5,

}_VOC_TGS2600_LEVEL_;



typedef struct
{
	float co_tgs5342;
	float voc_tgs2600_RS_R0;
	float nis05;




	_VOC_TGS2600_LEVEL_	voc_tgs2600_level;	/* ���������ȼ� */
	int nis05_;
	int	co_tgs5342_;

}sensors_value_t;






/*--------------------------- ADC1 ---------------------------------*/


#define ADC1_SCAN_CHANNEL_NUM	3
#define ADC1_FILTER_NUM 		3


/*------------------------ VOC_TGS2600 -------------------------------*/

/* 2600�ڿ����еĵ���ֵ */
#define VOC_TGS2600_R0		20.0

#define VOC_TGS2600_RS_R0_1	0.5
#define VOC_TGS2600_RS_R0_2	0.3
#define VOC_TGS2600_RS_R0_3	0.2
#define VOC_TGS2600_RS_R0_4	0.15






extern ADC1_value_t	ADC1_value;
extern sensors_value_t sensors_value;





void ADC1_value_filter(void);

void sensors_init_all(void);



#endif




















