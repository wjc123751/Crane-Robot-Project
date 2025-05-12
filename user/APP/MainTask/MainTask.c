#include "all_head.h"

// 物品位置数组
int ObjectResult[6];

// 函数声明
_Bool Decision_Function(u8 USART_RX_BUF[], int length);
void USART_SendString(USART_TypeDef* USARTx, const char* str);
int GM6020Turn(int Rod, int Position);

/*			// 遥控器测试
				get_Rc_Measure_Point(&CraneMasterDataSet.rc_ctrol);
				if(CraneMasterDataSet.rc_ctrol.rc.s1 == 1 && CraneMasterDataSet.rc_ctrol.rc.s2 == 3)
				{
						TIM_SetCompare1(TIM2, Servo1_Suction);
						TIM_SetCompare2(TIM2, Servo3_Right_Suction);
					
						CraneMasterDataSet.target_air_pump.target_air_pump1 = 1;
						CraneMasterDataSet.target_air_pump.target_air_pump_middle = 1;
						CraneMasterDataSet.target_air_pump.target_air_pump_right = 1;
						CraneMasterDataSet.target_air_pump.target_air_pump_left = 1;
				}*/

// 主逻辑任务
void MainTask(void *pvParameters)
	
{
	
		while(1)
		{
				// 一键开始
				if (Key_value == 1)
				{
						// 设置起重机各部分的目标位置
						Motor_Expect.Target_2006_Black = Black_Rack_Ready;//Black_Rack_Ready
						Motor_Expect.Target_2006_White = White_Rack_Ready;//White_Rack_Ready
						Motor_Expect.Target_3508_Black = Black_Gear_Limit_Position;//Black_Gear_Limit_Position
						Motor_Expect.Target_3508_White = White_Gear_Limit_Position;//White_Gear_Limit_Position
						Motor_Expect.Angle_Target_6020 = 0;
						Motor_Expect.Target_Speed_3508_Chassis = 0;
						TIM_SetCompare1(TIM2, Servo1_Suction);
						TIM_SetCompare2(TIM2, Servo3_Suction);
					
						CraneMasterDataSet.CraneStatus = CraneReady;//CraneReady
						Key_value = 2;
						time_flag = 1;

				}
				
				// 准备状态,等待1.5s,向视觉发送"start"指令
				if (CraneMasterDataSet.CraneStatus == CraneReady && FreeRTOSRunTimeTicks >= 3)
				{
						USART_RX_STA = 0;
					
						const char* message = "start";
						USART_SendString(UART7, message);
						CraneMasterDataSet.CraneStatus = CraneSendMessage;
					
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}
				
				// 接收到消息,处理物品位置信息
				if ((USART_RX_STA&0x08) && CraneMasterDataSet.CraneStatus == CraneSendMessage)
				{
						// 判断数据是否正确
						if (Decision_Function(CraneMasterDataSet.usart_rx_buf,6) == 1)
						{
								int j = 0;
								int k = 3;
								// 遍历接收到的数据
								for (int i=0;i<6;i++)
								{
										// 判断吸盘状态
										if (CraneMasterDataSet.usart_rx_buf[i] == '2')
										{
												ObjectResult[j] = i+1; 	
												j++;
										}
										else if (CraneMasterDataSet.usart_rx_buf[i] == '1')
										{
												ObjectResult[k] = i+1;
												k++;
										}
								}
								
								USART_RX_STA = 0;
								
								CraneMasterDataSet.CraneStatus = CraneReceiveMessage;
						}
						else
						{
								// 数据不正确，触发蜂鸣器报警
								buzzer_on(2, 25000);
						}
				}
		
				// 消息数据正确,起重机向前低速运动,单个吸盘向第一个箱子位置旋转
				if (CraneMasterDataSet.CraneStatus == CraneReceiveMessage)
				{
					
						// 设置底盘前进低速
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Forward_Low_Speed;
					
						// 计算吸盘1旋转目标角度并设置期望值
						CraneMasterDataSet.rotate_expect.Rotate_Expect1A = GM6020Turn(1,ObjectResult[0]);
					
						// 设置状态为起重机前进低速状态
						CraneMasterDataSet.CraneStatus = CraneForwardLowSpeed;
				}
				
				// 碰到传感器1(右前),起重机停下,打开两端气泵
				if (CraneMasterDataSet.sensor1 == 0 && CraneMasterDataSet.CraneStatus == CraneForwardLowSpeed)
				{
						// 停止底盘运动
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Stop;
						
						// 打开各端气泵
						CraneMasterDataSet.target_air_pump.target_air_pump1 = 1;
						CraneMasterDataSet.target_air_pump.target_air_pump_middle = 1;
					
						// 设置状态为起重机前进停止状态
						CraneMasterDataSet.CraneStatus = CraneForwardStop;
				}
					
				// 单个吸盘转到第一个箱子位置
				if (CraneMasterDataSet.CraneStatus == CraneForwardStop)
				{
						// 判断吸盘1旋转是否达到目标角度范围
						if(fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect1A-(float)motor_6020[2].total_ecd_out_6020) < 120)
						{
								// 设置状态为吸盘1旋转到位状态
								CraneMasterDataSet.CraneStatus = CraneRotateExpect1A;
							
								// 设置时间标志
								time_flag = 1;
						}
				}

				// 等待2s,单个吸盘向下运动
				if (CraneMasterDataSet.CraneStatus == CraneRotateExpect1A && FreeRTOSRunTimeTicks >= 4)
				{
						// 设置黑色吸盘位置
						Motor_Expect.Target_2006_Black = Black_Rack_Suction_Box1;
					
						// 设置状态为黑色吸盘吸取第一个箱子状态
						CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox1;
					
						// 重置时间标志和 FreeRTOS 运行时间计数
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}
				
				// 单个吸盘到达第一个箱子位置
				if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox1)
				{
						// 判断黑色吸盘是否到达目标位置
						if (fabs((float)Black_Rack_Suction_Box1 - (float)motor_2006[1].total_ecd_out_2006) < 40)
						{
								// 设置状态为黑色吸盘到位状态
								CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox1Wait;
						}
				}
				
				// 等待1秒，单个吸盘抬升到一层摆放高度
				if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox1Wait)
				{
						// 设置时间标志
						time_flag = 1;

						// 判断时间是否满足条件
						if (FreeRTOSRunTimeTicks >= 2)
						{
								// 设置黑色吸盘抬升到一层摆放高度
								Motor_Expect.Target_2006_Black = Black_Rack_Suction_One_Layer;

								// 设置状态为黑色吸盘抬升到一层摆放高度状态
								CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox1_OneLayer;

								// 重置时间标志和 FreeRTOS 运行时间计数
								time_flag = 0;
								FreeRTOSRunTimeTicks = 0;
						}
				}

				// 三个吸盘向第二个箱子旋转
				if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox1_OneLayer)
				{
						// 计算旋转角度并设定期望角度
						CraneMasterDataSet.rotate_expect.Rotate_Expect2C = GM6020Turn(2, ObjectResult[1]);

						// 如果当前角度接近期望角度
						if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2C - (float)motor_6020[2].total_ecd_out_6020) < 100)
						{
								// 切换状态至三个吸盘旋转期望2C状态
								CraneMasterDataSet.CraneStatus = CraneRotateExpect2C;

								// 标记时间标志位
								time_flag = 1;
						}
				}

				// 等待2s,三个吸盘向下运动
				if (CraneMasterDataSet.CraneStatus == CraneRotateExpect2C && FreeRTOSRunTimeTicks >= 4)
				{
						// 设置目标位置使三个吸盘向下运动
						Motor_Expect.Target_2006_White = White_Rack_Suction_Box;

						// 切换状态至三个吸盘白色齿条吸取箱子状态
						CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionBox;

						// 重置时间标志位和FreeRTOS运行时间
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}

				// 三个吸盘到达第二个箱子位置
				if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionBox)
				{
						// 如果三个吸盘的位置接近目标位置
						if (fabs((float)White_Rack_Suction_Box - (float)motor_2006[3].total_ecd_out_2006) < 40)
						{
								// 切换状态至三个吸盘白色齿条吸取箱子等待状态
								CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionBoxWait;
						}
				}

				
				// 等待2s,三个吸盘抬升到一层摆放高度,长杆向第一次摆放位置旋转
				if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionBoxWait)
				{
						time_flag = 1;

						// 如果等待时间超过4个FreeRTOS运行时间单位
						if (FreeRTOSRunTimeTicks >= 4)
						{
								// 设置三个吸盘目标位置，使其抬升到一层摆放高度
								Motor_Expect.Target_2006_White = White_Rack_Suction_One_Layer;

								// 计算长杆旋转角度并设定期望角度
								CraneMasterDataSet.rotate_expect.Rotate_Expect1Done = GM6020Turn(1, 1);

								// 切换状态至三个吸盘白色齿条吸取箱子一层摆放高度状态
								CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionBox_OneLayer;

								// 重置时间标志位和FreeRTOS运行时间
								time_flag = 0;
								FreeRTOSRunTimeTicks = 0;
						}
				}

				// 长杆旋转到第一次摆放位置,摆正舵机,齿轮回到摆放位置
				if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionBox_OneLayer)
				{
						// 如果长杆旋转角度接近期望角度
						if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect1Done - (float)motor_6020[2].total_ecd_out_6020) < 200)
						{
								// 设置底盘为向后高速运动
								Motor_Expect.Target_Speed_3508_Chassis = Chassis_Backward_High_Speed;

								// 舵机摆正
								TIM_SetCompare1(TIM2, Servo1_Placement);
								TIM_SetCompare2(TIM2, Servo3_Placement);

								// 设置底盘、黑色吸盘和白色吸盘的目标位置，使其到达摆放位置
								Motor_Expect.Target_3508_Black = Black_Placement_One;
								Motor_Expect.Target_3508_White = White_Placement_One;

								// 切换状态至摆放阶段一
								CraneMasterDataSet.CraneStatus = CranePlacementOne;
						}
				}

				// 碰到传感器3(左后),起重机减速
				if (CraneMasterDataSet.sensor3 == 0 && CraneMasterDataSet.CraneStatus == CranePlacementOne)
				{
						// 设置底盘为向后低速运动
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Backward_Low_Speed;

						// 切换状态至起重机向后低速状态
						CraneMasterDataSet.CraneStatus = CraneBackwardLowSpeed;
				}

				// 碰到传感器4(右后),起重机停下,两端吸盘向下移动(使摆放更加平稳)
				if (CraneMasterDataSet.sensor4 == 0 && CraneMasterDataSet.CraneStatus == CraneBackwardLowSpeed)
				{
						// 设置底盘停止运动
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Stop;

						// 设置黑色和白色吸盘目标位置，使其向下移动
						Motor_Expect.Target_2006_Black = Black_Rack_Suction_Devolved_One;
						Motor_Expect.Target_2006_White = White_Rack_Suction_Devolved_One;

						// 标记时间标志位
						time_flag = 1;

						// 切换状态至起重机向后停止状态
						CraneMasterDataSet.CraneStatus = CraneBackwardStop;
				}

				
				// 第一秒,关闭气泵;第二秒,两端吸盘抬升,长杆向第三个箱子旋转;
				if (CraneMasterDataSet.CraneStatus == CraneBackwardStop)
				{
						// 在0秒至1秒之间关闭气泵
						if (FreeRTOSRunTimeTicks > 0 && FreeRTOSRunTimeTicks <= 2)
						{
								CraneMasterDataSet.target_air_pump.target_air_pump1 = 0;
								CraneMasterDataSet.target_air_pump.target_air_pump_middle = 0;
						}

						// 在1秒至2秒之间，使两端吸盘抬升，同时控制长杆旋转到第三个箱子位置
						if (FreeRTOSRunTimeTicks > 2 && FreeRTOSRunTimeTicks <= 4)
						{
								// 设置黑色和白色吸盘目标位置，使其抬升
								Motor_Expect.Target_2006_Black = Black_Rack_Suction_Lift1;
								Motor_Expect.Target_2006_White = White_Rack_Suction_Lift1;

								// 计算长杆旋转角度并设定期望角度
								CraneMasterDataSet.rotate_expect.Rotate_Expect1B = GM6020Turn(1, ObjectResult[2]);

								// 切换状态至摆放阶段一完成状态
								CraneMasterDataSet.CraneStatus = CranePlacementOneFinish;

								// 重置时间标志位和FreeRTOS运行时间
								time_flag = 0;
								FreeRTOSRunTimeTicks = 0;
						}
				}

				// 两秒后,起重机向前高速运动,齿轮回到缓冲位置(防止齿轮因撞击机械限位,最终回不到限位位置)
				if (CraneMasterDataSet.CraneStatus == CranePlacementOneFinish)
				{
						// 设置底盘为向前高速运动
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Forward_High_Speed;

						// 设置黑色和白色齿轮的目标位置，使其回到缓冲位置
						Motor_Expect.Target_3508_Black = Black_Gear_Buffer_Position;
						Motor_Expect.Target_3508_White = White_Gear_Buffer_Position;

						// 切换状态至起重机向前高速状态
						CraneMasterDataSet.CraneStatus = CraneForwardHighSpeed;

						// 标记时间标志位
						time_flag = 1;
				}

				// 起重机向前高速运动1.5s后,两端吸盘回到准备状态,齿轮回到限位位置,舵机摆正
				if (CraneMasterDataSet.CraneStatus == CraneForwardHighSpeed && FreeRTOSRunTimeTicks >= 3)
				{
						// 设置黑色和白色吸盘目标位置，使其回到准备状态
						Motor_Expect.Target_2006_Black = Black_Rack_Ready;
						Motor_Expect.Target_2006_White = White_Rack_Ready;

						// 设置黑色和白色齿轮的目标位置，使其回到限位位置
						Motor_Expect.Target_3508_Black = Black_Gear_Limit_Position;
						Motor_Expect.Target_3508_White = White_Gear_Limit_Position;

						// 舵机摆正
						TIM_SetCompare1(TIM2, Servo1_Suction);
						TIM_SetCompare2(TIM2, Servo3_Suction);

						// 切换状态至准备状态1
						CraneMasterDataSet.CraneStatus = CraneReady1;

						// 重置时间标志位和FreeRTOS运行时间
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}

				// 碰到传感器2(左前),起重机减速,打开两端气泵
				if (CraneMasterDataSet.sensor2 == 0 && CraneMasterDataSet.CraneStatus == CraneReady1)
				{
						// 设置底盘为向前低速运动
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Forward_Low_Speed;

						// 打开两端气泵
						CraneMasterDataSet.target_air_pump.target_air_pump1 = 1;
						CraneMasterDataSet.target_air_pump.target_air_pump_middle = 1;

						// 切换状态至起重机向前低速状态
						CraneMasterDataSet.CraneStatus = CraneForwardLowSpeed1;
				}

				// 碰到传感器1(右前),起重机停止
				if (CraneMasterDataSet.sensor1 == 0 && CraneMasterDataSet.CraneStatus == CraneForwardLowSpeed1)
				{
						// 设置底盘停止运动
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Stop;

						// 切换状态至起重机向前停止状态
						CraneMasterDataSet.CraneStatus = CraneForwardStop1;
				}

				// 起重机停止后,长杆转到第三个箱子位置
				if (CraneMasterDataSet.CraneStatus == CraneForwardStop1)
				{
						// 如果长杆旋转角度接近期望角度
						if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect1B - (float)motor_6020[2].total_ecd_out_6020) < 150)
						{
								// 切换状态至长杆旋转到第三个箱子位置状态
								CraneMasterDataSet.CraneStatus = CraneRotate_Expect1B;

								// 标记时间标志位
								time_flag = 1;
						}
				}

				// 等待2s
				if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect1B && FreeRTOSRunTimeTicks >= 4)
				{
						// 切换状态至等待长杆旋转到第三个箱子位置状态
						CraneMasterDataSet.CraneStatus = CraneRotate_Expect1BWait;

						// 重置时间标志位和FreeRTOS运行时间
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}

				// 长杆停在第三个箱子位置
				if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect1BWait)
				{
						// 如果长杆的编码器值变化很小
						if (fabs((float)motor_6020[2].ecd - (float)motor_6020[2].last_ecd) <= 2)
						{
								// 切换状态至长杆停止在第三个箱子位置状态
								CraneMasterDataSet.CraneStatus = CraneRotate_Expect1BStop;

								// 标记时间标志位
								time_flag = 1;
						}
				}

				// 等待1秒，单个吸盘向第三个箱子位置移动
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect1BStop && FreeRTOSRunTimeTicks >= 2)
        {
            // 设置单个吸盘的目标位置为第三个箱子位置
            Motor_Expect.Target_2006_Black = Black_Rack_Suction_Box2;

            // 切换状态至单个吸盘移动到第三个箱子位置状态
            CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox2;

            // 重置时间标志位和计时器
            time_flag = 0;
            FreeRTOSRunTimeTicks = 0;
        }
        
        // 单个吸盘移动到第三个箱子位置
        if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox2)
        {
            // 判断单个吸盘是否达到目标位置
            if (fabs((float)Black_Rack_Suction_Box2 - (float)motor_2006[1].total_ecd_out_2006) < 25)
            {
                // 切换状态至单个吸盘移动到第三个箱子位置等待状态
                CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox2Wait;
            }
        }
        
        // 等待1秒，单个吸盘回到一层摆放高度
        if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox2Wait)
        {
            // 设置时间标志位
            time_flag = 1;
            
            // 在等待时间内，判断是否超过等待时间
            if (FreeRTOSRunTimeTicks >= 2)
            {
                // 设置单个吸盘目标位置为一层摆放高度
                Motor_Expect.Target_2006_Black = Black_Rack_Suction_One_Layer;
                
                // 切换状态至单个吸盘回到一层摆放高度状态
                CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox2OneLayer;
                
                // 重置时间标志位和计时器
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
        
        // 三个吸盘向第三瓶可乐位置移动
        if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox2OneLayer)
        {
            // 计算并设置三个吸盘的旋转目标位置
            CraneMasterDataSet.rotate_expect.Rotate_Expect2D = GM6020Turn(2, ObjectResult[5]);
            
            // 判断吸盘是否达到目标位置
            if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2D - (float)motor_6020[2].total_ecd_out_6020) < 90)
            {
                // 切换状态至三个吸盘旋转到第三瓶可乐位置状态
                CraneMasterDataSet.CraneStatus = CraneRotate_Expect2D;
            }
        }
        
        // 三个吸盘停在第三瓶可乐位置
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2D && fabs((float)motor_6020[2].ecd - (float)motor_6020[2].last_ecd) < 3)
        {
            // 切换状态至三个吸盘停在第三瓶可乐位置状态
            CraneMasterDataSet.CraneStatus = CraneRotate_Expect2DStop;
            
            // 标记时间标志位
            time_flag = 1;
        }
        
        // 等待2秒，三个吸盘向第三瓶可乐位置移动
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2DStop && FreeRTOSRunTimeTicks >= 4)
        {
            // 设置三个吸盘的目标位置为第三瓶可乐位置
            Motor_Expect.Target_2006_White = White_Rack_Suction_Coca_Cola;
            
            // 切换状态至三个吸盘向第三瓶可乐位置移动状态
            CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola3;
            
            // 重置时间标志位和计时器
            time_flag = 0;
            FreeRTOSRunTimeTicks = 0;
        }

        // 三个吸盘移动到第三瓶可乐位置
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola3)
        {
            // 判断三个吸盘是否达到第三瓶可乐位置
            if (fabs((float)White_Rack_Suction_Coca_Cola - (float)motor_2006[3].total_ecd_out_2006) < 25)
            {
                // 切换状态至三个吸盘移动到第三瓶可乐位置等待状态
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola3Wait;
            }
        }
        
        // 等待2秒，三个吸盘回到准备位置
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola3Wait)
        {
            // 设置时间标志位
            time_flag = 1;
            
            // 在等待时间内，判断是否超过等待时间
            if (FreeRTOSRunTimeTicks >= 4)
            {
                // 设置吸盘目标位置为准备位置
                Motor_Expect.Target_2006_White = White_Rack_Ready;
                
                // 切换状态至三个吸盘回到准备位置状态
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola3Finish;
                
                // 重置时间标志位和计时器
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
        
        // 三个吸盘舵机向左旋转，打开左边吸盘气泵，长杆向第二瓶可乐位置移动
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola3Finish)
        {
            // 设置舵机角度，打开左边吸盘气泵
            TIM_SetCompare2(TIM2, Servo3_Left_Suction);
            CraneMasterDataSet.target_air_pump.target_air_pump_left = 1;
            
            // 计算并设置长杆旋转目标位置
            CraneMasterDataSet.rotate_expect.Rotate_Expect2E = GM6020Turn(2, ObjectResult[4]);
            
            // 判断长杆是否达到目标位置
            if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2E - (float)motor_6020[2].total_ecd_out_6020) < 87)
            {
                // 切换状态至长杆旋转到第二瓶可乐位置状态
                CraneMasterDataSet.CraneStatus = CraneRotate_Expect2E;
            }
        }
        
        // 长杆停在第二瓶可乐位置
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2E && fabs((float)motor_6020[2].ecd - (float)motor_6020[2].last_ecd) < 3)
        {
            // 切换状态至长杆停在第二瓶可乐位置状态
            CraneMasterDataSet.CraneStatus = CraneRotate_Expect2EStop;
            
            // 设置时间标志位
            time_flag = 1;
        }
        
        // 等待2秒，三个吸盘向第二瓶可乐位置移动
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2EStop && FreeRTOSRunTimeTicks >= 4)
        {
            // 设置吸盘目标位置为第二瓶可乐位置
            Motor_Expect.Target_2006_White = White_Rack_Suction_Coca_Cola;
            
            // 切换状态至三个吸盘移动到第二瓶可乐位置状态
            CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola2;
            
            // 重置时间标志位和计时器
            time_flag = 0;
            FreeRTOSRunTimeTicks = 0;
        }
        
        // 三个吸盘移动到第二瓶可乐位置
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola2)
        {
            // 判断三个吸盘是否达到第二瓶可乐位置
            if (fabs((float)White_Rack_Suction_Coca_Cola - (float)motor_2006[3].total_ecd_out_2006) < 25)
            {
                // 切换状态至三个吸盘移动到第二瓶可乐位置等待状态
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola2Wait;
            }
        }
        
        // 等待2秒，三个吸盘回到准备位置
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola2Wait)
        {
            // 设置时间标志位
            time_flag = 1;
            
            // 在等待时间内，判断是否超过等待时间
            if (FreeRTOSRunTimeTicks >= 4)
            {
                // 设置吸盘目标位置为准备位置
                Motor_Expect.Target_2006_White = White_Rack_Ready;
                
                // 切换状态至三个吸盘回到准备位置状态
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola2Finish;
                
                // 重置时间标志位和计时器
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
				
				// 三个吸盘舵机向右旋转，打开右边吸盘气泵，长杆向第一瓶可乐位置移动
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola2Finish)
        {
            // 设置舵机角度，打开右边吸盘气泵
            TIM_SetCompare2(TIM2, Servo3_Right_Suction);
            CraneMasterDataSet.target_air_pump.target_air_pump_right = 1;
            
            // 计算并设置长杆旋转目标位置
            CraneMasterDataSet.rotate_expect.Rotate_Expect2F = GM6020Turn(2, ObjectResult[3]);
            
            // 判断长杆是否达到目标位置
            if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2F - (float)motor_6020[2].total_ecd_out_6020) < 72)
            {
                // 切换状态至长杆旋转到第一瓶可乐位置状态
                CraneMasterDataSet.CraneStatus = CraneRotate_Expect2F;
            }
        }
        
        // 长杆停在第一瓶可乐位置
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2F && fabs((float)motor_6020[2].ecd - (float)motor_6020[2].last_ecd) < 2)
        {
            // 切换状态至长杆停在第一瓶可乐位置状态
            CraneMasterDataSet.CraneStatus = CraneRotate_Expect2FStop;
            
            // 设置时间标志位
            time_flag = 1;
        }        
        
        // 等待3秒，三个吸盘向第一瓶可乐位置运动
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2FStop && FreeRTOSRunTimeTicks >= 6)
        {
            // 设置吸盘目标位置为第一瓶可乐位置
            Motor_Expect.Target_2006_White = White_Rack_Suction_Coca_Cola3;
            
            // 切换状态至三个吸盘向第一瓶可乐位置运动状态
            CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola1;
            
            // 重置时间标志位和计时器
            time_flag = 0;
            FreeRTOSRunTimeTicks = 0;
        }
				
				// 三个吸盘运动到第一瓶可乐位置
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola1)
        {
            if (fabs((float)White_Rack_Suction_Coca_Cola3 - (float)motor_2006[3].total_ecd_out_2006) < 25)
            {
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola1Wait;
            }
        }
        
        // 等待2秒，三个吸盘回到一层摆放高度
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola1Wait)
        {
            // 设置时间标志位
            time_flag = 1;
            
            if (FreeRTOSRunTimeTicks >= 4)
            {
                // 设置吸盘目标位置为一层摆放高度
                Motor_Expect.Target_2006_White = White_Rack_Suction_One_Layer;
                
                // 切换状态至三个吸盘回到一层摆放高度完成状态
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola1Finish;
                
                // 重置时间标志位和计时器
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
        
        // 两端吸盘回到二层摆放高度，齿轮回到摆放位置，长杆向第二次摆放位置旋转，
        // 旋转到给定范围后，起重机向后高速运动，摆正舵机
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola1Finish)
        {
            // 设置吸盘和齿轮目标位置
            Motor_Expect.Target_2006_Black = Black_Rack_Suction_Two_Layer;
            Motor_Expect.Target_2006_White = White_Rack_Suction_Two_Layer;
            Motor_Expect.Target_3508_Black = Black_Placement_Two;
            Motor_Expect.Target_3508_White = White_Placement_Two;
            
            // 计算并设置长杆旋转目标位置
            CraneMasterDataSet.rotate_expect.Rotate_Expect2Done = GM6020Turn(2, 1);
            
            // 判断长杆是否旋转到给定范围内
            if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2Done - (float)motor_6020[2].total_ecd_out_6020) < 200)
            {
                // 设置底盘后退高速运动，舵机摆正
                Motor_Expect.Target_Speed_3508_Chassis = Chassis_Backward_High_Speed;
                TIM_SetCompare1(TIM2, Servo1_Placement);
                
                // 切换状态至起重机后退高速运动状态
                CraneMasterDataSet.CraneStatus = CraneBackwardHighSpeed;
            }
        }
				
				// 碰到传感器3(左后)，起重机减速
        if (CraneMasterDataSet.sensor3 == 0 && CraneMasterDataSet.CraneStatus == CraneBackwardHighSpeed)
        {
            // 设置底盘后退低速运动
            Motor_Expect.Target_Speed_3508_Chassis = Chassis_Backward_Low_Speed;
            
            // 切换状态至起重机后退低速运动状态
            CraneMasterDataSet.CraneStatus = CraneBackwardLowSpeed1;
        }
        
        // 碰到传感器4(右后)，起重机停下，两端吸盘向下移动(使摆放更加平稳)
        if (CraneMasterDataSet.sensor4 == 0 && CraneMasterDataSet.CraneStatus == CraneBackwardLowSpeed1)
        {
            // 设置底盘停止运动
            Motor_Expect.Target_Speed_3508_Chassis = Chassis_Stop;
            
            // 设置吸盘目标位置为向下移动位置
            Motor_Expect.Target_2006_Black = Black_Rack_Suction_Devolved_Two;
            Motor_Expect.Target_2006_White = White_Rack_Suction_Devolved_Two;
            
            // 设置时间标志位
            time_flag = 1;
            
            // 切换状态至起重机后退停止状态
            CraneMasterDataSet.CraneStatus = CraneBackwardStop1;
        }
        
        // 起重机停止后，第二秒，关闭所有气泵；第三秒，抬升吸盘(证明起重机离开物品)
        if (CraneMasterDataSet.CraneStatus == CraneBackwardStop1)
        {
            if (FreeRTOSRunTimeTicks >= 3 && FreeRTOSRunTimeTicks <= 4)
            {
                // 关闭所有吸盘气泵
                CraneMasterDataSet.target_air_pump.target_air_pump1 = 0;
                CraneMasterDataSet.target_air_pump.target_air_pump_middle = 0;
                CraneMasterDataSet.target_air_pump.target_air_pump_left = 0;
                CraneMasterDataSet.target_air_pump.target_air_pump_right = 0;
            }
            
            if (FreeRTOSRunTimeTicks > 4 && FreeRTOSRunTimeTicks <= 6)
            {
                // 设置吸盘目标位置为抬升位置
                Motor_Expect.Target_2006_Black = Black_Rack_Suction_Lift2;
                Motor_Expect.Target_2006_White = White_Rack_Suction_Lift2;
                
                // 切换状态至第二次摆放完成状态
                CraneMasterDataSet.CraneStatus = CranePlacementTwoFinish;
                
                // 重置时间标志位和计时器
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
				
				// 延时2毫秒，让出CPU时间片
				vTaskDelay(2);
		}
}

