#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"


#define USART_DEBUG		USART1		//调试打印所使用的串口组
#define USART3_MAX_RECV_LEN		60					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		600					//最大发送缓存字节数
#define USART3_RX_EN 			1					//0,不接收;1,接收.

void usart3_init(u32 bound);				//串口2初始化 
void u3_printf(char* fmt,...);
void USART3_RX_Data(void);

void Usart1_Init(unsigned int baud);

void Usart3_Init(unsigned int baud);
void Usart3_SendStr(char*SendBuf);//串口3打印数据
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

#endif
