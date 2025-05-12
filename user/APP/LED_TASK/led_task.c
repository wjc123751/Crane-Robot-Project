//// ͷ�ļ�
//#include "FreeRTOSConfig.h"
//#include "FreeRTOS.h"
//#include "task.h"
#include "led_task.h"
//#include "led.h"
//#include "rc.h"
//#include "can_receive.h"
//#include "usart7.h"
//#include "pid.h"
//#include "FreeRTOS_Middleware.h"
//#include <string.h>
//#include "buzzer.h"
//	
///*
//ռ�ձ� = t / T ��ز������£�
//t = 0.5ms������������-�����ת�� 0 ��
//t = 1.0ms������������-�����ת�� 45��
//t = 1.5ms������������-�����ת�� 90��
//t = 2.0ms������������-�����ת�� 135��
//t = 2.5ms������������-�����ת�� 180��
//*/
////				TIM_SetCompare1(TIM2, 180);
////				TIM_SetCompare2(TIM2, 185);
////				TIM_SetCompare3(TIM2, 190);
////				TIM_SetCompare4(TIM2, 195);
////				CAN_CMD_motor_2006(MOTOR_2006_Id1_4, a, a, a, a);
////				CAN_CMD_motor_3508(MOTOR_3508_Id5_8, a, a, 0, 0);
////				CAN_CMD_motor_6020(MOTOR_6020_Id5_7, a, a, a);
////				GPIO_SetBits(GPIOF, GPIO_Pin_0);
////				GPIO_SetBits(GPIOF, GPIO_Pin_1);
////				GPIO_SetBits(GPIOE, GPIO_Pin_4);
////				GPIO_SetBits(GPIOE, GPIO_Pin_5);

//// �궨����λ��
//#define Servo1_Suction 										185
//#define Servo1_Placement 									195
//#define Servo3_Suction 										186
//#define Servo3_Left_Suction 							178
//#define Servo3_Right_Suction 							194
//#define Servo3_Placement 									180

//// �궨�����λ��
//#define White_Rack_Suction_One_Layer 			24500
//#define Black_Rack_Suction_One_Layer 			24500
//#define White_Rack_Suction_Box 						8000
//#define White_Rack_Suction_Coca_Cola 			7800
//#define Black_Rack_Suction_Box 						7600

//// �궨�������λλ��
//#define Gear_Limit_Position 							-3500

//// �궨�����ٶ�
//#define Chassis_Stop 											0
//#define Chassis_Low_Speed 								500
//#define Chassis_High_Speed 								2000

//// ����ȫ�ֱ���-ң�����ṹ��
//RC_Ctl_t rc;

//// �ⲿ�����������
//extern motor_measure motor_2006[4];
//extern motor_measure motor_3508[4];
//extern motor_measure motor_6020[3];

//// ���ÿ��ر�־λ
//int Relay_flag1 = 0;
//int Relay_flag2 = 0;
//int Relay_flag3 = 0;
//int Relay_flag4 = 0;

//// �ⲿ������������
//extern u8 USART_RX_BUF[200];

//// �ⲿ�������������С
//extern u16 USART_RX_STA;

//// �ⲿ������ʱ��
//extern volatile uint64_t FreeRTOSRunTimeTicks;
//extern int time_flag;

///*����*/
///*6020*/
//// ����PID�ṹ��
//PidTypeDef Angular_Velocity_6020;
//PidTypeDef Angular_6020;
//// �ٶȻ�PID����
//fp32 PID_Angular_Velocity_6020[3] = {160.f, 2.f, 7.f};
//// �ǶȻ�PID����
//fp32 PID_Angular_6020[3] = {0.00801f, 0.f, 0.00079f};
//// PID������
//int16_t Angular_Velocity_Current_6020;
//int16_t Angular_Result_6020;

