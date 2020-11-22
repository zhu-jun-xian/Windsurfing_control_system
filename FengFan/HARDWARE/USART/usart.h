#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"


#define USART_DEBUG		USART1		//���Դ�ӡ��ʹ�õĴ�����
#define USART3_MAX_RECV_LEN		60					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

void usart3_init(u32 bound);				//����2��ʼ�� 
void u3_printf(char* fmt,...);
void USART3_RX_Data(void);

void Usart1_Init(unsigned int baud);

void Usart3_Init(unsigned int baud);
void Usart3_SendStr(char*SendBuf);//����3��ӡ����
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

#endif
