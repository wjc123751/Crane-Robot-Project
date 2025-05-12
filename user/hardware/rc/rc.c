// 头文件
#include "rc.h"

// 宏定义
#define SBUS_RX_BUF_NUM		36u
#define RC_FRAME_LENGTH		18u

// 定义全局变量-接收数据数组
uint8_t sbus_rx_buffer[2][SBUS_RX_BUF_NUM];

// 遥控器初始化函数
void RC_Init(void)
{
		/* -------------- Enable Module Clock Source ------------------------*/
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2, ENABLE);
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
				RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
				RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
		/* -------------- Configure GPIO ------------------------------------*/
		{
				GPIO_InitTypeDef GPIO_InitStructure;
				USART_InitTypeDef USART_InitStructure;
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
				GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
				GPIO_Init(GPIOB, &GPIO_InitStructure);

				USART_DeInit(USART1);

				USART_InitStructure.USART_BaudRate = 100000;
				USART_InitStructure.USART_WordLength = USART_WordLength_8b;
				USART_InitStructure.USART_StopBits = USART_StopBits_1;
				USART_InitStructure.USART_Parity = USART_Parity_Even;
				USART_InitStructure.USART_Mode = USART_Mode_Rx;
				USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
				USART_Init(USART1, &USART_InitStructure);

				USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

				USART_ClearFlag(USART1, USART_FLAG_IDLE);
				USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

				USART_Cmd(USART1, ENABLE);
		}
		/* -------------- Configure NVIC ------------------------------------*/
		{
				NVIC_InitTypeDef NVIC_InitStructure;
				NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
		}
		/* -------------- Configure DMA -------------------------------------*/
		{
				DMA_InitTypeDef DMA_InitStructure;
				DMA_DeInit(DMA2_Stream2);

				DMA_InitStructure.DMA_Channel = DMA_Channel_4;
				DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
				DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer[0];
				DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
				DMA_InitStructure.DMA_BufferSize = SBUS_RX_BUF_NUM;
				DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
				DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
				DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
				DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
				DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
				DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
				DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
				DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
				DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
				DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
				DMA_Init(DMA2_Stream2, &DMA_InitStructure);
				DMA_DoubleBufferModeConfig(DMA2_Stream2, (uint32_t)sbus_rx_buffer[1], DMA_Memory_0);
				DMA_DoubleBufferModeCmd(DMA2_Stream2, ENABLE);
				DMA_Cmd(DMA2_Stream2, DISABLE);
				DMA_Cmd(DMA2_Stream2, ENABLE);
		}
}
// 定义全局变量-遥控器数据
RC_Ctl_t RC_CtrlData;
// 遥控器数据处理函数
void RemoteDataProcess(uint8_t *pData)
{
	if(pData == 0)
	{
		return;
	}

	RC_CtrlData.rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;
	RC_CtrlData.rc.ch0 -= 1024;
	RC_CtrlData.rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF;
	RC_CtrlData.rc.ch1 -= 1024;
	RC_CtrlData.rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |((int16_t)pData[4] << 10)) & 0x07FF;
	RC_CtrlData.rc.ch2 -= 1024;
	RC_CtrlData.rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) & 0x07FF;
	RC_CtrlData.rc.ch3 -= 1024;	

	RC_CtrlData.rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;
	RC_CtrlData.rc.s2 = ((pData[5] >> 4) & 0x0003);
	RC_CtrlData.mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
	RC_CtrlData.mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);
	RC_CtrlData.mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);
	RC_CtrlData.mouse.press_l = pData[12];
	RC_CtrlData.mouse.press_r = pData[13];
	RC_CtrlData.key.v = ((int16_t)pData[14]);
	
	RC_CtrlData.rc.ch4 = (((int16_t)pData[16] ) | ((int16_t)pData[17]<<8)) & 0x07FF;
	RC_CtrlData.rc.ch4 -= 1024;
}
//指针获取遥控器数据
void get_Rc_Measure_Point(RC_Ctl_t* RC)
{
		*RC = RC_CtrlData;
}
// 串口1中断服务函数
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ReceiveData(USART1);
    }
    else if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        static uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART1);

        if(DMA_GetCurrentMemoryTarget(DMA2_Stream2) == 0)
        {
            // 重新设置DMA
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA_SetCurrDataCounter(DMA2_Stream2, SBUS_RX_BUF_NUM);
            DMA2_Stream2->CR |= DMA_SxCR_CT;
            // 清DMA中断标志
            DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
            DMA_Cmd(DMA2_Stream2, ENABLE);
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                // 处理遥控器数据
                RemoteDataProcess(*sbus_rx_buffer);
            }
        }
        else
        {
            // 重新设置DMA
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA_SetCurrDataCounter(DMA2_Stream2, SBUS_RX_BUF_NUM);
            DMA2_Stream2->CR &= ~(DMA_SxCR_CT);
            // 清DMA中断标志
            DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
            DMA_Cmd(DMA2_Stream2, ENABLE);
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                // 处理遥控器数据
                RemoteDataProcess(*(1+sbus_rx_buffer));
            }

				}
    }
}
