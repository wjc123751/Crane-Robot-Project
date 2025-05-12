#include "usart8.h"
SEND_DATA Send_Data;
//RECEIVE_DATA Receive_Data;
//*******************************/
IMUData_Packet_t IMUData_Packet;
AHRSData_Packet_t AHRSData_Packet;

u8 ttl_receive;
u8 Fd_data[64];//串口接收数据缓存数组
u8 Fd_rsimu[64];
u8 Fd_rsahrs[56];
int rs_imutype =0;
int rs_ahrstype =0;
extern int Time_count;


/**************************************************************************
Function: Serial port 8 initialization
Input   : none
Output  : none
函数功能：串口8初始化
入口参数：无
返回  值：无
**************************************************************************/
//void uart5_init(u32 bound)
//{  	 
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	//PC12 TX
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	 //Enable the gpio clock  //使能GPIO时钟
//	//PD2 RX
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	 //Enable the gpio clock  //使能GPIO时钟
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //Enable the Usart clock //使能USART时钟

//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);	
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2 ,GPIO_AF_UART5);	 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //输出模式
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //推挽输出
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //高速50MHZ
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //上拉
//	GPIO_Init(GPIOC, &GPIO_InitStructure);  		          //初始化

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //输出模式
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //推挽输出
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //高速50MHZ
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //上拉
//	GPIO_Init(GPIOD, &GPIO_InitStructure);  		          //初始化
//	
//  //UsartNVIC configuration //UsartNVIC配置
//  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//	//Preempt priority //抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
//	//Preempt priority //抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
//	//Enable the IRQ channel //IRQ通道使能	
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
//  //Initialize the VIC register with the specified parameters 
//	//根据指定的参数初始化VIC寄存器		
//	NVIC_Init(&NVIC_InitStructure);
//	
//  //USART Initialization Settings 初始化设置
//	USART_InitStructure.USART_BaudRate = bound; //Port rate //串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //The word length is 8 bit data format //字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; //A stop bit //一个停止
//	USART_InitStructure.USART_Parity = USART_Parity_No; //Prosaic parity bits //无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //No hardware data flow control //无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//Sending and receiving mode //收发模式
//  USART_Init(UART5, &USART_InitStructure);      //Initialize serial port 5 //初始化串口5
//  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); //Open the serial port to accept interrupts //开启串口接受中断
//  USART_Cmd(UART5, ENABLE);                     //Enable serial port 5 //使能串口5
//}

void uart8_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOE时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8,ENABLE);//使能UART8时钟
 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_UART8); //GPIOE0复用为UART8
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_UART8); //GPIOE1复用为UART8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 ; //GPIOE0与GPIOGE1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化PE0,PE1
	
	USART_InitStructure.USART_BaudRate = 921600;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART8, &USART_InitStructure); //初始化串口8
	
	USART_Cmd(UART8, ENABLE);  //使能串口68
	
//	USART_ClearFlag(UART8, USART_FLAG_TC);
		
	USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);//开启相关中断

	NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;//串口8中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

/**************************************************************************
Function: Serial port 5 sends data
Input   : none
Output  : none
函数功能：串口5发送数据
入口参数：无
返回  值：无
**************************************************************************/
void USART5_SEND(void)
{
  unsigned char i = 0;	
	for(i=0; i<24; i++)
	{
		usart5_send(Send_Data.buffer[i]);
	}	 
}


/**************************************************************************
Function: Serial port 8 receives interrupted
Input   : none
Output  : none
函数功能：串口5接收中断
入口参数：无
返回  值：无
**************************************************************************/
void UART8_IRQHandler(void)
{
	static u8 Count=0;
  static u8 last_rsnum=0;
	u8 Usart_Receive;
	static u8 rsimu_flag=0;
	static u8 rsacc_flag=0;

	ttl_receive=1;
	if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET) //Check if data is received //判断是否接收到数据
	{
		Usart_Receive = USART_ReceiveData(UART8);//Read the data //读取数据
		Fd_data[Count]=Usart_Receive;  //串口数据填入数组
		//usart1_send(Usart_Receive);
    if(((last_rsnum==FRAME_END)&&(Usart_Receive == FRAME_HEAD))||Count>0)//帧头帧尾校验
		{
			Count++; 
			if((Fd_data[1]==TYPE_IMU)&&(Fd_data[2]==IMU_LEN))
				rsimu_flag=1;
			if((Fd_data[1]==TYPE_AHRS)&&(Fd_data[2]==AHRS_LEN))
				rsacc_flag=1;
		}
		else 
			Count=0;
		last_rsnum=Usart_Receive;
		if(rsimu_flag==1 && Count==IMU_RS) //将本帧数据保存至Fd_rsimu数组中
		{
			Count=0;
			rsimu_flag=0;
			rs_imutype=1;
			if(Fd_data[IMU_RS-1]==FRAME_END) //帧尾校验
			memcpy(Fd_rsimu, Fd_data, sizeof(Fd_data));       
		}	
		if(rsacc_flag==1 && Count==AHRS_RS) //
		{
			Count=0;
			rsacc_flag=0;
			rs_ahrstype=1;
			if(Fd_data[AHRS_RS-1]==FRAME_END)
			memcpy(Fd_rsahrs, Fd_data, sizeof(Fd_data));
			for(int i=0;i<sizeof(Fd_data);i++) Fd_data[i]=0;

		}
	}
}


