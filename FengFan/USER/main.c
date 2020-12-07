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


pid_struct mypid =
{
    .KP = 20.0,
    .KI = 0,
    .KD = 0,
    .e0 = 0.0,
    .e1 = 0.0,
    .e2 = 0.0
};
u16 adcx;
int pwmpulse;
float temp;
float angle;
float angle1;
float voltage;
char oledBuf[20];
int ZHONGZHI;
int Balance_KP = 25;
int Balance_KD = 0;

int blance(float Angle)
{
	float Bias;
	static float Last_Bias,D_Bias;
	int blance;
	Bias = Angle-ZHONGZHI;
	D_Bias =Bias-Last_Bias;
	
	blance =-Balance_KP * Bias - D_Bias * Balance_KD;
	Last_Bias = Bias;
	return blance;
}

int main(void)
{
ZHONGZHI=0;
	pwmpulse=899;
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
//    sprintf(oledBuf, "Welcome");
//    OLED_ShowString(32, 0, (u8*)oledBuf, 16);
    OLED_Refresh();
    TIM2_Int_Init(4999, 7199);
    //L298N
    L298N_Init();
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
    TIM4_PWM_Init(899, 0);	 //����Ƶ,PWMƵ��=72000000/900=80Khz
    //��λ��
    Adc_Init();             //ADC��ʼ��
	int flag=0;
    while(1)
    {
        adcx = Get_Adc_Average(ADC_Channel_7, 10);
        voltage = (float)adcx * (3.3 / 4096); //��ȡ��Ӧ�ĵ�ѹֵ
				angle1 = 360 - (float)voltage * (360 / 3.3);
				if(angle1<1){
					angle=0.000;
					}else if(angle1>100){
						angle=0.000;
					}
					else{
					angle=angle1;
					}
					
        UsartPrintf(USART_DEBUG, "angle:%0.2f\r\n", angle);
//        pwmpulse = Position_PID(angle, 10, mypid);
//				 pwmpulse =blance(angle);
//			  if(pwmpulse > 800)
//        {
//            pwmpulse = 10;
//        }
//        else if(pwmpulse < 0)
//        {
//            pwmpulse = 890;
//        }
//if(ZHONGZHI >35){
//			ZHONGZHI=0;
//			}else if(c <=0){ZHONGZHI=35;}
			
//				;
//	

// flag++;     
        TIM_SetCompare3(TIM4,pwmpulse);	  //TIM4->CCR2=800 limit:0~899
//				if(flag==50){		
////				
////				 sprintf(oledBuf, "WelcomeByZJX");
////        OLED_ShowString(16, 0, (u8*)oledBuf, 16);
////        sprintf(oledBuf, "Angle:%0.0f", angle);
////        OLED_ShowString(0, 16, (u8*)oledBuf, 16);
////        sprintf(oledBuf, "PWM:%d", pwmpulse);
////        OLED_ShowString(0, 32, (u8*)oledBuf, 16);
////        sprintf(oledBuf, "Voltage:%0.2f",voltage);
////        OLED_ShowString(0, 48, (u8*)oledBuf, 16);
//					OLED_Reflesh();
//				flag=0;
//				}
    }
}

