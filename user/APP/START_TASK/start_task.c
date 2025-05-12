#include "start_task.h"

//// �궨��
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


//// ��ʼ������ʵ��
//void start_task(void *pvParameters)
//{
//    taskENTER_CRITICAL();																					// �����ٽ������ٽ����ڲ�����������л���
//	
//    xTaskCreate((TaskFunction_t )		ChassisTask,     							// ����TASK1����
//                (const char*    )		"ChassisTask",   	
//                (uint16_t       )		ChassisTask_STK_SIZE, 
//                (void*          )		NULL,				
//                (UBaseType_t    )		ChassisTask_PRIO,	
//                (TaskHandle_t*  )		&ChassisTask_Handler);
//    
//    xTaskCreate((TaskFunction_t )		GimbalTask,     							// ����TASK2����
//                (const char*    )		"GimbalTask",   
//                (uint16_t       )		GimbalTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		GimbalTask_PRIO,
//                (TaskHandle_t*  )		&GimbalTask_Handler);
//								
//		xTaskCreate((TaskFunction_t )		LeftAndRightTask,     							// ����TASK3����
//                (const char*    )		"LeftAndRightTask",   
//                (uint16_t       )		LeftAndRightTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		LeftAndRightTask_PRIO,
//                (TaskHandle_t*  )		&LeftAndRightTask_Handler);
//								
//		xTaskCreate((TaskFunction_t )		UpAndDownTask,     							// ����TASK4����
//                (const char*    )		"UpAndDownTask",   
//                (uint16_t       )		UpAndDownTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		UpAndDownTask_PRIO,
//                (TaskHandle_t*  )		&UpAndDownTask_Handler);
//								
//		xTaskCreate((TaskFunction_t )		GrabTask,     							// ����TASK5����
//                (const char*    )		"GrabTask",   
//                (uint16_t       )		GrabTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		GrabTask_PRIO,
//                (TaskHandle_t*  )		&GrabTask_Handler);
//								
//		xTaskCreate((TaskFunction_t )		CommunicationTask,     							// ����TASK3����
//                (const char*    )		"CommunicationTask",   
//                (uint16_t       )		CommunicationTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		CommunicationTask_PRIO,
//                (TaskHandle_t*  )		&CommunicationTask_Handler);

//								
//		xTaskCreate((TaskFunction_t )		MainTask,     							// ����TASK3����
//                (const char*    )		"MainTask",   
//                (uint16_t       )		MainTask_STK_SIZE, 
//                (void*          )		NULL,
//                (UBaseType_t    )		MainTask_PRIO,
//                (TaskHandle_t*  )		&MainTask_Handler);
//								
//		vTaskDelete(StartTask_Handler); 												//ɾ����ʼ����
//								
//    taskEXIT_CRITICAL();																		// �˳��ٽ���
//}

//// ������ʼ������
//void StartTask(void)
//{
//    xTaskCreate((TaskFunction_t	)		start_task,          		// ָ����������ָ��
//                (const char*		)		"start_task",          	// �������ƣ����configMAX_TASK_NAME_LEN
//                (uint16_t				)		START_STK_SIZE,        	// �����ջ��С������Ϊ��λ
//                (void*					)		NULL,                  	// ���ݸ��������Ĳ���
//                (UBaseType_t		)		START_TASK_PRIO,       	// �������ȼ�0��32-1
//                (TaskHandle_t*	)		&StartTask_Handler);		// �������������������ƿ�
//}
