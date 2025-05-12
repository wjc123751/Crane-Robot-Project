#ifndef MAIN_H
#define MAIN_H
/* ----------------------------- #DEFINE -------------------------------*/
#ifndef NULL
#define NULL 0
#endif

#ifndef PI
#define PI 3.14159265358979f
#endif

#define CAN1_NVIC 4
#define CAN2_NVIC 4
/* --------------------------- data type --------------------------------*/
#include "rc.h"
#include "can_receive.h"

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;

typedef enum 
{
	CraneReady = 1, 
	CraneSendMessage, 
	CraneReceiveMessage, 
	CraneForwardLowSpeed, 
	CraneForwardStop, 
	CraneRotateExpect1A,
	CraneBlackRackSuctionBox1,
	CraneBlackRackSuctionBox1Wait,
	CraneBlackRackSuctionBox1_OneLayer,
	CraneRotateExpect2C,
	CraneWhiteRackSuctionBox,
	CraneWhiteRackSuctionBoxWait,
	CraneWhiteRackSuctionBox_OneLayer,
	CranePlacementOne,
	CraneBackwardLowSpeed,
	CraneBackwardStop,
	CranePlacementOneFinish,
	CraneForwardHighSpeed,
	CraneReady1,
	CraneForwardLowSpeed1, 
	CraneForwardStop1, 
	CraneRotate_Expect1B,
	CraneRotate_Expect1BWait,
	CraneRotate_Expect1BStop,
	CraneBlackRackSuctionBox2,
	CraneBlackRackSuctionBox2Wait,
	CraneBlackRackSuctionBox2OneLayer,
	CraneRotate_Expect2D,
	CraneRotate_Expect2DStop,
	CraneWhiteRackSuctionCoca_Cola3,
	CraneWhiteRackSuctionCoca_Cola3Wait,
	CraneWhiteRackSuctionCoca_Cola3Finish,
	CraneRotate_Expect2E,
	CraneRotate_Expect2EStop,
	CraneWhiteRackSuctionCoca_Cola2,
	CraneWhiteRackSuctionCoca_Cola2Wait,
	CraneWhiteRackSuctionCoca_Cola2Finish,
	CraneRotate_Expect2F,
	CraneRotate_Expect2FStop,
	CraneWhiteRackSuctionCoca_Cola1,
	CraneWhiteRackSuctionCoca_Cola1Wait,
	CraneWhiteRackSuctionCoca_Cola1Finish,
	CraneBackwardHighSpeed,
	CraneBackwardLowSpeed1,
	CraneBackwardStop1,
	CranePlacementTwoFinish,
	CraneHelloWorld
} CraneMasterStatus;

typedef struct
{
		int32_t Rotate_Expect1A;
		int32_t Rotate_Expect1B;
		int32_t Rotate_Expect2C;
		int32_t Rotate_Expect2D;
		int32_t Rotate_Expect2E;
		int32_t Rotate_Expect2F;
		int32_t Rotate_Expect1Done;
		int32_t Rotate_Expect2Done;
} Rotate_Expect;

typedef struct
{
		int8_t target_air_pump1;
		int8_t target_air_pump_left;
		int8_t target_air_pump_right;
		int8_t target_air_pump_middle;
} Target_Air_Pump;

typedef struct 
{
	RC_Ctl_t rc_ctrol;// 遥控器

	int8_t sensor1;// 传感器
	int8_t sensor2;// 传感器
	int8_t sensor3;// 传感器
	int8_t sensor4;// 传感器

	u8 usart_rx_buf[6];// 串口接收数组
	
	// 老标志位
//	int8_t ReceiveMessage;
//	int8_t SendMessage;
	
	Rotate_Expect rotate_expect;
	Target_Air_Pump target_air_pump;
	
//	int8_t CarryStatus;
	
	// 新标志位
	int8_t CraneStatus;
	
}CraneMasterData;

extern uint8_t Key;
extern uint8_t Key_value;
extern CraneMasterData CraneMasterDataSet;

#endif

