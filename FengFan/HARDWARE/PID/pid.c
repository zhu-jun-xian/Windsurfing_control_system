#include "pid.h"
#include "usart.h"   //USART设置

PID pid;

void PIDParament_Init()  //
{
    pid.choose_model = MODEL_PID;
    pid.T=330;                //采样周期，定时器使用1ms，则最小执行PID的周期为330ms
    
    pid.TargerAngle =30;      //用户设定值
    pid.Kp=0.5;               //比例系数
    pid.Ti=40;                //微分系数常数
    pid.Td=10;                //积分时间常数
    pid.OUT0=0;               //一个维持的输出

    pid.pwmcycle = 330;       //PWM的周期
}

    
void pid_calc(float angle , u8 choose_model)   //pid??
{
		float dk1,dk2;        //本次偏差与上次偏差之差
		float pout,iout,dout;
    
		pid.Tdata = 888;
    if(pid.Tdata < (pid.T))  //最小计算周期未到
     {
            return ;
     }
    pid.Tdata = TIM_GetCounter(TIM4);
    pid.angle = angle;
    pid.En=pid.TargerAngle-pid.angle;  //本次误差
    dk1=pid.En-pid.En_1;   //本次偏差与上次偏差之差
    dk2=pid.En-2*pid.En_1+pid.En_2;
    
    pout=pid.Kp*dk1;                            //比例
    
    iout=(pid.Kp*pid.T)/pid.Ti;      //积分
    iout=iout*pid.En;
    
    dout=(pid.Kp*pid.Td)/pid.T;        //微分
    dout=dout*dk2;
    
    switch(pid.choose_model)
     {
         case MODEL_P:     pid.Dout= pout;            printf("使用P运算\r\n") ;
             break;
         
         case MODEL_PI:  pid.Dout= pout+iout;           printf("使用PI运算\r\n") ;
             break;
                 
         case MODEL_PID: pid.Dout= pout+iout+dout;        printf("使用PID运算\r\n") ;
             break;
     } 
          
    pid.currpwm+=pid.Dout;  //本次应该输出的PWM
    printf("PID算得的OUT:\t%d\r\n",(int)pid.currpwm) ;
     
    /*判断算出的数是否符合控制要求*/
    if(pid.currpwm>pid.pwmcycle)            //算出的值取值，肯定是在0-pid.pwmcycle之间，不然的话PWM怎么输出
    {
      pid.currpwm=pid.pwmcycle;
    }
    if(pid.currpwm<0)
    {
     pid.currpwm=0;
    }
    
    printf("实际输出使用的OUT:\t%d\r\n",(int)pid.currpwm) ;
    pid.En_2=pid.En_1;
    pid.En_1=pid.En;
    
    // Turn_Angle(pid.currpwm);                //输出PWM

}
