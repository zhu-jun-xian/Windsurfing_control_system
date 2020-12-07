#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "stdio.h"
#include "oled.h"
#include "pwm.h"
#include "pid.h"
#include "exti.h"
#include "timer.h"
#include "adc.h"
#include "usart.h"
#include "m5311.h"


//pid_struct mypid =
//{
//    .KP = 20.0,
//    .KI = 0,
//    .KD = 0,
//    .e0 = 0.0,
//    .e1 = 0.0,
//    .e2 = 0.0
//};
u16 adcx;
int pwmpulse;
float temp;
float angle;
float voltage;
char oledBuf[20];
int main(void)
{

    delay_init();	    	 //��ʱ������ʼ��
    Usart1_Init(115200);//debug����
//    usart3_init(115200);
//    M5311_Init();
//    M5311_MQTTInit();
    UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
    LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
    TIM3_Int_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
    //����
    KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
    EXTIX_Init();		//�ⲿ�жϳ�ʼ��
    //OLED
    OLED_Init();
    OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
    OLED_Clear();
    sprintf(oledBuf, "Welcome");
    OLED_ShowString(32, 0, (u8*)oledBuf, 16);
    OLED_Refresh();
    TIM2_Int_Init(4999, 7199);
//    TIM3_Int_Init();
    //L298N
    L298N_Init();
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
		//��Ƶ,PWMƵ��=72000000/72=1Mhz������һ�ε�ʱ��=��1/1Mhz��*��999+1��=1ms
    TIM4_PWM_Init(999, 71);	 
    //��λ��
    Adc_Init();             //ADC��ʼ��
		PIDParament_Init();    //PID������ʼ��

    while(1)
    {
        adcx = Get_Adc_Average(ADC_Channel_7, 10);
        voltage = (float)adcx * (3.3 / 4096); //��ȡ��Ӧ�ĵ�ѹֵ
        angle = (float)voltage * (360 / 3.3);
        UsartPrintf(USART_DEBUG, "angle:%0.2f\r\n", angle);
			  pid_calc(angle , 1);
//        pwmpulse = pid_calc(1);

//        if(pwmpulse > 800)
//        {
//            pwmpulse = 800;
//        }
//        else if(pwmpulse < 0)
//        {
//            pwmpulse = 0;
//        }

        TIM_SetCompare3(TIM4, pid.currpwm);	  //TIM4->CCR2=330 limit:0~330

    }
}

