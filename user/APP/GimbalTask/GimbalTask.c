#include "all_head.h"

/*6020����*/
// ����PID�ṹ��
PidTypeDef Angular_Velocity_6020;
PidTypeDef Angular_6020;
// �ٶȻ�PID����
fp32 PID_Angular_Velocity_6020[3] = {160.f, 2.f, 7.f};
// �ǶȻ�PID����
fp32 PID_Angular_6020[3] = {0.0080120001f, 0.0000f, 0.00079f};
// PID������
int16_t Angular_Velocity_Current_6020;
int16_t Angular_Result_6020;

void GimbalTask(void *pvParameters)
{
		// PID��ʼ��
		PID_Init(&Angular_Velocity_6020, PID_POSITION, PID_Angular_Velocity_6020, 5800, 10000);
		PID_Init(&Angular_6020, PID_POSITION, PID_Angular_6020, 950, 0);
	
		while (1)
		{
			if (Key_value == 0)
			{
					CAN2_CMD_motor_6020(MOTOR_6020_Id5_7, 0, 0, 0);
			}
			else
			{
					// 6020_���� �Ƕȿ���
					Angular_Result_6020 = PID_Calc(&Angular_6020, motor_6020[2].total_ecd_out_6020, Motor_Expect.Angle_Target_6020);
					Angular_Velocity_Current_6020 = PID_Calc(&Angular_Velocity_6020, motor_6020[2].speed_rpm, Angular_Result_6020);
					CAN2_CMD_motor_6020(MOTOR_6020_Id5_7, 0, 0, Angular_Velocity_Current_6020);
			}
			
			vTaskDelay(1);
		}
	
}
