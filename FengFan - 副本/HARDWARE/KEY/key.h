#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"	 


#define KEY0 PAin(0) 
#define KEY1 PAin(1) 
#define KEY2 PAin(2) 
#define KEY3 PAin(5) 
#define KEY4 PAin(6) 

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��					    
#endif
