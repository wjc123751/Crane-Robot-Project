#include "all_head.h"

void GrabTask(void *pvParameters)
{
		while (1)
		{
				// 打开或关闭气泵
				if (CraneMasterDataSet.target_air_pump.target_air_pump1 == 0)
				{
						GPIO_ResetBits(GPIOF, GPIO_Pin_0);
				}
				else if (CraneMasterDataSet.target_air_pump.target_air_pump1 == 1)
				{
						GPIO_SetBits(GPIOF, GPIO_Pin_0);
				}
				
				if (CraneMasterDataSet.target_air_pump.target_air_pump_left == 0)
				{
						GPIO_ResetBits(GPIOF, GPIO_Pin_1);
				}
				else if (CraneMasterDataSet.target_air_pump.target_air_pump_left == 1)
				{
						GPIO_SetBits(GPIOF, GPIO_Pin_1);
				}
				
				if (CraneMasterDataSet.target_air_pump.target_air_pump_right == 0)
				{
						GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				}
				else if (CraneMasterDataSet.target_air_pump.target_air_pump_right == 1)
				{
						GPIO_SetBits(GPIOE, GPIO_Pin_5);
				}
				
				if (CraneMasterDataSet.target_air_pump.target_air_pump_middle == 0)
				{
						GPIO_ResetBits(GPIOE, GPIO_Pin_4);
				}
				else if (CraneMasterDataSet.target_air_pump.target_air_pump_middle == 1)
				{
						GPIO_SetBits(GPIOE, GPIO_Pin_4);
				}
			

				vTaskDelay(1);
		}
	
}
