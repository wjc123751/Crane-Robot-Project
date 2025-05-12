#include "all_head.h"

/*2006白齿条*/
// 定义PID结构体
PidTypeDef Angular_Velocity_2006_White;
PidTypeDef Angular_2006_White;
// 速度环PID参数
fp32 PID_Angular_Velocity_2006_White[3] = {14.99f, 0.f, 14.98f};
// 角度环PID参数
fp32 PID_Angular_2006_White[3] = {10.f, 0.f, 1.f};
// PID计算结果
int16_t Angular_Velocity_Current_2006_White;
int16_t Angular_Result_2006_White;

/*2006黑齿条*/
// 定义PID结构体
PidTypeDef Angular_Velocity_2006_Black;
PidTypeDef Angular_2006_Black;
// 速度环PID参数
fp32 PID_Angular_Velocity_2006_Black[3] = {25.f, 0.f, 1.f};
// 角度环PID参数
fp32 PID_Angular_2006_Black[3] = {3.f, 0.f, 1.f};
// PID计算结果
int16_t Angular_Velocity_Current_2006_Black;
int16_t Angular_Result_2006_Black;

void UpAndDownTask(void *pvParameters)
{
		// PID初始化
		PID_Init(&Angular_Velocity_2006_White, PID_POSITION, PID_Angular_Velocity_2006_White, 3000, 0);
		PID_Init(&Angular_2006_White, PID_POSITION, PID_Angular_2006_White, 25000, 0);
	
		PID_Init(&Angular_Velocity_2006_Black, PID_POSITION, PID_Angular_Velocity_2006_Black, 5000, 0);
		PID_Init(&Angular_2006_Black, PID_POSITION, PID_Angular_2006_Black, 25000, 0);
	
		while (1)
		{
				if (Key_value == 0)
				{
						CAN1_CMD_2006(MOTOR_2006_Id1_4, 0, 0, 0, 0);
				}
				else
				{
						Angular_Result_2006_White = PID_Calc(&Angular_2006_White, motor_2006[3].total_ecd_out_2006, Motor_Expect.Target_2006_White);
						Angular_Velocity_Current_2006_White = PID_Calc(&Angular_Velocity_2006_White, motor_2006[3].speed_rpm, Angular_Result_2006_White/4);
					
						Angular_Result_2006_Black = PID_Calc(&Angular_2006_Black, motor_2006[1].total_ecd_out_2006, Motor_Expect.Target_2006_Black);
						Angular_Velocity_Current_2006_Black = PID_Calc(&Angular_Velocity_2006_Black, motor_2006[1].speed_rpm, Angular_Result_2006_Black);
					
						CAN1_CMD_2006(MOTOR_2006_Id1_4, -Angular_Velocity_Current_2006_Black, Angular_Velocity_Current_2006_Black, -Angular_Velocity_Current_2006_White, Angular_Velocity_Current_2006_White);
				}

				vTaskDelay(1);
		}
	
}
