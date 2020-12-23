#include "stm32f10x.h"
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


//extern pid_struct mypid;
//int Position_PID (float angle, float TargetaAngle,pid_struct mypid)
//{
//	int PWM;
////    mypid.e2 = TargetaAngle -angle; //计算偏差
////    PWM = mypid.KP * (mypid.e2-mypid.e1)+mypid.KI*mypid.e2+mypid.KD*(mypid.e2+mypid.e0-2*mypid.e1);
////    mypid.e0=mypid.e1;
////	mypid.e1=mypid.e2;
//	float bias;
//	static float L_bias,D_bias;
//	
//    return PWM; //增量输出
//}
extern float angle;
extern int ZHONGZHI;
extern unsigned long pwmpulse;
extern u16 adcx;
extern float error;
PID_IncrementType PID;//定义PID算法的结构体

static void PID_IncrementMode()
{
//	PID_IncrementType PID;//定义PID算法的结构体
	float derrP , derrI , derrD;
	if(PID.kp < 0)		PID.kp =  -PID.kp;
	if(PID.ki < 0)		PID.ki =  -PID.ki;
	if(PID.kd < 0)		PID.kd =  -PID.kd;
	
	derrP = PID.errNow - PID.errd1;
	derrI = PID.errNow;
	derrD = PID.errNow - 2*PID.errd1 + PID.errd2;
	
	PID.errd2 = PID.errd1; //二阶误差微分
	PID.errd1 = PID.errNow; //一阶误差微分
	
	PID.dOut = (PID.kp * derrP) + (PID.ki * derrI) + (PID.kd * derrD);
	if (PID.kp == 0 && PID.ki == 0 && PID.kd ==0)		PID.Out = 0;
	else  PID.Out += PID.dOut;
}

s32 spdNow,spdTag;
float control;
long PID_Control (s32 SpeedTag)
{
//	PID_IncrementType PID;//定义PID算法的结构体
	spdNow = adcx;
	spdTag = SpeedTag;
	error=spdNow-spdTag ;
	PID.errNow = spdNow-spdTag ;//计算并写入速度误差
	 PID.kp      = 21;             //写入比例系数
   PID.ki      = 0.35;              //写入积分系数
   PID.kd      = 2;              //写入微分系数
	
	PID_IncrementMode();
	control = PID.Out;
	return control;
//	pwmpulse = control;
}
////////////////////////////////位置式//////////////////////////////////////
void PID_AbsoluteMode()
{
 if(PID.kp      < 0)    PID.kp      = -PID.kp;
 if(PID.ki      < 0)    PID.ki      = -PID.ki;
 if(PID.kd      < 0)    PID.kd      = -PID.kd;
 if(PID.errILim < 0)    PID.errILim = -PID.errILim;

 PID.errP = PID.errNow;  //读取现在的误差，用于kp控制

 PID.errI += PID.errNow; //误差积分，用于ki控制

 if(PID.errILim != 0)	   //微分上限和下限
 {
  if(     PID.errI >  PID.errILim)    PID.errI =  PID.errILim;
  else if(PID.errI < -PID.errILim)    PID.errI = -PID.errILim;
 }
 
 PID.errD = PID.errNow - PID.errOld;//误差微分，用于kd控制

 PID.errOld = PID.errNow;	//保存现在的误差
 
 PID.Out = PID.kp * PID.errP + PID.ki * PID.errI + PID.kd * PID.errD;//计算绝对式PID输出

}

s32 spdNow2,spdTag2;
float control2;
long PidSpeedControl(s32 SpeedTag)
{
   spdNow2 = adcx;
	 spdTag2 = SpeedTag;
	
   PID.errNow = spdTag2 - spdNow2; //计算并写入速度误差
   	
   PID.kp      = 20;             //写入比例系数为15
   PID.ki      = 1;              //写入积分系数为5
   PID.kd      = 500;              //写入微分系数为5
   PID.errILim = 20000;           //写入误差积分上限为1000 下限为-1000

   PID_AbsoluteMode();       //执行绝对式PID算法
	
   control2 = PID.Out;         //读取控制值
	 return control2;

}
