#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

// 头文件
#include "main.h"
#include "stm32f4xx.h"

// 电机ID共用体
enum
{
		MOTOR_2006_Id1_4 = 0x200,
		MOTOR_2006_Id5_8 = 0x1FF,
		MOTOR_3508_Id1_4 = 0x200,
		MOTOR_3508_Id5_8 = 0x1FF,
		MOTOR_6020_Id1_4 = 0x1FF,
		MOTOR_6020_Id5_7 = 0x2FF,
	
		CAN_2006_M1_ID = 0x201,
		CAN_2006_M2_ID = 0x202,
		CAN_2006_M3_ID = 0x203,
		CAN_2006_M4_ID = 0x204,
	
    CAN_3508_M1_ID = 0x205,
    CAN_3508_M2_ID = 0x206,
    CAN_3508_M3_ID = 0x207,
    CAN_3508_M4_ID = 0x208,
	
		CAN_6020_M1_ID = 0x209,
		CAN_6020_M2_ID = 0x20A,
		CAN_6020_M3_ID = 0x20B,
};
// 电机反馈数据结构体
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
	
		//以下为根据原始数据的推算量
		uint16_t	offset_ecd;
		int32_t		round_cnt;  					//理论可记录524352圈
		int32_t		total_ecd;  					//理论可记录4294967295编码值
		
		int32_t		total_ecd_out_2006;  	//理论可记录14565圈
		int32_t		total_ecd_out_3508;  	//理论可记录27305圈
		int32_t		total_ecd_out_6020;  	//理论可记录524352圈
} motor_measure;

// 电机反馈次数结构体
typedef struct
{
	uint8_t motor_6020[3];
	uint8_t motor_3508[4];
	uint8_t motor_2006[4];
	
}motor_feedback_time;

// 电机期望值结构体
typedef struct
{
	int32_t Target_Speed_3508_Chassis;
	int32_t Angle_Target_6020;
	int32_t Target_3508_White;
	int32_t Target_3508_Black;
	int32_t Target_2006_White;
	int32_t Target_2006_Black;
	
}motor_expect;

extern motor_expect Motor_Expect;

// 外部声明电机变量
extern motor_measure motor_2006[4];
extern motor_measure motor_3508[4];
extern motor_measure motor_6020[3];

// can回调函数声明
void CAN1_hook(CanRxMsg *rx_message);
void CAN2_hook(CanRxMsg *rx_message);

// CAN向电机发送电流函数声明
void CAN1_CMD_motor12_2006(uint32_t StdId, int16_t motor1, int16_t motor2);
void CAN1_CMD_motor34_2006(uint32_t StdId, int16_t motor3, int16_t motor4);
void CAN1_CMD_2006(uint32_t StdId, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);


void CAN1_CMD_motor1_3508(uint32_t StdId, int16_t motor1);
void CAN1_CMD_motor2_3508(uint32_t StdId, int16_t motor2);
void CAN1_CMD_3508(uint32_t StdId, int16_t motor1, int16_t motor2);

void CAN1_CMD_motor_6020(uint32_t StdId, int16_t motor1, int16_t motor2, int16_t motor3);

void CAN2_CMD_motor_2006(uint32_t StdId, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN2_CMD_motor34_3508(uint32_t StdId, int16_t motor3, int16_t motor4);
void CAN2_CMD_motor_6020(uint32_t StdId, int16_t motor1, int16_t motor2, int16_t motor3);

#endif
