// 头文件
#include "led.h"
#include "stm32f4xx.h"

// LED配置函数
void led_configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

		// 输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		// 输入模式
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    led_green_off();
    led_red_off();

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_SetBits(GPIOG, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
}

// 绿灯灭函数
void led_green_off(void)
{
    GPIO_SetBits(GPIOF, GPIO_Pin_14);
}
// 绿灯亮函数
void led_green_on(void)
{
    GPIO_ResetBits(GPIOF, GPIO_Pin_14);
}
// 绿灯反转函数
void led_green_toggle(void)
{
    GPIO_ToggleBits(GPIOF, GPIO_Pin_14);
}

// 红灯灭函数
void led_red_off(void)
{
    GPIO_SetBits(GPIOE, GPIO_Pin_11);
}
// 红灯亮函数
void led_red_on(void)
{
    GPIO_ResetBits(GPIOE, GPIO_Pin_11);
}
// 红灯反转函数
void led_red_toggle(void)
{
    GPIO_ToggleBits(GPIOE, GPIO_Pin_11);
}

// 单个流水灯亮函数
void flow_led_on(uint16_t GPIO_Pin_x)
{
    GPIO_ResetBits(GPIOG, GPIO_Pin_x);
}
// 单个流水灯灭函数
void flow_led_off(uint16_t GPIO_Pin_x)
{
    GPIO_SetBits(GPIOG, GPIO_Pin_x);
}
// 单个流水灯反转函数
void flow_led_toggle(uint16_t GPIO_Pin_x)
{
    GPIO_ToggleBits(GPIOG, GPIO_Pin_x);
}

// 所有流水灯亮函数
void all_flow_led_on(void)
{
		GPIO_ResetBits(GPIOG, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}
// 所有流水灯灭函数
void all_flow_led_off(void)
{
		GPIO_SetBits(GPIOG, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}
// 所有流水灯反转函数
void all_flow_led_toggle(void)
{
		GPIO_ToggleBits(GPIOG, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}

