// ͷ�ļ�
#include "usart7.h"
#include "main.h"
// ȫ�ֱ���-�������ݼ���-������������
u16 USART_RX_STA=0;
u8 USART_RX_BUF[6];

// ��ӡ���ݺ���
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

};

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((UART7->SR&0X40)==0);//ѭ������,ֱ���������   
    UART7->DR = (u8) ch;      
	return ch;
}

#endif 
// ����7��ʼ������������vofa��ӡ����ͼ
void Init_Uart7(u32 Bound)
{
    NVIC_InitTypeDef NVIC_InitStructure;      //�����жϽṹ��
    GPIO_InitTypeDef GPIO_InitStructure;      //����IO��ʼ���ṹ�� 
    USART_InitTypeDef USART_InitStructure;    //���崮�ڽṹ��

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //ʹ��GPIOEʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7,ENABLE);//ʹ��USART7ʱ��
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);          //DMA1ʱ��ʹ�� 
    //GPIO���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_UART7);       //���Ÿ���ӳ��  PE7 => USART7_RX
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_UART7);      //���Ÿ���ӳ�� PE8 => USART7_TX
    //GPIO�ṹ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;        //�ܽ�ָ��  PE7  PE8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                  //���ù���   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //���츴����� 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //�ٶ�50MHz   
    GPIO_Init(GPIOE, &GPIO_InitStructure);                        //��ʼ��       
    //Usart�� NVIC ����  
    USART_InitStructure.USART_BaudRate = Bound;                                       //���������ã�һ������Ϊ9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                       //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                            //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                               //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������������ 
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   //�շ�ģʽ   
    
		USART_Init(UART7,&USART_InitStructure);                                          //��ʼ������
    USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);    
    USART_Cmd(UART7, ENABLE);
    
		NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;         //�����ж�ͨ��  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //��ռ���ȼ�2  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;         //�����ȼ�0  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��  
    NVIC_Init(&NVIC_InitStructure);                           //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
}

u32 Res;
// ����7�жϷ�����
void UART7_IRQHandler(void)
{

	if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(UART7, USART_IT_RXNE);
		
		Res = USART_ReceiveData(UART7);
		
		if((USART_RX_STA&0x08)==0)//����δ���
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
