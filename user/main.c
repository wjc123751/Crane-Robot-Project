#include "all_head.h"

/*获取任务堆栈使用情况
#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t UserTaskStack;
#endif
#if INCLUDE_uxTaskGetStackHighWaterMark
		UserTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif*/

#define START_TASK_PRIO									1
#define START_STK_SIZE 									512
TaskHandle_t StartTask_Handler;

#define ChassisTask_PRIO								3
#define ChassisTask_STK_SIZE 						256
TaskHandle_t ChassisTask_Handler;

#define GimbalTask_PRIO									10
#define GimbalTask_STK_SIZE 						256
TaskHandle_t GimbalTask_Handler;

#define LeftAndRightTask_PRIO						9
#define LeftAndRightTask_STK_SIZE 			256
TaskHandle_t LeftAndRightTask_Handler;

#define UpAndDownTask_PRIO							8
#define UpAndDownTask_STK_SIZE 					256
TaskHandle_t UpAndDownTask_Handler;

#define GrabTask_PRIO										2
#define GrabTask_STK_SIZE 							256
TaskHandle_t GrabTask_Handler;

#define CommunicationTask_PRIO					11
#define CommunicationTask_STK_SIZE 			256
TaskHandle_t CommunicationTask_Handler;

#define MainTask_PRIO										12
#define MainTask_STK_SIZE 							256
TaskHandle_t MainTask_Handler;

// 函数声明
void BSP_init(void);
void Start_Task(void *pvParameters);
void ChassisTask(void *pvParameters);
void GimbalTask(void *pvParameters);
void LeftAndRightTask(void *pvParameters);
void UpAndDownTask(void *pvParameters);
void GrabTask(void *pvParameters);
void CommunicationTask(void *pvParameters);
void MainTask(void *pvParameters);

// 主函数
int main(void)
{
	  BSP_init();
	
    xTaskCreate((TaskFunction_t	)		Start_Task,          				// 指向任务函数的指针
                (const char*		)		"Start_Task",          			// 任务名称，最大configMAX_TASK_NAME_LEN
                (uint16_t				)		START_STK_SIZE,        			// 任务堆栈大小，以字为单位
                (void*					)		NULL,                  			// 传递给任务函数的参数
                (UBaseType_t		)		START_TASK_PRIO,       			// 任务优先级0到32-1
                (TaskHandle_t*	)		&StartTask_Handler);				// 任务句柄，任务的任务控制块
								
    vTaskStartScheduler();
								
    while (1)
    {
				
		}
}

// 初始化外设
void BSP_init(void)
{
		// 中断组 4
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		// 初始化滴答时钟
    delay_init(configTICK_RATE_HZ);
		// 初始化led
		led_configuration();
		// 初始化TIM2
		TIM2_Init(200, 9000);
		// 初始化蜂鸣器
		buzzer_init(30000, 90);
		// 初始化Uart7
		Init_Uart7(115200);
		// 初始化Uart8
		// uart8_init();
		// 初始化遥控器
		RC_Init();
		// 初始化CAN总线
		CAN1_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
    CAN2_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
}

// 开始任务函数实现
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();																					// 进入临界区（临界区内不会进行任务切换）
	
    xTaskCreate((TaskFunction_t )		ChassisTask,     							// 创建底盘任务
                (const char*    )		"ChassisTask",   	
                (uint16_t       )		ChassisTask_STK_SIZE, 
                (void*          )		NULL,				
                (UBaseType_t    )		ChassisTask_PRIO,	
                (TaskHandle_t*  )		&ChassisTask_Handler);
    
    xTaskCreate((TaskFunction_t )		GimbalTask,     							// 创建云台任务
                (const char*    )		"GimbalTask",   
                (uint16_t       )		GimbalTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		GimbalTask_PRIO,
                (TaskHandle_t*  )		&GimbalTask_Handler);
								
		xTaskCreate((TaskFunction_t )		LeftAndRightTask,     				// 创建左右机构任务
                (const char*    )		"LeftAndRightTask",   
                (uint16_t       )		LeftAndRightTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		LeftAndRightTask_PRIO,
                (TaskHandle_t*  )		&LeftAndRightTask_Handler);
								
		xTaskCreate((TaskFunction_t )		UpAndDownTask,     						// 创建上下机构任务
                (const char*    )		"UpAndDownTask",   
                (uint16_t       )		UpAndDownTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		UpAndDownTask_PRIO,
                (TaskHandle_t*  )		&UpAndDownTask_Handler);
								
		xTaskCreate((TaskFunction_t )		GrabTask,     								// 创建抓取任务
                (const char*    )		"GrabTask",   
                (uint16_t       )		GrabTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		GrabTask_PRIO,
                (TaskHandle_t*  )		&GrabTask_Handler);
								
		xTaskCreate((TaskFunction_t )		CommunicationTask,     				// 创建通讯任务
                (const char*    )		"CommunicationTask",   
                (uint16_t       )		CommunicationTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		CommunicationTask_PRIO,
                (TaskHandle_t*  )		&CommunicationTask_Handler);
								
		xTaskCreate((TaskFunction_t )		MainTask,     								// 创建主逻辑任务
                (const char*    )		"MainTask",   
                (uint16_t       )		MainTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		MainTask_PRIO,
                (TaskHandle_t*  )		&MainTask_Handler);
								
		vTaskDelete(StartTask_Handler); 															// 删除开始任务
								
    taskEXIT_CRITICAL();																					// 退出临界区
}
