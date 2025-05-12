#include "all_head.h"

/*3508白齿轮*/
// 定义PID结构体
PidTypeDef Angular_Velocity_3508_White;
PidTypeDef Angular_3508_White;
// 速度环PID参数
fp32 PID_Angular_Velocity_3508_White[3] = {15.f, 0.f, 0.f};
// 角度环PID参数
fp32 PID_Angular_3508_White[3] = {3.f, 0.f, 0.5f};
// PID计算结果
int16_t Angular_Velocity_Current_3508_White;
int16_t Angular_Result_3508_White;

/*3508黑齿轮*/
// 定义PID结构体
PidTypeDef Angular_Velocity_3508_Black;
PidTypeDef Angular_3508_Black;
// 速度环PID参数
fp32 PID_Angular_Velocity_3508_Black[3] = {15.f, 0.f, 0.f};
// 角度环PID参数
fp32 PID_Angular_3508_Black[3] = {3.f, 0.f, 5.f};
// PID计算结果
int16_t Angular_Velocity_Current_3508_Black;
int16_t Angular_Result_3508_Black;

void LeftAndRightTask(void *pvParameters)
{
		// PID初始化
		PID_Init(&Angular_Velocity_3508_White, PID_POSITION, PID_Angular_Velocity_3508_White, 2000, 0);
		PID_Init(&Angular_3508_White, PID_POSITION, PID_Angular_3508_White, 25000, 0);
	
		PID_Init(&Angular_Velocity_3508_Black, PID_POSITION, PID_Angular_Velocity_3508_Black, 1800, 0);
		PID_Init(&Angular_3508_Black, PID_POSITION, PID_Angular_3508_Black, 25000, 0);
		
		while (1)
		{
				if (Key_value == 0)
				{
						CAN1_CMD_3508(MOTOR_3508_Id5_8, 0, 0);
				}
				else
				{
						Angular_Result_3508_White = PID_Calc(&Angular_3508_White, motor_3508[0].total_ecd_out_3508, Motor_Expect.Target_3508_White);
						Angular_Velocity_Current_3508_White = PID_Calc(&Angular_Velocity_3508_White, motor_3508[0].speed_rpm, Angular_Result_3508_White/5);
					
						Angular_Result_3508_Black = PID_Calc(&Angular_3508_Black, motor_3508[1].total_ecd_out_3508, Motor_Expect.Target_3508_Black);
						Angular_Velocity_Current_3508_Black = PID_Calc(&Angular_Velocity_3508_Black, motor_3508[1].speed_rpm, Angular_Result_3508_Black/5);
				
						CAN1_CMD_3508(MOTOR_3508_Id5_8, Angular_Velocity_Current_3508_White, Angular_Velocity_Current_3508_Black);
				}
			
				vTaskDelay(1);
		}
	
}