///*�׳���*/
///*3508*/
//// ����PID�ṹ��
//PidTypeDef Angular_Velocity_3508_White;
//PidTypeDef Angular_3508_White;
//// �ٶȻ�PID����
//fp32 PID_Angular_Velocity_3508_White[3] = {15.f, 0.f, 0.f};
//// �ǶȻ�PID����
//fp32 PID_Angular_3508_White[3] = {3.f, 0.f, 0.5f};
//// PID������
//int16_t Angular_Velocity_Current_3508_White;
//int16_t Angular_Result_3508_White;
///*2006*/
//// ����PID�ṹ��
//PidTypeDef Angular_Velocity_2006_White;
//PidTypeDef Angular_2006_White;
//// �ٶȻ�PID����
//fp32 PID_Angular_Velocity_2006_White[3] = {11.f, 0.f, 1.f};
//// �ǶȻ�PID����
//fp32 PID_Angular_2006_White[3] = {3.f, 0.f, 1.f};
//// PID������
//int16_t Angular_Velocity_Current_2006_White;
//int16_t Angular_Result_2006_White;

///*�ڳ���*/
///*3508*/
//// ����PID�ṹ��
//PidTypeDef Angular_Velocity_3508_Black;
//PidTypeDef Angular_3508_Black;
//// �ٶȻ�PID����
//fp32 PID_Angular_Velocity_3508_Black[3] = {15.f, 0.f, 0.f};
//// �ǶȻ�PID����
//fp32 PID_Angular_3508_Black[3] = {3.f, 0.f, 5.f};
//// PID������
//int16_t Angular_Velocity_Current_3508_Black;
//int16_t Angular_Result_3508_Black;
///*2006*/
//// ����PID�ṹ��
//PidTypeDef Angular_Velocity_2006_Black;
//PidTypeDef Angular_2006_Black;
//// �ٶȻ�PID����
//fp32 PID_Angular_Velocity_2006_Black[3] = {25.f, 0.f, 1.f};
//// �ǶȻ�PID����
//fp32 PID_Angular_2006_Black[3] = {3.f, 0.f, 1.f};
//// PID������
//int16_t Angular_Velocity_Current_2006_Black;
//int16_t Angular_Result_2006_Black;

///*3508����*/
//// ����PID�ṹ��
//PidTypeDef Angular_Velocity_3508_Chassis;
//PidTypeDef Angular_3508_Chassis;
//// �ٶȻ�PID����
//fp32 PID_Angular_Velocity_3508_Chassis[3] = {60.f, 0.0001f, 29.9999f};
//// �ǶȻ�PID����
//fp32 PID_Angular_3508_Chassis[3] = {3.f, 0.f, 0.001f};
//// PID������
//int16_t Angular_Velocity_Current_3508_Chassis;
//int16_t Angular_Result_3508_Chassis;

//// �������ֵ
//int16_t Target_6020 = 0;
//int16_t Target_3508_White = 0;
//int16_t Target_2006_White = 0;
//int16_t Target_3508_Black = 0;
//int16_t Target_2006_Black = 0;
//int32_t Target_3508_Chassis = 0;
//// �������ֵ
//int16_t Target_Servo1 = 0;
//int16_t Target_Servo3 = 0;
//// ��������ֵ
//int16_t Target_Air_Pump = 0;

//// �����־λ
//int task2_flag = 0;
//int task3_flag = 0;
//int task4_flag = 0;
//int task5_flag = 0;
//int task6_flag = 0;
//int task7_flag = 0;

//// ������Ϣ��־λ
//int SendMessage = 0;
//// ������Ϣ��־λ
//int ReceiveMessage = 2;

//// ץȡλ�ô�������־λ
//int Grasping_Position_Sensor = 0;
//// �ڷ�λ�ô�������־λ
//int Placement_Position_Sensor = 0;

//// ���̵����˶���־λ
//int Chassis_Low_Speed_Movement = 0;
//// ���̸����˶���־λ
//int Chassis_High_Speed_Movement = 0;
//// ����ֹͣ�˶���־λ
//int Chassis_Stop_Movement = 0;

