
#define _USART_GLOBAL
#include "includes.h"
#include "stdio.h"
#include "stdlib.h"
  
u8  DMA_Rece_Buf[DMA_Rec_Len];	   //DMA���մ������ݻ�����
u16 Usart1_Rec_Cnt=0;             //��֡���ݳ���	
u8  USART1_Send_Buf[Send_len];		//���ͻ�����
u16 Usart1_Send_Length;
u16 Usart1_Send_Cnt;
u8  USART2_Send_Buf[Send_len];		//���ͻ�����
u16 Usart2_Send_Length;
u16 Usart2_Send_Cnt;

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 



//��ʼ��IO ����1 
//bound:������
void uart1_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	  DMA_InitTypeDef DMA_InitStructure;
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
		
 	  USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//���������ж�
		USART_ClearFlag(USART1,USART_IT_TC);					//���������ɱ�־  
		USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);   //ʹ�ܴ���1 DMA����
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
 
    //��Ӧ��DMA����
		DMA_DeInit(DMA1_Channel5);   //��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA����ADC����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Rece_Buf;  //DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
		DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len;  //DMAͨ����DMA����Ĵ�С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
				
    DMA_Cmd(DMA1_Channel5, ENABLE);  //��ʽ����DMA����

}

//���»ָ�DMAָ��
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA_CHx,DMA_Rec_Len);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	

//�ַ�������
u8 * copystring(u8 *des,u8 *src){
u8 *tmp=des;
while( *tmp++ = *src++ ) ;
return des;
}


//����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���
/*
void Usart1_Send(u8 *buf,u8 len)
{
	u8 t;
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,buf[t]);
	}	 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
}
*/
void Usart1_Send(u8 *buf,u16 len)
{
	if(len >= Send_len)	len = Send_len;
	mymemcpy(USART1_Send_Buf,buf,len);
	Usart1_Send_Length = len;
	Usart1_Send_Cnt = 0;
	USART_ITConfig(USART1,USART_IT_TXE,ENABLE);    //�����������������ж�
}



//�����жϺ���
void USART1_IRQHandler(void)                	//����1�жϷ������
{
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
		
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ReceiveData(USART1);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ����Ҳ��֪��Ϊɶ��
		Usart1_Rec_Cnt = DMA_Rec_Len-DMA_GetCurrDataCounter(DMA1_Channel5);	//����ӱ�֡���ݳ���
		//ssp_length = Usart1_Rec_Cnt;
		//mymemcpy(ssp_buf,DMA_Rece_Buf,ssp_length);
		ssp_length = get_sspbuf(Usart1_Rec_Cnt);
		rev_success = 1;
		rev_success2 = 1;
		//***********֡���ݴ�����************//
		printf ("The lenght:%d\r\n",Usart1_Rec_Cnt);
		printf ("The data:\r\n");
		Usart2_Send(DMA_Rece_Buf,Usart1_Rec_Cnt);
		printf ("The ssp_buf:\r\n");
		Usart2_Send(ssp_buf,ssp_length);
		printf ("\r\nOver! \r\n");
		//*************************************//
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);         //����жϱ�־
		MYDMA_Enable(DMA1_Channel5);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���
	}

	if(USART_GetITStatus(USART1,USART_IT_TXE) != RESET)	//�����ж�(���ͼĴ���Ϊ��ʱ)
	{
		if(Usart1_Send_Cnt < Usart1_Send_Length)              
		{
				USART1->DR = USART1_Send_Buf[Usart1_Send_Cnt]&0x01FF;
				Usart1_Send_Cnt++;
		}
		else
		{
			Usart1_Send_Cnt = 0;
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);	//�رշ����ж�
		}
	}
	
		
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 


//��ʼ��IO ����2 
//bound:������
void uart2_init(u32 bound)
{
	 //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
		USART_DeInit(USART2);  //��λ����1
	 //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART2_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10
	
	//Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4 ;//��ռ���ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//�����ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
}

//����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���

void Usart2_Send(u8 *buf,u16 len)
{
	u16 t;
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART2,buf[t]);
	}	 
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
}
//�����жϺ���
void USART2_IRQHandler(void)
{  	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{ 
  		//USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		//USART_SendData(USART1, USART_ReceiveData(USART1));
		printf("%c",USART_ReceiveData(USART2));
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  	}
	
}

//�ӽ��ջ�����ȡ����BB BB��ͷ����0A 0A��β�����ݰ�
u16 get_sspbuf(u16 src_len)
{
	u16 i,start,end,ssp_len;
	for(i = 0; i < src_len;i++){
		if((DMA_Rece_Buf[i] == 0xBB) && (DMA_Rece_Buf[i+1] == 0xBB)){
			start = i;
		}
		if((DMA_Rece_Buf[i] == 0x0A) && (DMA_Rece_Buf[i+1] == 0x0A)){
			end = i;
		}
	}
	ssp_len = end -start;
	for(i = 0; i < ssp_len;i++){
		ssp_buf[i] = DMA_Rece_Buf[start+i];
	}
	return ssp_len;
}
	


