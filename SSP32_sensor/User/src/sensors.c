
#define _SENSORS_GLOBAL

#include "includes.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

#include "sensors.h"



sensors_value_t sensors_value;

ADC1_value_t	ADC1_value;





/*-----------------------------------------------------------------------
	每100ms执行一次
------------------------------------------------------------------------*/
void ADC1_value_filter(void)
{
	static int32_t filterFlag = 0;
	static ADC1_value_t filterData[ADC1_FILTER_NUM];
	int32_t i;

	filterData[filterFlag].co_tgs5342 = ADC1_ConvertedValue[1];
	filterData[filterFlag].voc_tgs2600 = ADC1_ConvertedValue[0];
	filterData[filterFlag].nis05 = ADC1_ConvertedValue[2];
	filterFlag++;

	if(filterFlag >= ADC1_FILTER_NUM){		/* 保存ADC1_FILTER_NUM 次数据后跟上一次的数据一起算平均 */

		for(i=1; i<ADC1_FILTER_NUM; i++){	/* 这里不直接使用ADC1_value，避免其他地方获取ADC1_value是出现差错 */
			filterData[0].co_tgs5342 += filterData[i].co_tgs5342;
			filterData[0].voc_tgs2600 += filterData[i].voc_tgs2600;
			filterData[0].nis05 += filterData[i].nis05;	
		}
		filterData[0].co_tgs5342 += ADC1_value.co_tgs5342;
		filterData[0].voc_tgs2600 += ADC1_value.voc_tgs2600;
		filterData[0].nis05 += ADC1_value.nis05; 

		ADC1_value.co_tgs5342 = filterData[0].co_tgs5342 / (ADC1_FILTER_NUM + 1);
		ADC1_value.voc_tgs2600 = filterData[0].voc_tgs2600 / (ADC1_FILTER_NUM + 1);
		ADC1_value.nis05 = filterData[0].nis05 / (ADC1_FILTER_NUM + 1);


		sensors_value.voc_tgs2600_RS_R0 = (4096.0/ADC1_value.voc_tgs2600 - 1.0) / VOC_TGS2600_R0;
		if(sensors_value.voc_tgs2600_RS_R0 > VOC_TGS2600_RS_R0_1){
			sensors_value.voc_tgs2600_level = VOC_TGS2600_LEVEL_1;
			
		}else if(sensors_value.voc_tgs2600_RS_R0 > VOC_TGS2600_RS_R0_2){
			sensors_value.voc_tgs2600_level = VOC_TGS2600_LEVEL_2;
			
		}else if(sensors_value.voc_tgs2600_RS_R0 > VOC_TGS2600_RS_R0_3){
			sensors_value.voc_tgs2600_level = VOC_TGS2600_LEVEL_3;
			
		}else if(sensors_value.voc_tgs2600_RS_R0 > VOC_TGS2600_RS_R0_4){
			sensors_value.voc_tgs2600_level = VOC_TGS2600_LEVEL_4;

		}else{
			sensors_value.voc_tgs2600_level = VOC_TGS2600_LEVEL_5;
		}

		printf("\n co_tgs5342:%4d   voc_tgs2600:%4d   nis05:%4d", ADC1_value.co_tgs5342, ADC1_value.voc_tgs2600, ADC1_value.nis05);
		
		printf("\n co_tgs5342:ADC1_value:%4d    RS_R0:%.03f   level:%d", ADC1_value.voc_tgs2600, sensors_value.voc_tgs2600_RS_R0, sensors_value.voc_tgs2600_level);

		filterFlag = 0;
		
	}


}




/*-----------------------------------------------------------------------
	每100ms执行一次
------------------------------------------------------------------------*/
void sensors_init_all(void)
{

	printf("\n 1. ADC1_Init_All.");
	ADC1_Init_All();

	printf("\n 2. GP2Y1023_pwm_init.");
	GP2Y1023_pwm_init();

	printf("\n test_gpio_config.");
	test_gpio_config();



	printf("\n 3. CO2_usart3_init.");
	CO2_usart3_init();



}
















