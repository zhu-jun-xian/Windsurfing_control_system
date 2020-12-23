#include "stm32f10x.h"
#include "pid.h"
#include "usart.h"
/**************************************************************************
�������ܣ�λ��ʽPID������
1.��ڲ���������������λ����Ϣ��Ŀ��λ��
2.��ڲ����������������ٶ� , Ŀ���ٶ�
���� ֵ�����PWM
����λ��ʽ��ɢPID��ʽ
PWM=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ��
e(k-1)������һ�ε�ƫ��
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
PWM�������
**************************************************************************/


//extern pid_struct mypid;
//int Position_PID (float angle, float TargetaAngle,pid_struct mypid)
//{
//	int PWM;
////    mypid.e2 = TargetaAngle -angle; //����ƫ��
////    PWM = mypid.KP * (mypid.e2-mypid.e1)+mypid.KI*mypid.e2+mypid.KD*(mypid.e2+mypid.e0-2*mypid.e1);
////    mypid.e0=mypid.e1;
////	mypid.e1=mypid.e2;
//	float bias;
//	static float L_bias,D_bias;
//	
//    return PWM; //�������
//}
extern float angle;
extern int ZHONGZHI;
extern unsigned long pwmpulse;
extern u16 adcx;
extern float error;
PID_IncrementType PID;//����PID�㷨�Ľṹ��

static void PID_IncrementMode()
{
//	PID_IncrementType PID;//����PID�㷨�Ľṹ��
	float derrP , derrI , derrD;
	if(PID.kp < 0)		PID.kp =  -PID.kp;
	if(PID.ki < 0)		PID.ki =  -PID.ki;
	if(PID.kd < 0)		PID.kd =  -PID.kd;
	
	derrP = PID.errNow - PID.errd1;
	derrI = PID.errNow;
	derrD = PID.errNow - 2*PID.errd1 + PID.errd2;
	
	PID.errd2 = PID.errd1; //�������΢��
	PID.errd1 = PID.errNow; //һ�����΢��
	
	PID.dOut = (PID.kp * derrP) + (PID.ki * derrI) + (PID.kd * derrD);
	if (PID.kp == 0 && PID.ki == 0 && PID.kd ==0)		PID.Out = 0;
	else  PID.Out += PID.dOut;
}

s32 spdNow,spdTag;
float control;
long PID_Control (s32 SpeedTag)
{
//	PID_IncrementType PID;//����PID�㷨�Ľṹ��
	spdNow = adcx;
	spdTag = SpeedTag;
	error=spdNow-spdTag ;
	PID.errNow = spdNow-spdTag ;//���㲢д���ٶ����
	 PID.kp      = 21;             //д�����ϵ��
   PID.ki      = 0.35;              //д�����ϵ��
   PID.kd      = 2;              //д��΢��ϵ��
	
	PID_IncrementMode();
	control = PID.Out;
	return control;
//	pwmpulse = control;
}
////////////////////////////////λ��ʽ//////////////////////////////////////
void PID_AbsoluteMode()
{
 if(PID.kp      < 0)    PID.kp      = -PID.kp;
 if(PID.ki      < 0)    PID.ki      = -PID.ki;
 if(PID.kd      < 0)    PID.kd      = -PID.kd;
 if(PID.errILim < 0)    PID.errILim = -PID.errILim;

 PID.errP = PID.errNow;  //��ȡ���ڵ�������kp����

 PID.errI += PID.errNow; //�����֣�����ki����

 if(PID.errILim != 0)	   //΢�����޺�����
 {
  if(     PID.errI >  PID.errILim)    PID.errI =  PID.errILim;
  else if(PID.errI < -PID.errILim)    PID.errI = -PID.errILim;
 }
 
 PID.errD = PID.errNow - PID.errOld;//���΢�֣�����kd����

 PID.errOld = PID.errNow;	//�������ڵ����
 
 PID.Out = PID.kp * PID.errP + PID.ki * PID.errI + PID.kd * PID.errD;//�������ʽPID���

}

s32 spdNow2,spdTag2;
float control2;
long PidSpeedControl(s32 SpeedTag)
{
   spdNow2 = adcx;
	 spdTag2 = SpeedTag;
	
   PID.errNow = spdTag2 - spdNow2; //���㲢д���ٶ����
   	
   PID.kp      = 20;             //д�����ϵ��Ϊ15
   PID.ki      = 1;              //д�����ϵ��Ϊ5
   PID.kd      = 500;              //д��΢��ϵ��Ϊ5
   PID.errILim = 20000;           //д������������Ϊ1000 ����Ϊ-1000

   PID_AbsoluteMode();       //ִ�о���ʽPID�㷨
	
   control2 = PID.Out;         //��ȡ����ֵ
	 return control2;

}
