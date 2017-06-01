
#ifndef __SENSORS_UART_H
#define __SENSORS_UART_H

#ifndef  _SENSORS_UART_GLOBAL
#define  _SENSORS_UART_GLOBAL  extern 
#else
#define  _SENSORS_UART_GLOBAL 
#endif
#include "sys.h"








/*-----------------------------------------------------------------------

	CO2 uart ����˵��
	�ж���������Ҫ����ʱ��ÿ��100ms����һ�����
	��ȡ����ÿ1s����һ��
	��������������������໥��������ͬʱ���ֶ��������Ҫ����ʱ��
	���ȼ�����:
	setrFlag > atuoFlag > spanFlag > zeroFlag > readFlag
	�����ϣ�spanFlag��zeroFlag����ͬʱ���֣���Ϊ���������������Ҫ
	��Ӧ��Ũ�Ȼ�����
	ÿ������ķ���ʱ��Ϊ9*10/9600=10ms.
	

	�����־˵��:
		0: ��ʾ������
		1: ��ʾ��Ҫ���͸�����


	sendFlag��־˵��:
		0: ��ʾû�з���
		1: ��ʾ���ڷ��������ʼ���ͽ��̣�������ɻص�0״̬
		
//		2: ��ʾ���������ɣ����ڵȴ�һ�����ӳ�(50ms�ӳ�ʹ�õ��Ƕ�ʱ���жϵĶ�ʱ��
//			������ֱ�ӵȴ��ӳ�)��ȴ��ظ����(ʵ���д����������ϻظ���
//			���һظ�ʱ��Ϊ10ms����)���ӳ�ʱ�䵽���ʾ�÷��ͽ��̽������ص�0״̬��

			

------------------------------------------------------------------------*/

#define CO2_TXRX_BUF_LEN	12
#define CO2_TXRX_DATA_LEN	9

typedef struct
{

	uint8_t 	readFlag;		/* 1������־ */
	uint8_t 	readCtrl;		/* ����ÿ1s��ȡһ������  */	
	uint16_t 	readValue;		/* ��ȡ��Ũ��ֵ  */

	uint8_t 	zeroFlag;		/* 2��У׼��־*/

	uint8_t 	spanFlag;		/* 3�� */
	uint8_t 	spanHigh;		/* SPAN���ֽ� */
	uint8_t 	spanLow;		/* SPAN���ֽ� */

	uint8_t 	atuoFlag;		/* 4���Զ�У׼����־ */
	uint8_t 	atuoValue;		/* �Զ�У׼���� */

	uint8_t 	setrFlag;		/* 5���������̱�־ */
	uint8_t 	setrHigh;		/* �������̸��ֽ� */
	uint8_t 	setrLow;		/* �������̵��ֽ� */


	uint8_t		txLen;			/* ��Ҫ���͵��ֽ��� */
	uint8_t		rxLen;
	uint8_t		hasTxLen;		/* �Ѿ����͵��ֽ���  */
	uint8_t		sendFlag;		/* ���ڷ��͵ĵı�־  */


	
	uint8_t		txBuf[CO2_TXRX_BUF_LEN];	
	uint8_t		rxBuf[CO2_TXRX_BUF_LEN];

}CO2_uart_t;



extern CO2_uart_t CO2_uart;




void CO2_usart3_init(void);
void CO2_usart3_send(void);

void CO2_zero_config(void);
void CO2_atuo_config(uint16_t setrValue);
void CO2_setr_config(uint16_t setrValue);
void CO2_SPAN_config(uint16_t spanValue);





#endif




















