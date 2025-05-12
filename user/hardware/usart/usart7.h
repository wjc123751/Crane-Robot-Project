#ifndef _USART7_H
#define _USART7_H

// 头文件
#include "stm32f4xx.h"
#include "stdio.h"
#include "math.h"

// 宏定义
#define tail 0x41

extern u16 USART_RX_STA;

// 函数声明
void Init_Uart7(u32 Bound);

#endif