//// ����ת����־λ
//int long_lever_rotation_One_Position = 0;
//int long_lever_rotation_Two_Position = 0;
//int long_lever_rotation_Three_Position = 0;
//int long_lever_rotation_Four_Position = 0;
//int long_lever_rotation_Five_Position = 0;
//int long_lever_rotation_Six_Position = 0;

//// ������ȡ��־λ
//int Suction_Cup_Suction_One = 0;
//int Suction_Cup_Suction_Two = 0;
//int Suction_Cup_Suction_Three = 0;
//int Suction_Cup_Suction_Four = 0;
//int Suction_Cup_Suction_Five = 0;
//int Suction_Cup_Suction_Six = 0;

//// ����̧���־λ
//int Suction_Cup_Lift_Black_One = 0;
//int Suction_Cup_Lift_Black_Two = 0;
//int Suction_Cup_Lift_White_One = 0;
//int Suction_Cup_Lift_White_Two = 0;

//// �������˱�־λ
//int Aligning_The_Long_Lever1 = 0;
//int Aligning_The_Long_Lever2 = 0;

//// task1����ʵ��(�ܿ�)
//void task1(void *pvParameters)
//{
//		while (1)
//    {
//				// ң����
//				get_Rc_Measure_Point(&rc);
//				
//				// ����ģʽ
//				if(rc.rc.s1 == 1 && rc.rc.s2 == 1)
//				{
//						CAN1_CMD_motor34_2006(MOTOR_2006_Id1_4, 0, 0);
//						CAN1_CMD_motor12_2006(MOTOR_2006_Id1_4, 0, 0);
//						CAN1_CMD_motor1_3508(MOTOR_3508_Id5_8, 0);
//						CAN1_CMD_motor2_3508(MOTOR_3508_Id5_8, 0);
//						CAN2_CMD_motor_6020(MOTOR_6020_Id5_7, 0, 0, 0);
//						CAN2_CMD_motor34_3508(MOTOR_3508_Id5_8, 0, 0);
//						Target_Air_Pump = 0;
//					
//						task2_flag = 0;
//						task3_flag = 0;
//						task4_flag = 0;
//						task5_flag = 0;
//						task6_flag = 0;
//						task7_flag = 0;
//				}
//				// ��������
//				else
//				{
//						task2_flag = 1;
//						task3_flag = 1;
//						task4_flag = 1;
//						task5_flag = 1;
//						task6_flag = 1;
//						task7_flag = 1;
//				}
//				
//				// ��ʼ״̬
//				if(rc.rc.s1 == 1 && rc.rc.s2 == 3)
//				{
//						Target_6020 = 0;
//						Target_3508_White = Gear_Limit_Position;
//						Target_2006_White = White_Rack_Suction_One_Layer;
//						Target_3508_Black = 0;
//						Target_2006_Black = Black_Rack_Suction_One_Layer;
//						CAN2_CMD_motor34_3508(MOTOR_3508_Id5_8, 0, 0);
//						Target_Air_Pump = 0;
//						Target_Servo1 = Servo1_Suction;
//						Target_Servo3 = Servo3_Suction;
//				}
//				// ������Ϣ������ȴ�
//				else if(rc.rc.s1 == 1 && rc.rc.s2 == 2)
//				{
//						// ��������
//						if(SendMessage == 0)
//						{
//								const char* message = "start";
//							
//								// �����ַ���
//								USART_SendString(UART7, message);
//								
//								// ���±�־λ
//								SendMessage = 1;
//						}
//						
//						Communication(6);
//						
//						// ���յ���Ϣ��������ȷ
//						if(SendMessage == 1 && ReceiveMessage == 1)
//						{
//								buzzer_on(5, 25000);
//						}
//						// ���յ���Ϣ������ȷ
//						else if(SendMessage == 1 && ReceiveMessage == 2)
//						{
//								//�����˶�(����)
//								Target_3508_Chassis = Chassis_Low_Speed;
//							
//								// ����������Ϣ
//								ReceiveMessage = 3;
//								// ���̵����˶�
//								Chassis_Low_Speed_Movement = 1;
//						}
//						
//						// ���յ��������ź�
//						if(Chassis_Low_Speed_Movement == 1 && GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 0)
//						{
//								// �����˶�(ֹͣ)
//								Target_3508_Chassis = Chassis_Stop;
//							
//								// ��������
//								Target_Air_Pump = 1;
//							
//								Chassis_Low_Speed_Movement = 0;
//								Chassis_Stop_Movement = 1;
//						}
//						
//						// ת����һ��λ��(������)
//						if(Chassis_Stop_Movement == 1 && (motor_6020[2].total_ecd_out_6020>=-200&&motor_6020[2].total_ecd_out_6020<=200))
//						{
//								Target_6020 = 8192;
//							
//								Chassis_Stop_Movement = 0;
//								long_lever_rotation_One_Position = 1;
//						}
//						
//						// ץȡ����
//						if(long_lever_rotation_One_Position == 1 && (motor_6020[2].total_ecd_out_6020 >= 7800 && motor_6020[2].total_ecd_out_6020 <= 8100))
//						{
//								// ��ɫ������ȡ����
//								Target_2006_Black = Black_Rack_Suction_Box;
//							
//								// ��ʼ��ʱ
//								FreeRTOSRunTimeTicks = 0;
//								time_flag = 1;
//							
//								long_lever_rotation_One_Position = 0;
//								Suction_Cup_Suction_One = 1;
//						}
//						
//						// ̧��һ�����ӰڷŸ߶�
//						if(FreeRTOSRunTimeTicks == 6 && Suction_Cup_Suction_One == 1 && (motor_2006[1].total_ecd_out_2006 >= 7500 && motor_2006[1].total_ecd_out_2006 <= 7800))
//						{
//								Target_2006_Black = Black_Rack_Suction_One_Layer;
//							
//								// ֹͣ��ʱ����ռ�ʱ��
//								time_flag = 0;
//								FreeRTOSRunTimeTicks = 0;
//							
//								Suction_Cup_Suction_One = 0;
//								Suction_Cup_Lift_Black_One = 1;
//						}
//						
//						// ת�������λ��(������)
//						if(Suction_Cup_Lift_Black_One == 1)
//						{
//								Target_6020 = 11469;
//							
//								Suction_Cup_Lift_Black_One = 0;
//								long_lever_rotation_Five_Position = 1;
//						}
//						
//						// ץȡ����
//						if(long_lever_rotation_Five_Position == 1 && (motor_6020[2].total_ecd_out_6020 >= 11350 && motor_6020[2].total_ecd_out_6020 <= 11500))
//						{
//								// ��ɫ������ȡ����
//								Target_2006_White = White_Rack_Suction_Box;
//							
//								// ��ʼ��ʱ
//								FreeRTOSRunTimeTicks = 0;
//								time_flag = 1;
//							
//								long_lever_rotation_Five_Position = 0;
//								Suction_Cup_Suction_Five = 1;
//						}
//						
//						// ̧��һ�����ӰڷŸ߶�
//						if(FreeRTOSRunTimeTicks == 6 && Suction_Cup_Suction_Five == 1 && ((motor_2006[3].total_ecd_out_2006 >= 5000 && motor_2006[3].total_ecd_out_2006 <= 9000)))
//						{
//								Target_2006_White = White_Rack_Suction_One_Layer;
//							
//								// ֹͣ��ʱ����ռ�ʱ��
//								time_flag = 0;
//								FreeRTOSRunTimeTicks = 0;
//							
//								Suction_Cup_Suction_Five = 0;
//								Suction_Cup_Lift_White_One = 1;
//						}
//						
//						// ��������
//						if(Suction_Cup_Lift_White_One == 1)
//						{
//								Target_6020 = 8192;
//							
//								Suction_Cup_Lift_White_One = 0;
//								Aligning_The_Long_Lever1 = 1;
//						}
//						
//						// �ص�������
//						if(Aligning_The_Long_Lever1 == 1)
//						{
//								// �����˶�(����)
//								Target_3508_Chassis = -Chassis_High_Speed;
//							
//								Aligning_The_Long_Lever1 = 0;
//								Chassis_High_Speed_Movement = 1;
//						}
//						
//						// ��������������
//						if(Chassis_High_Speed_Movement == 1 && GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) == 0)
//						{
//								// �����˶�(����)
//								Target_3508_Chassis = -Chassis_Low_Speed;
//								
//								Chassis_High_Speed_Movement = 0;
//								Chassis_Low_Speed_Movement = 1;
//						}
//						
//						// ����������ֹͣ
//						if(Chassis_Low_Speed_Movement == 1 && GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) == 0)
//						{
//								// �����˶�(����)
//								Target_3508_Chassis = Chassis_Stop;
//							
//								Chassis_Low_Speed_Movement = 0;
//						}
//				}
//				
//				vTaskDelay(1);
//    }
//}