/**
 * @brief 通过 USART 发送字符串
 *
 * 该函数通过指定的 USART 接口发送给定的字符串。遍历字符串中的每个字符，
 * 逐个发送，并在每个字符发送完成后等待确认。
 *
 * @param[in] USARTx USART 接口的指针，如 USART1、USART2 等
 * @param[in] str 要发送的字符串的地址
 */
void USART_SendString(USART_TypeDef* USARTx, const char* str)
{
    // 遍历字符串中的每个字符，并一个一个发送
    for (size_t i = 0; i < strlen(str); i++)
    {
        // 等待数据发送完成
        while (!(USARTx->SR & USART_SR_TXE));
        
        // 将字符发送到串口数据寄存器
        USARTx->DR = (uint16_t)str[i];
        
        // 等待数据发送完成
        while (!(USARTx->SR & USART_SR_TC));
    }
}

/**
 * @brief 决策函数判断
 *
 * 该函数用于判断接收到的 USART 数据是否满足特定条件，返回判断结果。
 *
 * @param[in] USART_RX_BUF USART 接收缓冲区
 * @param[in] length 缓冲区长度
 * @return _Bool 判断结果，1 表示满足条件，0 表示不满足条件
 */
_Bool Decision_Function(u8 USART_RX_BUF[], int length)
{
    int count0 = 0; // 计数器，记录字符 '1' 的数量
    int count1 = 0; // 计数器，记录字符 '2' 的数量

    // 遍历接收缓冲区中的字符
    for (int i = 0; i < length; i++) 
    {
        if (USART_RX_BUF[i] == '1') 
        {
            count0++; // 统计字符 '1' 的数量
        } 
        else if (USART_RX_BUF[i] == '2') 
        {
            count1++; // 统计字符 '2' 的数量
        }
        
        // 判断是否同时出现了3次字符 '1' 和 3次字符 '2'
        if (count0 == 3 && count1 == 3)
        {
            return 1; // 满足条件，返回 1
        }
    }
    
    return 0; // 不满足条件，返回 0
}


