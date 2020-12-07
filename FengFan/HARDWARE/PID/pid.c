#include "pid.h"
#include "usart.h"   //USART����

PID pid;

void PIDParament_Init()  //
{
    pid.choose_model = MODEL_PID;
    pid.T=330;                //�������ڣ���ʱ��ʹ��1ms������Сִ��PID������Ϊ330ms
    
    pid.TargerAngle =30;      //�û��趨ֵ
    pid.Kp=0.5;               //����ϵ��
    pid.Ti=40;                //΢��ϵ������
    pid.Td=10;                //����ʱ�䳣��
    pid.OUT0=0;               //һ��ά�ֵ����

    pid.pwmcycle = 330;       //PWM������
}

    
void pid_calc(float angle , u8 choose_model)   //pid??
{
		float dk1,dk2;        //����ƫ�����ϴ�ƫ��֮��
		float pout,iout,dout;
    
		pid.Tdata = 888;
    if(pid.Tdata < (pid.T))  //��С��������δ��
     {
            return ;
     }
    pid.Tdata = TIM_GetCounter(TIM4);
    pid.angle = angle;
    pid.En=pid.TargerAngle-pid.angle;  //�������
    dk1=pid.En-pid.En_1;   //����ƫ�����ϴ�ƫ��֮��
    dk2=pid.En-2*pid.En_1+pid.En_2;
    
    pout=pid.Kp*dk1;                            //����
    
    iout=(pid.Kp*pid.T)/pid.Ti;      //����
    iout=iout*pid.En;
    
    dout=(pid.Kp*pid.Td)/pid.T;        //΢��
    dout=dout*dk2;
    
    switch(pid.choose_model)
     {
         case MODEL_P:     pid.Dout= pout;            printf("ʹ��P����\r\n") ;
             break;
         
         case MODEL_PI:  pid.Dout= pout+iout;           printf("ʹ��PI����\r\n") ;
             break;
                 
         case MODEL_PID: pid.Dout= pout+iout+dout;        printf("ʹ��PID����\r\n") ;
             break;
     } 
          
    pid.currpwm+=pid.Dout;  //����Ӧ�������PWM
    printf("PID��õ�OUT:\t%d\r\n",(int)pid.currpwm) ;
     
    /*�ж���������Ƿ���Ͽ���Ҫ��*/
    if(pid.currpwm>pid.pwmcycle)            //�����ֵȡֵ���϶�����0-pid.pwmcycle֮�䣬��Ȼ�Ļ�PWM��ô���
    {
      pid.currpwm=pid.pwmcycle;
    }
    if(pid.currpwm<0)
    {
     pid.currpwm=0;
    }
    
    printf("ʵ�����ʹ�õ�OUT:\t%d\r\n",(int)pid.currpwm) ;
    pid.En_2=pid.En_1;
    pid.En_1=pid.En;
    
    // Turn_Angle(pid.currpwm);                //���PWM

}
