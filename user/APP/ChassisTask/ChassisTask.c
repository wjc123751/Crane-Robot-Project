#include "all_head.h"

/*3508底盘*/
// 定义PID结构体
PidTypeDef Current_Loop_3508_Chassis;
PidTypeDef Speed_Loop_3508_Chassis;
// 电流环PID参数
fp32 PID_Angular_Current_Loop_3508_Chassis[3] = {60.f, 0.0001f, 29.9999f};
// 速度环PID参数
fp32 PID_Angular_Speed_Loop_3508_Chassis[3] = {3.f, 0.f, 0.001f};
// PID计算结果
int16_t Result_Current_Loop_3508_Chassis;
int16_t Result_Speed_Loop_3508_Chassis;

void ChassisTask(void *pvParameters)
{
		// PID初始化
		PID_Init(&Current_Loop_3508_Chassis, PID_POSITION, PID_Angular_Current_Loop_3508_Chassis, 7000, 200);
		PID_Init(&Speed_Loop_3508_Chassis, PID_POSITION, PID_Angular_Speed_Loop_3508_Chassis, 3000, 0);
		
		while(1)
		{
			if (Key_value == 0)
			{
					CAN2_CMD_motor34_3508(MOTOR_3508_Id5_8, 0, 0);
				
					TIM_SetCompare1(TIM2, Servo1_Suction);
					TIM_SetCompare2(TIM2, Servo3_Suction);
				
					CraneMasterDataSet.target_air_pump.target_air_pump1 = 0;
					CraneMasterDataSet.target_air_pump.target_air_pump_left = 0;
					CraneMasterDataSet.target_air_pump.target_air_pump_middle = 0;
					CraneMasterDataSet.target_air_pump.target_air_pump_right = 0;
			}
			else
			{
					// 底盘位置控制
					Result_Current_Loop_3508_Chassis = PID_Calc(&Current_Loop_3508_Chassis, motor_3508[3].speed_rpm, Motor_Expect.Target_Speed_3508_Chassis);
					Result_Speed_Loop_3508_Chassis = PID_Calc(&Speed_Loop_3508_Chassis, motor_3508[3].given_current, Result_Current_Loop_3508_Chassis);
					CAN2_CMD_motor34_3508(MOTOR_3508_Id5_8, -Result_Speed_Loop_3508_Chassis, Result_Speed_Loop_3508_Chassis);
			}
			
			vTaskDelay(1);
		}
}



