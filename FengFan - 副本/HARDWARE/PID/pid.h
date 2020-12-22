#ifndef PIBOT_PID_H_
#define PIBOT_PID_H_

//typedef struct PID_INIT{
//int KP;
//float KI;
//int KD;
//	int e0,e1,e2;
//}pid_struct;
//int Position_PID (float angle, float TargetaAngle,pid_struct mypid);


/*增量式PID算法，接口参数结构类型*/
typedef struct
{
	/*PID算法接口变量，用于给用户获取或修改PID算法的特性*/
	 float kp;     //比例系数
	 float ki;     //积分系数
	 float kd;     //微分系数
	
	float errNow;  //当前的误差
	float dOut;		//控制增量输出
	float Out;		//控制输出
	
	float errd1;  //前一时刻的误差
	float errd2;
	
	float errILim;//误差积分上限
	 float errOld;
	 float errP;
	 float errI;
	 float errD;
}PID_IncrementType;

extern long PID_Control(s32 SpeedTag);
extern long PidSpeedControl(s32 SpeedTag);
	
#endif
