#ifndef _pid_
#define _pid_
#include "stm32f10x_conf.h"
#define     MODEL_P       1
#define     MODEL_PI      2
#define     MODEL_PID     3

//����ʽPID��pid.h
typedef struct
{
    u8 choose_model;    //ʹ���ĸ�ģʽ����
    
    float angle;              //��ǰֵ
    float TargerAngle;             //�趨ֵ
    

    float En;                  //��ǰʱ��
    float En_1;                //ǰһʱ��
    float En_2;                //ǰ��ʱ��
        
    float Kp;               //����ϵ��
    float T;                //��������---�������ڣ�ÿ��T���������һ��PID������
    u16   Tdata;            //�ж�PID���ڵ�û��
    float Ti;               //����ʱ�䳣��
    float Td;               //΢��ʱ�䳣��
    
    float Dout;                //����PID���㱾��Ӧ�����������ֵ--���μ���Ľ��
    float OUT0;                //һ��ά�ֵ��������ֹʧ��
    
    short currpwm;      //��ǰ��pwm���
    u16 pwmcycle;       //pwm����

}PID;


extern u8 STATUS;
extern PID pid;
void PIDParament_Init(void);          /*����ʽPID��ʼ��*/
void pid_calc(float angle , u8 choose_model);                  /*pid���� �����*/
#endif
