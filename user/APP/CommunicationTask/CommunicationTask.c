#include "all_head.h"

CraneMasterData CraneMasterDataSet;
uint8_t Key;
uint8_t Key_value;

void CommunicationTask(void *pvParameters)
{
	while (1)
	{
			Key = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
		
			if (Key == 1)
			{
					delay_ms(10);
					if (Key == 1)
					{
							Key_value = 1;
					}
			}
		
			// 读取传感器
			CraneMasterDataSet.sensor1 = !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6);
			CraneMasterDataSet.sensor2 = !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12);
			CraneMasterDataSet.sensor3 = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
			CraneMasterDataSet.sensor4 = !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
			//读取IMU
			//	TTL_Hex2Dec();
		
			vTaskDelay(1);
	}
	
}
