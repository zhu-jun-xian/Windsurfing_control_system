#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
extern int ZHONGZHI;
extern int pwmpulse;
//外部中断初始化函数
void EXTIX_Init(void)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //外部中断，需要使能AFIO时钟

    KEY_Init();//初始化按键对应io模式

    //GPIOA.5和6 中断线以及中断初始化配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

 NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

    //GPIOA.0	  中断线以及中断初始化配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

   
  //GPIOA.1	  中断线以及中断初始化配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		//GPIOA.2	  中断线以及中断初始化配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器



}


void EXTI0_IRQHandler(void)
{
    delay_ms(10);    //消抖

    if(KEY0 == 1)
    {
        LED0 = !LED0;
			ZHONGZHI=ZHONGZHI-5;
			if(ZHONGZHI >30){
			ZHONGZHI=0;
			}else if(ZHONGZHI <0){ZHONGZHI=30;}
			switch(ZHONGZHI){
				case 0:pwmpulse=899;break;
				case 5:pwmpulse=780;break;
				case 10:pwmpulse=700;
				break;
				case 15:pwmpulse=630;
				break;
				case 20:pwmpulse=550;
				break;
				case 25:pwmpulse=430;
				break;
				case 30:pwmpulse=220;
				break;
			};
	
		
    }

    EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
}
void EXTI1_IRQHandler(void)
{
    delay_ms(10);    //消抖

    if(KEY1 == 1)
    {
        LED0 = !LED0;
				ZHONGZHI=ZHONGZHI+5;
				if(ZHONGZHI >30){
			ZHONGZHI=0;
			}else if(ZHONGZHI <0){ZHONGZHI=30;}
			switch(ZHONGZHI){
				case 0:pwmpulse=899;break;
				case 5:pwmpulse=780;break;
				case 10:pwmpulse=700;
				break;
				case 15:pwmpulse=630;
				break;
				case 20:pwmpulse=550;
				break;
				case 25:pwmpulse=430;
				break;
				case 30:pwmpulse=220;
				break;
			};
	
    }

    EXTI_ClearITPendingBit(EXTI_Line1);  //清除EXTI0线路挂起位
}
void EXTI2_IRQHandler(void)
{
    delay_ms(10);    //消抖

    if(KEY1 == 1)
    {
        LED0 = !LED0;
    }

    EXTI_ClearITPendingBit(EXTI_Line2);  //清除EXTI0线路挂起位
}
void EXTI9_5_IRQHandler(void)
{
    delay_ms(10);   //消抖

    if(KEY3 == 0)
    {
        LED0 = !LED0;
        EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位
    }

    if(KEY4 == 0)
    {
        LED0 = !LED0;
        EXTI_ClearITPendingBit(EXTI_Line6);    //清除LINE5上的中断标志位
    }

}




