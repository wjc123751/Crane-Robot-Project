#include "start_task.h"

//// 宏定义
//#define START_TASK_PRIO									1
//#define START_STK_SIZE 									512
//TaskHandle_t StartTask_Handler;

//#define ChassisTask_PRIO								3
//#define ChassisTask_STK_SIZE 						256
//TaskHandle_t ChassisTask_Handler;

//#define GimbalTask_PRIO									10
//#define GimbalTask_STK_SIZE 						256
//TaskHandle_t GimbalTask_Handler;

//#define LeftAndRightTask_PRIO						9
//#define LeftAndRightTask_STK_SIZE 			256
//TaskHandle_t LeftAndRightTask_Handler;

//#define UpAndDownTask_PRIO							8
//#define UpAndDownTask_STK_SIZE 					256
//TaskHandle_t UpAndDownTask_Handler;

//#define GrabTask_PRIO										2
//#define GrabTask_STK_SIZE 							256
//TaskHandle_t GrabTask_Handler;

//#define CommunicationTask_PRIO					11
//#define CommunicationTask_STK_SIZE 			256
//TaskHandle_t CommunicationTask_Handler;

//#define MainTask_PRIO										12
//#define MainTask_STK_SIZE 							256
//TaskHandle_t MainTask_Handler;


//// 开始任务函数实现
//void start_task(void *pvParameters)
//{
//    taskENTER_CRITICAL();																					// 进入临界区（临界区内不会进行任务切换）
//	
//    xTaskCreate((TaskFunction_t )		ChassisTask,     							// 创建TASK1任务
//                (const char*    )		"ChassisTask",   	
//                (uint16_t       )		ChassisTask_STK_SIZE, 
//                (void*          )		NULL,				
//                (UBaseType_t    )		ChassisTask_PRIO,	
//                (TaskHandle_t*  )		&ChassisTask_Handler);
//    
//    xTaskCreate((TaskFunction_t )		GimbalTask,     							// 创建TASK2任务
//                (const char*    )		"GimbalTask",   
//                (uint16_t       )		GimbalTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		GimbalTask_PRIO,
//                (TaskHandle_t*  )		&GimbalTask_Handler);
//								
//		xTaskCreate((TaskFunction_t )		LeftAndRightTask,     							// 创建TASK3任务
//                (const char*    )		"LeftAndRightTask",   
//                (uint16_t       )		LeftAndRightTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		LeftAndRightTask_PRIO,
//                (TaskHandle_t*  )		&LeftAndRightTask_Handler);
//								
//		xTaskCreate((TaskFunction_t )		UpAndDownTask,     							// 创建TASK4任务
//                (const char*    )		"UpAndDownTask",   
//                (uint16_t       )		UpAndDownTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		UpAndDownTask_PRIO,
//                (TaskHandle_t*  )		&UpAndDownTask_Handler);
//								
//		xTaskCreate((TaskFunction_t )		GrabTask,     							// 创建TASK5任务
//                (const char*    )		"GrabTask",   
//                (uint16_t       )		GrabTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		GrabTask_PRIO,
//                (TaskHandle_t*  )		&GrabTask_Handler);
//								
//		xTaskCreate((TaskFunction_t )		CommunicationTask,     							// 创建TASK3任务
//                (const char*    )		"CommunicationTask",   
//                (uint16_t       )		CommunicationTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		CommunicationTask_PRIO,
//                (TaskHandle_t*  )		&CommunicationTask_Handler);

//								
//		xTaskCreate((TaskFunction_t )		MainTask,     							// 创建TASK3任务
//                (const char*    )		"MainTask",   
//                (uint16_t       )		MainTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		MainTask_PRIO,
//                (TaskHandle_t*  )		&MainTask_Handler);
//								
//		vTaskDelete(StartTask_Handler); 												//删除开始任务
//								
//    taskEXIT_CRITICAL();																		// 退出临界区
//}

//// 创建开始任务函数
//void StartTask(void)
//{
//    xTaskCreate((TaskFunction_t	)		start_task,          		// 指向任务函数的指针
//                (const char*		)		"start_task",          	// 任务名称，最大configMAX_TASK_NAME_LEN
//                (uint16_t				)		START_STK_SIZE,        	// 任务堆栈大小，以字为单位
//                (void*					)		NULL,                  	// 传递给任务函数的参数
//                (UBaseType_t		)		START_TASK_PRIO,       	// 任务优先级0到32-1
//                (TaskHandle_t*	)		&StartTask_Handler);		// 任务句柄，任务的任务控制块
//}
