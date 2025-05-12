// 头文件
#include "can_receive.h"

// 宏函数电机数据初始化
#define get_user_motor_measure_init(ptr, rx_message)                                           \
		{                                                                                          \
			(ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);             \
			(ptr)->offset_ecd = (ptr)->ecd;                                                          \
		}
// 宏函数读取电机数据
#define get_motor_measure(ptr, rx_message)                                                                 	  			\
    {                                                                                                        				\
        (ptr)->last_ecd = (ptr)->ecd;                                                                        				\
        (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);                         				\
        (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]);                   				\
        (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]);               				\
        (ptr)->temperate = (rx_message)->Data[6];                                                            				\
																																																										\
				/*以下为根据原始数据的推算*/																													                				\
																																																										\
				if((ptr)->ecd - (ptr)->last_ecd > 4096)                                                              				\
					(ptr)->round_cnt --;                                                                             					\
				else if ((ptr)->ecd - (ptr)->last_ecd < -4096)                                                       				\
					(ptr)->round_cnt ++;                                                                             					\
				(ptr)->total_ecd = (ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd;                        				\
																																																										\
				(ptr)->total_ecd_out_2006 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) / 36.f;             	\
				(ptr)->total_ecd_out_3508 = ((ptr)->round_cnt * 8192 + (ptr)->ecd - (ptr)->offset_ecd) * 187.f / 3591.f;   	\
				(ptr)->total_ecd_out_6020 = (ptr)->total_ecd;           																		 							 	\
   }
// CAN1中断服务函数
CanRxMsg rx1_message;
void CAN1_RX0_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx1_message);//FIFO 0中是否有消息待处理
        CAN1_hook(&rx1_message);
    }
}
// CAN2中断服务函数
CanRxMsg rx2_message;
void CAN2_RX1_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET)
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
        CAN_Receive(CAN2, CAN_FIFO1, &rx2_message);
        CAN2_hook(&rx2_message);
    }
}
// 定义电机变量
motor_measure motor_2006[4], motor_3508[4], motor_6020[3];
// 定义电机反馈次数
motor_feedback_time feedback_time;
// 电机期望
motor_expect Motor_Expect;
// CAN1回调函数
static void CAN1_hook(CanRxMsg *rx_message)
{
		switch (rx_message->StdId)
    {
				case CAN_2006_M1_ID:
				case CAN_2006_M2_ID:
				case CAN_2006_M3_ID:
				case CAN_2006_M4_ID:
				{
						uint8_t i = 0;
						i = rx_message->StdId - CAN_2006_M1_ID;
					
						if(feedback_time.motor_2006[i] == 0)
						{
								get_user_motor_measure_init(&motor_2006[i], rx_message);
								feedback_time.motor_2006[i] = 1;
						}
						get_motor_measure(&motor_2006[i], rx_message);
						break;
				}
				case CAN_3508_M1_ID:
				case CAN_3508_M2_ID:
				{
						uint8_t i = 0;
						i = rx_message->StdId - CAN_3508_M1_ID;
					
						if(feedback_time.motor_3508[i] == 0)
						{
								get_user_motor_measure_init(&motor_3508[i], rx_message);
								feedback_time.motor_3508[i] = 1;
						}
						get_motor_measure(&motor_3508[i], rx_message);
						break;
				}
				default:
				{
						break;
				}
		}
}
// CAN2回调函数
static void CAN2_hook(CanRxMsg *rx_message)
{
		switch (rx_message->StdId)
    {
				case CAN_3508_M3_ID:
				case CAN_3508_M4_ID:
				{
						static uint8_t i = 0;
						i = rx_message->StdId - CAN_3508_M1_ID;
					
						if(feedback_time.motor_3508[i] == 0)
						{
								get_user_motor_measure_init(&motor_3508[i], rx_message);
								feedback_time.motor_3508[i] = 1;
						}
						get_motor_measure(&motor_3508[i], rx_message);
						break;
				}
				case CAN_6020_M1_ID:
				case CAN_6020_M2_ID:
				case CAN_6020_M3_ID:
				{
						static uint8_t i = 0;
						i = rx_message->StdId - CAN_6020_M1_ID;
					
						if(feedback_time.motor_6020[i] == 0)
						{
								get_user_motor_measure_init(&motor_6020[i], rx_message);
								feedback_time.motor_6020[i] = 1;
						}
						get_motor_measure(&motor_6020[i], rx_message);
						break;
				}
				default:
				{
						break;
				}
		}
}


