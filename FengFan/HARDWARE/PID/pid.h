#ifndef _pid_
#define _pid_
#include "stm32f10x_conf.h"
#define     MODEL_P       1
#define     MODEL_PI      2
#define     MODEL_PID     3

//增量式PID的pid.h
typedef struct
{
    u8 choose_model;    //使用哪个模式调节
    
    float angle;              //当前值
    float TargerAngle;             //设定值
    

    float En;                  //当前时刻
    float En_1;                //前一时刻
    float En_2;                //前二时刻
        
    float Kp;               //比例系数
    float T;                //采样周期---控制周期，每隔T控制器输出一次PID运算结果
    u16   Tdata;            //判断PID周期到没到
    float Ti;               //积分时间常数
    float Td;               //微分时间常数
    
    float Dout;                //增量PID计算本次应该输出的增量值--本次计算的结果
    float OUT0;                //一个维持的输出，防止失控
    
    short currpwm;      //当前的pwm宽度
    u16 pwmcycle;       //pwm周期

}PID;


extern u8 STATUS;
extern PID pid;
void PIDParament_Init(void);          /*增量式PID初始化*/
void pid_calc(float angle , u8 choose_model);                  /*pid计算 并输出*/
#endif