/**
 * @brief GM6020 转动设置函数
 *
 * 该函数根据传入的参数设置 GM6020 电机的转动目标角度，并返回设置的目标角度值。
 *
 * @param[in] Rod 电机编号（1 或 2，代表不同的吸盘）
 * @param[in] Position 目标位置编号（1~6，代表不同的目标位置）
 * @return int 设置的 GM6020 电机的目标角度值

 */
int GM6020Turn(int Rod, int Position)
{
    // 根据 Rod 和 Position 的不同组合，设置相应的目标角度值
    if(Rod==1 && Position == 1)	
        Motor_Expect.Angle_Target_6020 = 8163+20;
    if(Rod==1 && Position == 2)		
        Motor_Expect.Angle_Target_6020 = 4864+20;
    if(Rod==1 && Position == 3)	
        Motor_Expect.Angle_Target_6020 = 1596+20;
    if(Rod==1 && Position == 4)	
        Motor_Expect.Angle_Target_6020 = -1712+20;
    if(Rod==1 && Position == 5)	
        Motor_Expect.Angle_Target_6020 = -5012+20;
    if(Rod==1 && Position == 6)	
        Motor_Expect.Angle_Target_6020 = -8215+20;
    if(Rod==2 && Position == 1)	
        Motor_Expect.Angle_Target_6020 = 24675+25;
    if(Rod==2 && Position == 2)	
        Motor_Expect.Angle_Target_6020 = 21429+20;
    if(Rod==2 && Position == 3)	
        Motor_Expect.Angle_Target_6020 = 18084+25;
    if(Rod==2 && Position == 4)	
        Motor_Expect.Angle_Target_6020 = 14803+20;
    if(Rod==2 && Position == 5)	
        Motor_Expect.Angle_Target_6020 = 11490+20;
    if(Rod==2 && Position == 6)	
        Motor_Expect.Angle_Target_6020 = 8171+20;
    
    return Motor_Expect.Angle_Target_6020; // 返回设置的目标角度值
}