// CAN1向2006发送电流函数
CanTxMsg motor_2006_TxMessage;
void CAN1_CMD_motor12_2006(uint32_t StdId, int16_t motor1, int16_t motor2)
{
		motor_2006_TxMessage.StdId = StdId;
		motor_2006_TxMessage.IDE = CAN_ID_STD;
		motor_2006_TxMessage.RTR = CAN_RTR_DATA;
		motor_2006_TxMessage.DLC = 0x08;
		motor_2006_TxMessage.Data[0] = (motor1 >> 8);
		motor_2006_TxMessage.Data[1] = motor1;
		motor_2006_TxMessage.Data[2] = (motor2 >> 8);
		motor_2006_TxMessage.Data[3] = motor2;
	
		CAN_Transmit(CAN1, &motor_2006_TxMessage);
}
void CAN1_CMD_motor34_2006(uint32_t StdId, int16_t motor3, int16_t motor4)
{
		motor_2006_TxMessage.StdId = StdId;
		motor_2006_TxMessage.IDE = CAN_ID_STD;
		motor_2006_TxMessage.RTR = CAN_RTR_DATA;
		motor_2006_TxMessage.DLC = 0x08;
		motor_2006_TxMessage.Data[4] = (motor3 >> 8);
		motor_2006_TxMessage.Data[5] = motor3;
		motor_2006_TxMessage.Data[6] = (motor4 >> 8);
		motor_2006_TxMessage.Data[7] = motor4;
	
		CAN_Transmit(CAN1, &motor_2006_TxMessage);
}

void CAN1_CMD_2006(uint32_t StdId, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
		motor_2006_TxMessage.StdId = StdId;
		motor_2006_TxMessage.IDE = CAN_ID_STD;
		motor_2006_TxMessage.RTR = CAN_RTR_DATA;
		motor_2006_TxMessage.DLC = 0x08;
		motor_2006_TxMessage.Data[0] = (motor1 >> 8);
		motor_2006_TxMessage.Data[1] = motor1;
		motor_2006_TxMessage.Data[2] = (motor2 >> 8);
		motor_2006_TxMessage.Data[3] = motor2;
		motor_2006_TxMessage.Data[4] = (motor3 >> 8);
		motor_2006_TxMessage.Data[5] = motor3;
		motor_2006_TxMessage.Data[6] = (motor4 >> 8);
		motor_2006_TxMessage.Data[7] = motor4;
	
		CAN_Transmit(CAN1, &motor_2006_TxMessage);
}

// CAN1向3508发送电流函数
CanTxMsg motor_3508_TxMessage;
void CAN1_CMD_motor1_3508(uint32_t StdId, int16_t motor1)
{
		motor_3508_TxMessage.StdId = StdId;
		motor_3508_TxMessage.IDE = CAN_ID_STD;
		motor_3508_TxMessage.RTR = CAN_RTR_DATA;
		motor_3508_TxMessage.DLC = 0x08;
		motor_3508_TxMessage.Data[0] = (motor1 >> 8);
		motor_3508_TxMessage.Data[1] = motor1;
	
		CAN_Transmit(CAN1, &motor_3508_TxMessage);
}
void CAN1_CMD_motor2_3508(uint32_t StdId, int16_t motor2)
{
		motor_3508_TxMessage.StdId = StdId;
		motor_3508_TxMessage.IDE = CAN_ID_STD;
		motor_3508_TxMessage.RTR = CAN_RTR_DATA;
		motor_3508_TxMessage.DLC = 0x08;
		motor_3508_TxMessage.Data[2] = (motor2 >> 8);
		motor_3508_TxMessage.Data[3] = motor2;
	
		CAN_Transmit(CAN1, &motor_3508_TxMessage);
}

