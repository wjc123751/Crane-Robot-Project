#ifndef _USART7_H
#define _USART7_H

// ͷ�ļ�
#include "stm32f4xx.h"
#include "stdio.h"
#include "math.h"

// �궨��
#define tail 0x41

extern u16 USART_RX_STA;

// ��������
void Init_Uart7(u32 Bound);

#endif
