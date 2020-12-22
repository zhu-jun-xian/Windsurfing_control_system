#ifndef PIBOT_PID_H_
#define PIBOT_PID_H_

//typedef struct PID_INIT{
//int KP;
//float KI;
//int KD;
//	int e0,e1,e2;
//}pid_struct;
//int Position_PID (float angle, float TargetaAngle,pid_struct mypid);


/*����ʽPID�㷨���ӿڲ����ṹ����*/
typedef struct
{
	/*PID�㷨�ӿڱ��������ڸ��û���ȡ���޸�PID�㷨������*/
	 float kp;     //����ϵ��
	 float ki;     //����ϵ��
	 float kd;     //΢��ϵ��
	
	float errNow;  //��ǰ�����
	float dOut;		//�����������
	float Out;		//�������
	
	float errd1;  //ǰһʱ�̵����
	float errd2;
	
	float errILim;//����������
	 float errOld;
	 float errP;
	 float errI;
	 float errD;
}PID_IncrementType;

extern long PID_Control(s32 SpeedTag);
extern long PidSpeedControl(s32 SpeedTag);
	
#endif