//// task2����ʵ��
//void task2(void *pvParameters)
//{
//		// PID��ʼ��
//		PID_Init(&Angular_Velocity_2006_White, PID_POSITION, PID_Angular_Velocity_2006_White, 3000, 0);
//		PID_Init(&Angular_2006_White, PID_POSITION, PID_Angular_2006_White, 25000, 0);

//		while (1)
//    {
//				flow_led_off(GPIO_Pin_1);
//			
//				if(task2_flag == 1)
//				{
//						flow_led_on(GPIO_Pin_1);
//					
//						// 2006_White �Ƕȿ���
//						Angle_Control_2006_White();
//				}
//			
//				vTaskDelay(1);
//    }
//}
//// task3����ʵ��
//void task3(void *pvParameters)
//{
//		// PID��ʼ��
//		PID_Init(&Angular_Velocity_2006_Black, PID_POSITION, PID_Angular_Velocity_2006_Black, 5000, 0);
//		PID_Init(&Angular_2006_Black, PID_POSITION, PID_Angular_2006_Black, 25000, 0);
//	
//    while (1)
//    {
//				flow_led_off(GPIO_Pin_2);
//			
//				if(task3_flag == 1)
//				{
//						flow_led_on(GPIO_Pin_2);
//					
//						// 2006_Black �Ƕȿ���
//						Angle_Control_2006_Black();
//				}
//				
//        vTaskDelay(1);
//    }
//}
//// task4����ʵ��
//void task4(void *pvParameters)
//{
//		// PID��ʼ��
//		PID_Init(&Angular_Velocity_3508_White, PID_POSITION, PID_Angular_Velocity_3508_White, 2000, 0);
//		PID_Init(&Angular_3508_White, PID_POSITION, PID_Angular_3508_White, 25000, 0);
//	
//		while (1)
//    {
//				flow_led_off(GPIO_Pin_3);
//			
//				if(task4_flag == 1)
//				{
//						flow_led_on(GPIO_Pin_3);
//					
//						// 3508_White �Ƕȿ���
//						Angle_Control_3508_White();
//				}
//				
//        vTaskDelay(1);
//    }
//}
//// task5����ʵ��
//void task5(void *pvParameters)
//{
//		// PID��ʼ��
//		PID_Init(&Angular_Velocity_3508_Black, PID_POSITION, PID_Angular_Velocity_3508_Black, 1800, 0);
//		PID_Init(&Angular_3508_Black, PID_POSITION, PID_Angular_3508_Black, 25000, 0);
//	
//		while (1)
//    {
//				flow_led_off(GPIO_Pin_4);
//			
//				if(task5_flag == 1)
//				{
//						flow_led_on(GPIO_Pin_4);
//					
//						// 3508_Black �Ƕȿ���
//						Angle_Control_3508_Black();
//				}
//				
//        vTaskDelay(1);
//    }
//}
//// task6����ʵ��
//void task6(void *pvParameters)
//{
//		// PID��ʼ��
//		PID_Init(&Angular_Velocity_6020, PID_POSITION, PID_Angular_Velocity_6020, 5000, 10000);
//		PID_Init(&Angular_6020, PID_POSITION, PID_Angular_6020, 950, 0);
//	
//		while (1)
//    {
//				flow_led_off(GPIO_Pin_5);
//			
//				if(task6_flag == 1)
//				{
//						flow_led_on(GPIO_Pin_5);
//					
//						// 6020_���� �Ƕȿ���
//						Angle_Control_6020();
//				}
//				
//        vTaskDelay(1);
//    }
//}
//// task7����ʵ��
//void task7(void *pvParameters)
//{
//		// PID��ʼ��
//		PID_Init(&Angular_Velocity_3508_Chassis, PID_POSITION, PID_Angular_Velocity_3508_Chassis, 7000, 200);
//		PID_Init(&Angular_3508_Chassis, PID_POSITION, PID_Angular_3508_Chassis, 3000, 0);
//	
//		while (1)
//    {
//				flow_led_off(GPIO_Pin_6);
//			
//				if(task7_flag == 1)
//				{
//						flow_led_on(GPIO_Pin_6);
//					
//						// ����λ�ÿ���
//						Angle_Control_3508_Chassis();
//				}
//				
//			
//        vTaskDelay(1);
//    }
//}
//// task8����ʵ��
//void task8(void *pvParameters)
//{
//	
//		while (1)
//    {
//				Air_Pump_Control();
//				
//				TIM_SetCompare1(TIM2, Target_Servo1);
//				TIM_SetCompare2(TIM2, Target_Servo3);
//			
//        vTaskDelay(1);
//    }
//}