/*************
实现16进制的can数据转换成浮点型数据
****************/
float DATA_Trans(u8 Data_1,u8 Data_2,u8 Data_3,u8 Data_4)
{
  u32 transition_32;
	float tmp=0;
	int sign=0;
	int exponent=0;
	float mantissa=0;
  transition_32 = 0;
  transition_32 |=  Data_4<<24;   
  transition_32 |=  Data_3<<16; 
	transition_32 |=  Data_2<<8;
	transition_32 |=  Data_1;
  sign = (transition_32 & 0x80000000) ? -1 : 1;//符号位
	//先右移操作，再按位与计算，出来结果是30到23位对应的e
	exponent = ((transition_32 >> 23) & 0xff) - 127;
	//将22~0转化为10进制，得到对应的x系数 
	mantissa = 1 + ((float)(transition_32 & 0x7fffff) / 0x7fffff);
	tmp=sign * mantissa * pow(2, exponent);
	return tmp;
}
long long timestamp(u8 Data_1,u8 Data_2,u8 Data_3,u8 Data_4)
{
  u32 transition_32;
  transition_32 = 0;
  transition_32 |=  Data_4<<24;   
  transition_32 |=  Data_3<<16; 
	transition_32 |=  Data_2<<8;
	transition_32 |=  Data_1;
	return transition_32;
}

/*************
实现16进制的can数据转换成浮点型数据
****************/
float Data_Trans(u8 Data_1,u8 Data_2,u8 Data_3,u8 Data_4)
{
  long long transition_32;
	float tmp=0;
	int sign=0;
	int exponent=0;
	float mantissa=0;
  transition_32 = 0;
  transition_32 |=  Data_4<<24;   
  transition_32 |=  Data_3<<16; 
	transition_32 |=  Data_2<<8;
	transition_32 |=  Data_1;
  sign = (transition_32 & 0x80000000) ? -1 : 1;//符号位
	//先右移操作，再按位与计算，出来结果是30到23位对应的e
	exponent = ((transition_32 >> 23) & 0xff) - 127;
	//将22~0转化为10进制，得到对应的x系数 
	mantissa = 1 + ((float)(transition_32 & 0x7fffff) / 0x7fffff);
	tmp=sign * mantissa * pow(2, exponent);
	return tmp;
}

void AHRSData2PC(void)
{
//	 printf("**********                **********\r\n");	
//	
// 	 printf("AHRS: The RollSpeed =  %f\r\n",AHRSData_Packet.RollSpeed);
//	 printf("AHRS: The PitchSpeed =  %f\r\n",AHRSData_Packet.PitchSpeed);
		 //printf("AHRS: The HeadingSpeed =  %f\r\n",AHRSData_Packet.HeadingSpeed);
//   printf("AHRS: The Roll =  %f\r\n",AHRSData_Packet.Roll);
//   printf("AHRS: The Pitch =  %f\r\n",AHRSData_Packet.Pitch);
//		 printf("AHRS: The Heading =  %f\r\n",AHRSData_Packet.Heading);
//   printf("AHRS: The Quaternion.Qw =  %f\r\n",AHRSData_Packet.Qw);
//   printf("AHRS: The Quaternion.Qx =  %f\r\n",AHRSData_Packet.Qx);
//   printf("AHRS: The Quaternion.Qy =  %f\r\n",AHRSData_Packet.Qy);
//   printf("AHRS: The Quaternion.Qz =  %f\r\n",AHRSData_Packet.Qz);
//   printf("AHRS: The Timestamp =  %d\r\n",AHRSData_Packet.Timestamp);
//	  printf("**********                **********\r\n");	
	
}
void IMUData2PC(void)
{
   //printf("Now start sending IMU data.\r\n");
//	 printf("**********                **********\r\n");	
//	 printf("IMU: The gyroscope_x =  %f\r\n",IMUData_Packet.gyroscope_x);
//	 printf("IMU:The gyroscope_y =  %f\r\n",IMUData_Packet.gyroscope_y);
//   printf("IMU:The gyroscope_z =  %f\r\n",IMUData_Packet.gyroscope_z);
//   printf("IMU:The accelerometer_x =  %f\r\n",IMUData_Packet.accelerometer_x);
//   printf("IMU:The accelerometer_y =  %f\r\n",IMUData_Packet.accelerometer_y);
//   printf("IMU:The accelerometer_z =  %f\r\n",IMUData_Packet.accelerometer_z);
//   printf("IMU:The magnetometer_x =  %f\r\n",IMUData_Packet.magnetometer_x);
//   printf("IMU:The magnetometer_y =  %f\r\n",IMUData_Packet.magnetometer_y);
//   printf("IMU:The magnetometer_z =  %f\r\n",IMUData_Packet.magnetometer_z);
//   printf("IMU:The Timestamp =  %d\r\n",IMUData_Packet.Timestamp);
//	 //printf("Now the data of IMU has been sent.\r\n");
//   printf("**********                **********\r\n");	

}

