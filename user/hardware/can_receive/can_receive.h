#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

// ͷ�ļ�
#include "main.h"
#include "stm32f4xx.h"

// ���ID������
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
// ����������ݽṹ��
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
	
		//����Ϊ����ԭʼ���ݵ�������
		uint16_t	offset_ecd;
		int32_t		round_cnt;  					//���ۿɼ�¼524352Ȧ
		int32_t		total_ecd;  					//���ۿɼ�¼4294967295����ֵ
		
		int32_t		total_ecd_out_2006;  	//���ۿɼ�¼14565Ȧ
		int32_t		total_ecd_out_3508;  	//���ۿɼ�¼27305Ȧ
		int32_t		total_ecd_out_6020;  	//���ۿɼ�¼524352Ȧ
} motor_measure;

// ������������ṹ��
typedef struct
{
	uint8_t motor_6020[3];
	uint8_t motor_3508[4];
	uint8_t motor_2006[4];
	
}motor_feedback_time;

// �������ֵ�ṹ��
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

// �ⲿ�����������
extern motor_measure motor_2006[4];
extern motor_measure motor_3508[4];
extern motor_measure motor_6020[3];

// can�ص���������
void CAN1_hook(CanRxMsg *rx_message);
void CAN2_hook(CanRxMsg *rx_message);

// CAN�������͵�����������
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
