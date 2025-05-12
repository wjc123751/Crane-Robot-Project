#include "all_head.h"

/*��ȡ�����ջʹ�����
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

// ��������
void BSP_init(void);
void Start_Task(void *pvParameters);
void ChassisTask(void *pvParameters);
void GimbalTask(void *pvParameters);
void LeftAndRightTask(void *pvParameters);
void UpAndDownTask(void *pvParameters);
void GrabTask(void *pvParameters);
void CommunicationTask(void *pvParameters);
void MainTask(void *pvParameters);

// ������
int main(void)
{
	  BSP_init();
	
    xTaskCreate((TaskFunction_t	)		Start_Task,          				// ָ����������ָ��
                (const char*		)		"Start_Task",          			// �������ƣ����configMAX_TASK_NAME_LEN
                (uint16_t				)		START_STK_SIZE,        			// �����ջ��С������Ϊ��λ
                (void*					)		NULL,                  			// ���ݸ��������Ĳ���
                (UBaseType_t		)		START_TASK_PRIO,       			// �������ȼ�0��32-1
                (TaskHandle_t*	)		&StartTask_Handler);				// �������������������ƿ�
								
    vTaskStartScheduler();
								
    while (1)
    {
				
		}
}

// ��ʼ������
void BSP_init(void)
{
		// �ж��� 4
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		// ��ʼ���δ�ʱ��
    delay_init(configTICK_RATE_HZ);
		// ��ʼ��led
		led_configuration();
		// ��ʼ��TIM2
		TIM2_Init(200, 9000);
		// ��ʼ��������
		buzzer_init(30000, 90);
		// ��ʼ��Uart7
		Init_Uart7(115200);
		// ��ʼ��Uart8
		// uart8_init();
		// ��ʼ��ң����
		RC_Init();
		// ��ʼ��CAN����
		CAN1_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
    CAN2_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
}

// ��ʼ������ʵ��
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();																					// �����ٽ������ٽ����ڲ�����������л���
	
    xTaskCreate((TaskFunction_t )		ChassisTask,     							// ������������
                (const char*    )		"ChassisTask",   	
                (uint16_t       )		ChassisTask_STK_SIZE, 
                (void*          )		NULL,				
                (UBaseType_t    )		ChassisTask_PRIO,	
                (TaskHandle_t*  )		&ChassisTask_Handler);
    
    xTaskCreate((TaskFunction_t )		GimbalTask,     							// ������̨����
                (const char*    )		"GimbalTask",   
                (uint16_t       )		GimbalTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		GimbalTask_PRIO,
                (TaskHandle_t*  )		&GimbalTask_Handler);
								
		xTaskCreate((TaskFunction_t )		LeftAndRightTask,     				// �������һ�������
                (const char*    )		"LeftAndRightTask",   
                (uint16_t       )		LeftAndRightTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		LeftAndRightTask_PRIO,
                (TaskHandle_t*  )		&LeftAndRightTask_Handler);
								
		xTaskCreate((TaskFunction_t )		UpAndDownTask,     						// �������»�������
                (const char*    )		"UpAndDownTask",   
                (uint16_t       )		UpAndDownTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		UpAndDownTask_PRIO,
                (TaskHandle_t*  )		&UpAndDownTask_Handler);
								
		xTaskCreate((TaskFunction_t )		GrabTask,     								// ����ץȡ����
                (const char*    )		"GrabTask",   
                (uint16_t       )		GrabTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		GrabTask_PRIO,
                (TaskHandle_t*  )		&GrabTask_Handler);
								
		xTaskCreate((TaskFunction_t )		CommunicationTask,     				// ����ͨѶ����
                (const char*    )		"CommunicationTask",   
                (uint16_t       )		CommunicationTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		CommunicationTask_PRIO,
                (TaskHandle_t*  )		&CommunicationTask_Handler);
								
		xTaskCreate((TaskFunction_t )		MainTask,     								// �������߼�����
                (const char*    )		"MainTask",   
                (uint16_t       )		MainTask_STK_SIZE, 
                (void*          )		NULL,
                (UBaseType_t    )		MainTask_PRIO,
                (TaskHandle_t*  )		&MainTask_Handler);
								
		vTaskDelete(StartTask_Handler); 															// ɾ����ʼ����
								
    taskEXIT_CRITICAL();																					// �˳��ٽ���
}
