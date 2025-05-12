// 头文件
#include "usart7.h"
#include "main.h"
// 全局变量-接收数据计数-接收数据数组
u16 USART_RX_STA=0;
u8 USART_RX_BUF[6];

// 打印数据函数
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

};

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((UART7->SR&0X40)==0);//循环发送,直到发送完毕   
    UART7->DR = (u8) ch;      
	return ch;
}

#endif 
// 串口7初始化函数，用于vofa打印波形图
void Init_Uart7(u32 Bound)
{
    NVIC_InitTypeDef NVIC_InitStructure;      //定义中断结构体
    GPIO_InitTypeDef GPIO_InitStructure;      //定义IO初始化结构体 
    USART_InitTypeDef USART_InitStructure;    //定义串口结构体

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOE时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7,ENABLE);//使能USART7时钟
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);          //DMA1时钟使能 
    //GPIO引脚复用映射
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_UART7);       //引脚复用映射  PE7 => USART7_RX
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_UART7);      //引脚复用映射 PE8 => USART7_TX
    //GPIO结构体设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;        //管脚指定  PE7  PE8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                  //复用功能   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //推挽复用输出 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //速度50MHz   
    GPIO_Init(GPIOE, &GPIO_InitStructure);                        //初始化       
    //Usart的 NVIC 配置  
    USART_InitStructure.USART_BaudRate = Bound;                                       //波特率设置，一般设置为9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                       //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                            //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                               //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件数据流控制 
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   //收发模式   
    
		USART_Init(UART7,&USART_InitStructure);                                          //初始化串口
    USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);    
    USART_Cmd(UART7, ENABLE);
    
		NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;         //串口中断通道  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //抢占优先级2  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;         //子优先级0  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能  
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化NVIC寄存器
}

u32 Res;
// 串口7中断服务函数
void UART7_IRQHandler(void)
{

	if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(UART7, USART_IT_RXNE);
		
		Res = USART_ReceiveData(UART7);
		
		if((USART_RX_STA&0x08)==0)//接收未完成
		{
				CraneMasterDataSet.usart_rx_buf[USART_RX_STA&0X07]=Res;
				//USART_RX_BUF[USART_RX_STA&0X07] = Res;
				USART_RX_STA++;
			
				if(USART_RX_STA > 5)
				{
						USART_RX_STA = USART_RX_STA | 0x08;
				}
		}
	}
}
