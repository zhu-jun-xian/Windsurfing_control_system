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


extern pid_struct mypid;
int Position_PID (float angle, float TargetaAngle,pid_struct mypid)
{
	int PWM;
    mypid.e2 = TargetaAngle -angle; //����ƫ��
    PWM = mypid.KP * (mypid.e2-mypid.e1)+mypid.KI*mypid.e2+mypid.KD*(mypid.e2+mypid.e0-2*mypid.e1);
    mypid.e0=mypid.e1;
	mypid.e1=mypid.e2;
  
    return PWM; //�������
}



