#include "includes.h"
#include "sensors.h"


void Task2ms(void);
void Task100ms(void);
void Task300ms(void);
void Task1s(void);
void Task2s(void);

void system_init(void)
{
	
}


 void test_gpio_config(void)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能时钟
 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 /* PA4 */									 
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //输出频率最大50MHz
 //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			 //带上拉电阻输出
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
 
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
 
 //  GPIO_SetBits(GPIOA, GPIO_Pin_4);
	 GPIO_ResetBits(GPIOA, GPIO_Pin_4);
 }
 
 
 void test_gpio_pwm(void)
 {
	 static int i = 0;
	 if(i){
		 GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		 i = 0;
	 }else{
		 GPIO_SetBits(GPIOA, GPIO_Pin_4);
		 i = 1;
	 }
 //  GPIO_SetBits(GPIOA, GPIO_Pin_4);
 //  GPIO_ResetBits(GPIOA, GPIO_Pin_4);
 }




 int main(void)
 {
	TIM2_Init();
	delay_init();	    	 //延时函数初始化	
	NVIC_Configuration();		// 设置中断优先级分组
	uart1_init(115200);	 //串口初始化为9600
	uart2_init(115200);
	LED_Init();		  	 //初始化与LED连接的硬件接口 
	system_init();
	printf("Hello!\r\n");
	//test_cjson();
	printf("acsfaflaslfk!\r\n");

	sensors_init_all();


	while(1)
	{
		Task2ms();
		Task100ms();
		Task1s();




		
	}	 

	
}


void Task2ms(void)
{
	if(f_2ms)
	{
		f_2ms = 0;
		
		
	}
}

void Task100ms(void)
{
	if(f_100ms)
	{
		f_100ms = 0;
		//init_send_Txmessage(CONTINUE);

		ADC1_value_filter();
		test_gpio_pwm();

		CO2_usart3_send();


	}
}

void Task300ms(void)
{
	if(f_300ms)
	{
		f_300ms = 0;


	}
}

void Task1s(void)
{
	
	if(f_1s)
	{
		f_1s = 0;
		LED1_Toggle;
//		LED2_Toggle;
//		LED3_Toggle;
//		LED4_Toggle;
	CO2_zero_config();
	CO2_atuo_config(1);
	CO2_setr_config(5000);
	


	}
}

void Task2s(void)
{
	if(f_2s)
	{
		f_2s = 0;
	}
}