//// �Ƚ����������Ƿ����
//_Bool compareArrays(const char* array1, const char* array2, int length)
//{
//		for (int i = 0; i < length; i++) 
//		{
//        if (array1[i] != array2[i]) 
//				{
//            return 0;
//        }
//    }
//		return 1;
//}
//// �����ж�
//_Bool Decision_Function(char USART_RX_BUF[], int length)
//{
//		int count0 = 0;
//    int count1 = 0;

//    for (int i = 0; i < length; i++) 
//		{
//					if (USART_RX_BUF[i] == '1') 
//					{
//							count0++;
//					} 
//					else if (USART_RX_BUF[i] == '2') 
//					{
//							count1++;
//					}
//					if (count0 == 3 && count1 == 3)
//					{
//							return 1;
//					}
//		}
//    return 0;
//}
//// ͨѶ
//void Communication(int length)
//{
//		if(USART_RX_STA&0x08)
//		{
//				ReceiveMessage = 1;
//				if(Decision_Function((char*)USART_RX_BUF, length))
//				{
//						ReceiveMessage = 2;
//				}
//				
//				USART_RX_STA = 0;
//		}
//}
//// ��ʱ
//void TimeKeeping(void)
//{
//		if(FreeRTOSRunTimeTicks < 6)
//		{
//				GPIO_ResetBits(GPIOG, GPIO_Pin_1);
//		}
//		else
//		{
//				GPIO_SetBits(GPIOG, GPIO_Pin_1);
//		}
//}
//// 6020�Ƕȿ���
//void Angle_Control_6020(void)
//{
//		Angular_Result_6020 = PID_Calc(&Angular_6020, motor_6020[2].total_ecd_out_6020, Target_6020);
//				
//		Angular_Velocity_Current_6020 = PID_Calc(&Angular_Velocity_6020, motor_6020[2].speed_rpm, Angular_Result_6020);
//				
//		CAN2_CMD_motor_6020(MOTOR_6020_Id5_7, 0, 0, Angular_Velocity_Current_6020);
//}