void CAN1_CMD_3508(uint32_t StdId, int16_t motor1, int16_t motor2)
{
		motor_3508_TxMessage.StdId = StdId;
		motor_3508_TxMessage.IDE = CAN_ID_STD;
		motor_3508_TxMessage.RTR = CAN_RTR_DATA;
		motor_3508_TxMessage.DLC = 0x08;
		motor_3508_TxMessage.Data[0] = (motor1 >> 8);
		motor_3508_TxMessage.Data[1] = motor1;
		motor_3508_TxMessage.Data[2] = (motor2 >> 8);
		motor_3508_TxMessage.Data[3] = motor2;
	
		CAN_Transmit(CAN1, &motor_3508_TxMessage);
}

// CAN1向6020发送电流函数
void CAN1_CMD_motor_6020(uint32_t StdId, int16_t motor1, int16_t motor2, int16_t motor3)
{
		CanTxMsg motor_6020_TxMessage;
		motor_6020_TxMessage.StdId = StdId;
		motor_6020_TxMessage.IDE = CAN_ID_STD;
		motor_6020_TxMessage.RTR = CAN_RTR_DATA;
		motor_6020_TxMessage.DLC = 0x08;
		motor_6020_TxMessage.Data[0] = (motor1 >> 8);
		motor_6020_TxMessage.Data[1] = motor1;
		motor_6020_TxMessage.Data[2] = (motor2 >> 8);
		motor_6020_TxMessage.Data[3] = motor2;
		motor_6020_TxMessage.Data[4] = (motor3 >> 8);
		motor_6020_TxMessage.Data[5] = motor3;
	
		CAN_Transmit(CAN1, &motor_6020_TxMessage);
}

// CAN2向2006发送电流函数
void CAN2_CMD_motor_2006(uint32_t StdId, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
		CanTxMsg motor_2006_TxMessage;
		motor_2006_TxMessage.StdId = StdId;
		motor_2006_TxMessage.IDE = CAN_ID_STD;
		motor_2006_TxMessage.RTR = CAN_RTR_DATA;
		motor_2006_TxMessage.DLC = 0x08;
		motor_2006_TxMessage.Data[0] = (motor1 >> 8);
		motor_2006_TxMessage.Data[1] = motor1;
		motor_2006_TxMessage.Data[2] = (motor2 >> 8);
		motor_2006_TxMessage.Data[3] = motor2;
		motor_2006_TxMessage.Data[4] = (motor3 >> 8);
		motor_2006_TxMessage.Data[5] = motor3;
		motor_2006_TxMessage.Data[6] = (motor4 >> 8);
		motor_2006_TxMessage.Data[7] = motor4;
	
		CAN_Transmit(CAN2, &motor_2006_TxMessage);
}
// CAN2向3508发送电流函数
void CAN2_CMD_motor34_3508(uint32_t StdId, int16_t motor3, int16_t motor4)
{
//		CanTxMsg motor_3508_TxMessage;
		motor_3508_TxMessage.StdId = StdId;
		motor_3508_TxMessage.IDE = CAN_ID_STD;
		motor_3508_TxMessage.RTR = CAN_RTR_DATA;
		motor_3508_TxMessage.DLC = 0x08;
		motor_3508_TxMessage.Data[4] = (motor3 >> 8);
		motor_3508_TxMessage.Data[5] = motor3;
		motor_3508_TxMessage.Data[6] = (motor4 >> 8);
		motor_3508_TxMessage.Data[7] = motor4;
	
		CAN_Transmit(CAN2, &motor_3508_TxMessage);
}
// CAN1向6020发送电流函数
CanTxMsg motor_6020_TxMessage;
void CAN2_CMD_motor_6020(uint32_t StdId, int16_t motor1, int16_t motor2, int16_t motor3)
{
		motor_6020_TxMessage.StdId = StdId;
		motor_6020_TxMessage.IDE = CAN_ID_STD;
		motor_6020_TxMessage.RTR = CAN_RTR_DATA;
		motor_6020_TxMessage.DLC = 0x08;
		motor_6020_TxMessage.Data[0] = (motor1 >> 8);
		motor_6020_TxMessage.Data[1] = motor1;
		motor_6020_TxMessage.Data[2] = (motor2 >> 8);
		motor_6020_TxMessage.Data[3] = motor2;
		motor_6020_TxMessage.Data[4] = (motor3 >> 8);
		motor_6020_TxMessage.Data[5] = motor3;
	
		CAN_Transmit(CAN2, &motor_6020_TxMessage);
}
