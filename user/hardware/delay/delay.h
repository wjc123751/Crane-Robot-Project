#ifndef DELAY_H
#define DELAY_H

// 头文件
#include "main.h"

// 函数声明
void delay_init(uint32_t TICK_RATE_HZ);
void delay_us(uint16_t nus);
void delay_ms(uint16_t nms);

#endif

