#include "pid.h"
#include "usart.h"
/**************************************************************************
函数功能：位置式PID控制器
1.入口参数：编码器测量位置信息，目标位置
2.入口参数：编码器测量速度 , 目标速度
返回 值：电机PWM
根据位置式离散PID公式
PWM=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
PWM代表输出
**************************************************************************/


extern pid_struct mypid;
int Position_PID (float angle, float TargetaAngle,pid_struct mypid)
{
	int PWM;
    mypid.e2 = TargetaAngle -angle; //计算偏差
    PWM = mypid.KP * (mypid.e2-mypid.e1)+mypid.KI*mypid.e2+mypid.KD*(mypid.e2+mypid.e0-2*mypid.e1);
    mypid.e0=mypid.e1;
	mypid.e1=mypid.e2;
  
    return PWM; //增量输出
}



