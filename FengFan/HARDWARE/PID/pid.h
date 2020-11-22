#ifndef PIBOT_PID_H_
#define PIBOT_PID_H_
typedef struct PID_INIT{
int KP;
float KI;
int KD;
	int e0,e1,e2;
}pid_struct;
int Position_PID (float angle, float TargetaAngle,pid_struct mypid);
#endif
