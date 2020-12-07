#include "timer.h"
#include "stdio.h"
#include "usart.h"
#include "adc.h"
#include "pid.h"
extern int ZHONGZHI;
extern char oledBuf[20];
extern float angle;;
extern float voltage;
extern float adcx;
extern int pwmpulse;
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //打开相应的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


    //清除TIM3的中断待处理位:TIM 中断源
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);


    //设置定时器TIM_TimeBaseInit初始化定时器的时钟基数
    TIM_TimeBaseStructure.TIM_Period = 11000-1;
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);


    //使能指定的TIM中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );


    //使能TIM定时器
    TIM_Cmd(TIM3, ENABLE);


    //设置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void TIM3_IRQHandler(void)   //TIM3中断
{
    //判断是否发生中断
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
				sprintf(oledBuf, "Error:%02d", (int)angle);	
        OLED_ShowString(0, 0, (u8*)oledBuf, 16);
				sprintf(oledBuf, "Angle:%02d", (int)angle);
        OLED_ShowString(0, 16, (u8*)oledBuf, 16);
        sprintf(oledBuf, "PWM:%d", pwmpulse);
        OLED_ShowString(0, 32, (u8*)oledBuf, 16);
        sprintf(oledBuf, "Tangle:%02d",ZHONGZHI);
        OLED_ShowString(0, 48, (u8*)oledBuf, 16);
        OLED_Refresh();
			
//       pwmpulse = Position_PID(angle, 40);
//			 TIM_SetCompare3(TIM4, pwmpulse);	  //TIM4->CCR2=800 limit:0~800
//清除EXTI线路挂起位
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}
void TIM2_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(  //使能或者失能指定的TIM中断
        TIM2, //TIM2
        TIM_IT_Update,
        ENABLE  //使能
    );
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设

}

void TIM2_IRQHandler(void)   //TIM2中断
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
				sprintf(oledBuf, "Error:%02d", (int)angle);	
        OLED_ShowString(0, 0, (u8*)oledBuf, 16);
				sprintf(oledBuf, "Angle:%02d", (int)angle);
        OLED_ShowString(0, 16, (u8*)oledBuf, 16);
        sprintf(oledBuf, "PWM:%d", pwmpulse);
        OLED_ShowString(0, 32, (u8*)oledBuf, 16);
        sprintf(oledBuf, "Tangle:%02d",ZHONGZHI);
        OLED_ShowString(0, 48, (u8*)oledBuf, 16);
        OLED_Refresh();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源

    }
}










