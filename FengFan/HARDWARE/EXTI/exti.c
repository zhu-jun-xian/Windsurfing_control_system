#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
extern int ZHONGZHI;
extern int pwmpulse;
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //�ⲿ�жϣ���Ҫʹ��AFIOʱ��

    KEY_Init();//��ʼ��������Ӧioģʽ

    //GPIOA.5��6 �ж����Լ��жϳ�ʼ������
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

 NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);

    //GPIOA.0	  �ж����Լ��жϳ�ʼ������
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

   
  //GPIOA.1	  �ж����Լ��жϳ�ʼ������
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
		//GPIOA.2	  �ж����Լ��жϳ�ʼ������
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���



}


void EXTI0_IRQHandler(void)
{
    delay_ms(10);    //����

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

    EXTI_ClearITPendingBit(EXTI_Line0);  //���EXTI0��·����λ
}
void EXTI1_IRQHandler(void)
{
    delay_ms(10);    //����

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

    EXTI_ClearITPendingBit(EXTI_Line1);  //���EXTI0��·����λ
}
void EXTI2_IRQHandler(void)
{
    delay_ms(10);    //����

    if(KEY1 == 1)
    {
        LED0 = !LED0;
    }

    EXTI_ClearITPendingBit(EXTI_Line2);  //���EXTI0��·����λ
}
void EXTI9_5_IRQHandler(void)
{
    delay_ms(10);   //����

    if(KEY3 == 0)
    {
        LED0 = !LED0;
        EXTI_ClearITPendingBit(EXTI_Line5);    //���LINE5�ϵ��жϱ�־λ
    }

    if(KEY4 == 0)
    {
        LED0 = !LED0;
        EXTI_ClearITPendingBit(EXTI_Line6);    //���LINE5�ϵ��жϱ�־λ
    }

}




