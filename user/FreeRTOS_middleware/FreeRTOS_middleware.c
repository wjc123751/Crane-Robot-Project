/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       freeRTOS_middle.c/h
  * @brief      freeRTOS的中间层，将滴答计时中断和统计任务用时的接口函数放到这里.
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#include "FreeRTOS_Middleware.h"
#include "stm32f4xx.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "timer.h"

// 设置调度中断定时器配置
void vPortSetupTimerInterrupt(void)
{
    // 此函数应该用于配置调度中断定时器的设置
    // 这里没有提供具体的配置内容
}

// 外部函数声明，用于处理系统滴答中断
extern void xPortSysTickHandler(void);

// SysTick 中断处理函数
void SysTick_Handler(void)
{
    // 检查任务调度器的状态，确保已经启动
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        // 调用外部函数 xPortSysTickHandler 处理 SysTick 中断
        xPortSysTickHandler();
    }
}


//利用tim3作为任务统计用时，所有任务目前测试cpu利用率
volatile uint64_t FreeRTOSRunTimeTicks = 0;

void ConfigureTimeForRunTimeStats(void)
{
    FreeRTOSRunTimeTicks = 0;
		TIM3_Init(5000, 9000);
}

int time_flag = 0;
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearFlag(TIM3, TIM_IT_Update);
				if(time_flag == 1)
				{
						FreeRTOSRunTimeTicks++;
				}
    }
}
/*
void TimerInterruptHandler(void)
{
    pulseCount++;
    if (pulseCount >= NUM_PULSES_TO_GENERATE)
    {
        // 停止定时器或采取其他操作
    }
}
*/
 