//// 3508_White�Ƕȿ���
//void Angle_Control_3508_White(void)
//{
//		Angular_Result_3508_White = PID_Calc(&Angular_3508_White, motor_3508[0].total_ecd_out_3508, Target_3508_White);
//				
//		Angular_Velocity_Current_3508_White = PID_Calc(&Angular_Velocity_3508_White, motor_3508[0].speed_rpm, Angular_Result_3508_White);
//				
//		CAN1_CMD_motor1_3508(MOTOR_3508_Id5_8, Angular_Velocity_Current_3508_White);
//}
//// 2006_White�Ƕȿ���
//void Angle_Control_2006_White(void)
//{
//		Angular_Result_2006_White = PID_Calc(&Angular_2006_White, motor_2006[3].total_ecd_out_2006, Target_2006_White);
//				
//		Angular_Velocity_Current_2006_White = PID_Calc(&Angular_Velocity_2006_White, motor_2006[3].speed_rpm, Angular_Result_2006_White);
//				
//		CAN1_CMD_motor34_2006(MOTOR_2006_Id1_4, -Angular_Velocity_Current_2006_White, Angular_Velocity_Current_2006_White);
//}
//// 3508_Black�Ƕȿ���
//void Angle_Control_3508_Black(void)
//{
//		Angular_Result_3508_Black = PID_Calc(&Angular_3508_Black, motor_3508[1].total_ecd_out_3508, Target_3508_Black);
//				
//		Angular_Velocity_Current_3508_Black = PID_Calc(&Angular_Velocity_3508_Black, motor_3508[1].speed_rpm, Angular_Result_3508_Black);
//		
//		CAN1_CMD_motor2_3508(MOTOR_3508_Id5_8, Angular_Velocity_Current_3508_Black);
//}
//// 2006_Black�Ƕȿ���
//void Angle_Control_2006_Black(void)
//{
//		Angular_Result_2006_Black = PID_Calc(&Angular_2006_Black, motor_2006[1].total_ecd_out_2006, Target_2006_Black);
//				
//		Angular_Velocity_Current_2006_Black = PID_Calc(&Angular_Velocity_2006_Black, motor_2006[1].speed_rpm, Angular_Result_2006_Black);
//		
//		CAN1_CMD_motor12_2006(MOTOR_2006_Id1_4, -Angular_Velocity_Current_2006_Black, Angular_Velocity_Current_2006_Black);
//}
//// 3508_Chassis�ٶȿ���
//void Angle_Control_3508_Chassis(void)
//{
//		Angular_Velocity_Current_3508_Chassis = PID_Calc(&Angular_Velocity_3508_Chassis, motor_3508[3].speed_rpm, Target_3508_Chassis);
//	
//		Angular_Result_3508_Chassis = PID_Calc(&Angular_3508_Chassis, motor_3508[3].given_current, Angular_Velocity_Current_3508_Chassis);
//		
//		CAN2_CMD_motor34_3508(MOTOR_3508_Id5_8, -Angular_Result_3508_Chassis, Angular_Result_3508_Chassis);
//}
//// ���ÿ���
//void Air_Pump_Control(void)
//{
//		if(Target_Air_Pump == 0)
//		{
//				GPIO_ResetBits(GPIOF, GPIO_Pin_0);
//				GPIO_ResetBits(GPIOF, GPIO_Pin_1);
//				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
//				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
//		}
//		else if(Target_Air_Pump == 1)
//		{
//				GPIO_SetBits(GPIOF, GPIO_Pin_0);
//				GPIO_SetBits(GPIOF, GPIO_Pin_1);
//				GPIO_SetBits(GPIOE, GPIO_Pin_4);
//				GPIO_SetBits(GPIOE, GPIO_Pin_5);
//		}
//}
//// �����ַ�������
//void USART_SendString(USART_TypeDef* USARTx, const char* str)
//{
//		// �����ַ����е�ÿ���ַ�����һ��һ������
//		for (size_t i = 0; i < strlen(str); i++)
//		{
//				// �ȴ����ݷ������
//				while (!(USARTx->SR & USART_SR_TXE));
//				
//				// ���ַ����͵��������ݼĴ���
//				USARTx->DR = (uint16_t)str[i];
//				
//				// �ȴ����ݷ������
//				while (!(USARTx->SR & USART_SR_TC));
//		}
//}