///*******************************
//16进制转浮点型数据

u8 TTL_Hex2Dec(void)  
{
	 if(rs_ahrstype==1)
	{
		if(Fd_rsahrs[1]==TYPE_AHRS&&Fd_rsahrs[2]==AHRS_LEN)
		{	
			AHRSData_Packet.RollSpeed=DATA_Trans(Fd_rsahrs[7],Fd_rsahrs[8],Fd_rsahrs[9],Fd_rsahrs[10]);       //横滚角速度
			AHRSData_Packet.PitchSpeed=DATA_Trans(Fd_rsahrs[11],Fd_rsahrs[12],Fd_rsahrs[13],Fd_rsahrs[14]);   //俯仰角速度
			AHRSData_Packet.HeadingSpeed=DATA_Trans(Fd_rsahrs[15],Fd_rsahrs[16],Fd_rsahrs[17],Fd_rsahrs[18]); //偏航角速度
				
			AHRSData_Packet.Roll=DATA_Trans(Fd_rsahrs[19],Fd_rsahrs[20],Fd_rsahrs[21],Fd_rsahrs[22]);      //横滚角
			AHRSData_Packet.Pitch=DATA_Trans(Fd_rsahrs[23],Fd_rsahrs[24],Fd_rsahrs[25],Fd_rsahrs[26]);     //俯仰角
			AHRSData_Packet.Heading=DATA_Trans(Fd_rsahrs[27],Fd_rsahrs[28],Fd_rsahrs[29],Fd_rsahrs[30]);	 //偏航角
				
			AHRSData_Packet.Qw=DATA_Trans(Fd_rsahrs[31],Fd_rsahrs[32],Fd_rsahrs[33],Fd_rsahrs[34]);  //四元数
			AHRSData_Packet.Qx=DATA_Trans(Fd_rsahrs[35],Fd_rsahrs[36],Fd_rsahrs[37],Fd_rsahrs[38]);
			AHRSData_Packet.Qy=DATA_Trans(Fd_rsahrs[39],Fd_rsahrs[40],Fd_rsahrs[41],Fd_rsahrs[42]);
			AHRSData_Packet.Qz=DATA_Trans(Fd_rsahrs[43],Fd_rsahrs[44],Fd_rsahrs[45],Fd_rsahrs[46]);
			AHRSData_Packet.Timestamp=timestamp(Fd_rsahrs[47],Fd_rsahrs[48],Fd_rsahrs[49],Fd_rsahrs[50]);   //时间戳
			AHRSData2PC();
		}
	rs_ahrstype=0;
 }
	if(rs_imutype==1)
	{
		if(Fd_rsimu[1]==TYPE_IMU&&Fd_rsimu[2]==IMU_LEN)
		{
			IMUData_Packet.gyroscope_x=DATA_Trans(Fd_rsimu[7],Fd_rsimu[8],Fd_rsimu[9],Fd_rsimu[10]);  //角速度
			IMUData_Packet.gyroscope_y=DATA_Trans(Fd_rsimu[11],Fd_rsimu[12],Fd_rsimu[13],Fd_rsimu[14]);
			IMUData_Packet.gyroscope_z=DATA_Trans(Fd_rsimu[15],Fd_rsimu[16],Fd_rsimu[17],Fd_rsimu[18]);
				
			IMUData_Packet.accelerometer_x=DATA_Trans(Fd_rsimu[19],Fd_rsimu[20],Fd_rsimu[21],Fd_rsimu[22]);  //线加速度
			IMUData_Packet.accelerometer_y=DATA_Trans(Fd_rsimu[23],Fd_rsimu[24],Fd_rsimu[25],Fd_rsimu[26]);
			IMUData_Packet.accelerometer_z=DATA_Trans(Fd_rsimu[27],Fd_rsimu[28],Fd_rsimu[29],Fd_rsimu[30]);

			IMUData_Packet.magnetometer_x=DATA_Trans(Fd_rsimu[31],Fd_rsimu[32],Fd_rsimu[33],Fd_rsimu[34]);  //磁力计数据
			IMUData_Packet.magnetometer_y=DATA_Trans(Fd_rsimu[35],Fd_rsimu[36],Fd_rsimu[37],Fd_rsimu[38]);
			IMUData_Packet.magnetometer_z=DATA_Trans(Fd_rsimu[39],Fd_rsimu[40],Fd_rsimu[41],Fd_rsimu[42]);
				
			IMUData_Packet.Timestamp=timestamp(Fd_rsimu[55],Fd_rsimu[56],Fd_rsimu[57],Fd_rsimu[58]);   //时间戳
			IMUData2PC();
		}
		rs_imutype=0;
 }
return 0;
}

/**************************************************************************
Function: Serial port 5 sends data
Input   : The data to send
Output  : none
函数功能：串口5发送数据
入口参数：要发送的数据
返回  值：无
**************************************************************************/
void usart5_send(u8 data)
{
	UART5->DR = data;
	while((UART5->SR&0x40)==0);	
}







