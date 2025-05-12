#ifndef RC_H
#define RC_H

// 头文件
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"

// 定义遥控器数据结构体
typedef __packed struct
{
	__packed struct
	{
		int16_t ch0;
		int16_t ch1;
		int16_t ch2;
		int16_t ch3;
		int16_t ch4;
		uint8_t s1;
		uint8_t s2;
	} rc;
	__packed struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	} mouse;
	__packed struct
	{
		uint16_t v;
	} key;
} RC_Ctl_t;

// 函数声明
void RC_Init(void);
void RemoteDataProcess(uint8_t *pData);
void get_Rc_Measure_Point(RC_Ctl_t* RC);

#endif
