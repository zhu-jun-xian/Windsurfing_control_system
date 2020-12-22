#ifndef __M5311_H
#define __M5311_H
#include "usart.h"
#include <stm32f10x.h>
#include "delay.h"


#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志
void Clear_Buffer(void);//清空缓存
void M5311_Init(void);
void M5311_CreateUDPSokcet(void);
void M5311_UDPSend(uint8_t *len, uint8_t *data);
void M5311_CreateSokcet(void);
void Clear_Buffer(void);
void M5311_ChecekConStatus(void);
void M5311_RECData(void);
void M5311_CreateInstance(void);
void ONENET_Readdata(void);
void ONENET_Senddata(u8*buffer);
void StringToHex(char *str, unsigned char *strhex);
void M5311_MQTTInit(void);
void M5311_MQTTPUBMessage(char*len, char*data);
void HexToHexString(u8 *p, u8 *buffer);
void M5311_SUB(void);

typedef struct
{
    uint8_t CSQ;
    uint8_t Senddata[100];
    uint8_t Senddatalen[50];
    uint8_t  Headdata[20];
    uint8_t  HexStringdata[100];
    uint8_t  ALLStringdata[100];
} M5311;


#endif





