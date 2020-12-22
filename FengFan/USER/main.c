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
u16 adcx;
float error;
signed long pwmpulse;
signed long pwmpulse1;
signed long pwm=0;
float temp;
float angle;
float angle1;
float voltage;
char oledBuf[20];
int ZHONGZHI;
int Balance_KP = 25;
int Balance_KD = 0;
float x,y,z;
int flag;
int flag1;
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
	
ZHONGZHI=15;
	pwmpulse1=0;
	pwmpulse=0;
	flag1=0;
	flag=0;
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
    TIM2_Int_Init(100, 7199);
			  IIC_init();
	 adxl345_init();
    //L298N
    L298N_Init();
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
		
		
//		  GPIO_SetBits(GPIOA, GPIO_Pin_4);
//     GPIO_ResetBits(GPIOA, GPIO_Pin_3);
    TIM4_PWM_Init(35999, 0);	 //����Ƶ,PWMƵ��=72000000/900=80Khz
    //��λ��
    Adc_Init();             //ADC��ʼ��

    while(1)
    {
//			adxl345_read_average(&x,&y,&z,10);//????
//	x=atan(y/z)*(-180)/3.14;
//	y=atan(x/z)*180/3.14;
//	z=atan(x/y)*180/3.14;
        
//pwmpulse1 =35999-PID_Control(ZHONGZHI*13.04 + 2080);
//			 pwmpulse1 = 15 * (10-angle) + 1.5 * (10-angle) ;//�������ʽPID���
			  
//				if(angle1<1){
//					angle=0.000;
//					}else if(angle1>100){
//						angle=0.000;
//					}
//					else{
//					angle=angle1;
//					}
//					UsartPrintf(USART_DEBUG, "x:%f,y:%f,z:%f\r\n",x,y,z);
//										delay_ms(500);
       	adcx = Get_Adc_Average(ADC_Channel_7, 10);
        voltage = (float)adcx * (3.3 / 4096); //��ȡ��Ӧ�ĵ�ѹֵ
				angle1 = 360-(float)voltage * (360 / 3.3);
				delay_ms(500);				
				UsartPrintf(USART_DEBUG, "Tangle:%d ,angle:%0.2f,pwmpulse: %d,pwmpulse1: %d,error:%f,adcx:%d\r\n",ZHONGZHI,angle,pwmpulse,pwmpulse1,error,adcx);
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
    }
}

