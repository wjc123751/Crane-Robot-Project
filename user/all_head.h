#ifndef ALL_HEAD_H
#define ALL_HEAD_H

/****��������ת��****/
#include "main.h"

/****Stm32�⺯���ļ�****/
#include "stm32f4xx.h"

/****C�⺯�������ͷ�ļ�****/
#include <stdio.h> 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/****����ϵͳ****/
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_Middleware.h"

/****HardWare(Ӳ��)***/
#include "delay.h"
#include "led.h"
#include "rc.h"
#include "can.h"
#include "timer.h"
#include "usart7.h"
#include "usart8.h"
#include "buzzer.h"
#include "can_receive.h"
#include "pid.h"
#include "sys.h"

/***�궨��***/
// �궨�����ٶ�
#define Chassis_Stop 										0
#define Chassis_Forward_Low_Speed 			700
#define Chassis_Forward_High_Speed 			3100
#define Chassis_Backward_Low_Speed 			-700
#define Chassis_Backward_High_Speed 		-3100

// �궨����λ��
#define Servo1_Suction 										177
#define Servo1_Placement 									186
#define Servo3_Suction 										186
#define Servo3_Left_Suction 							178
#define Servo3_Right_Suction 							194
#define Servo3_Placement 									186

// �궨�����λ��
#define White_Rack_Ready 									12000
#define Black_Rack_Ready						 			12000
#define White_Rack_Suction_One_Layer 			26000
#define Black_Rack_Suction_One_Layer 			24500
#define White_Rack_Suction_Devolved_One		23500
#define Black_Rack_Suction_Devolved_One	 	22000
#define White_Rack_Suction_Lift1 					26500
#define Black_Rack_Suction_Lift1			 		25000
#define White_Rack_Suction_Two_Layer 			32100
#define Black_Rack_Suction_Two_Layer 			31200
#define White_Rack_Suction_Devolved_Two		31000
#define Black_Rack_Suction_Devolved_Two	 	30100
#define White_Rack_Suction_Lift2 					32400
#define Black_Rack_Suction_Lift2		 			31300
#define White_Rack_Suction_Box 						7600
#define White_Rack_Suction_Coca_Cola 			7600
#define White_Rack_Suction_Coca_Cola3 		7580
#define Black_Rack_Suction_Box1 					7500
#define Black_Rack_Suction_Box2 					7450

// �궨�������λλ��
#define White_Gear_Limit_Position 				-3550
#define Black_Gear_Limit_Position 				-200
#define White_Gear_Buffer_Position 				-1950
#define Black_Gear_Buffer_Position 				3000
#define White_Placement_One								2500
#define Black_Placement_One								5420
#define White_Placement_Two								1700
#define Black_Placement_Two								5420

#endif
