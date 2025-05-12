#include "all_head.h"

// ��Ʒλ������
int ObjectResult[6];

// ��������
_Bool Decision_Function(u8 USART_RX_BUF[], int length);
void USART_SendString(USART_TypeDef* USARTx, const char* str);
int GM6020Turn(int Rod, int Position);

/*			// ң��������
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

// ���߼�����
void MainTask(void *pvParameters)
	
{
	
		while(1)
		{
				// һ����ʼ
				if (Key_value == 1)
				{
						// �������ػ������ֵ�Ŀ��λ��
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
				
				// ׼��״̬,�ȴ�1.5s,���Ӿ�����"start"ָ��
				if (CraneMasterDataSet.CraneStatus == CraneReady && FreeRTOSRunTimeTicks >= 3)
				{
						USART_RX_STA = 0;
					
						const char* message = "start";
						USART_SendString(UART7, message);
						CraneMasterDataSet.CraneStatus = CraneSendMessage;
					
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}
				
				// ���յ���Ϣ,������Ʒλ����Ϣ
				if ((USART_RX_STA&0x08) && CraneMasterDataSet.CraneStatus == CraneSendMessage)
				{
						// �ж������Ƿ���ȷ
						if (Decision_Function(CraneMasterDataSet.usart_rx_buf,6) == 1)
						{
								int j = 0;
								int k = 3;
								// �������յ�������
								for (int i=0;i<6;i++)
								{
										// �ж�����״̬
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
								// ���ݲ���ȷ����������������
								buzzer_on(2, 25000);
						}
				}
		
				// ��Ϣ������ȷ,���ػ���ǰ�����˶�,�����������һ������λ����ת
				if (CraneMasterDataSet.CraneStatus == CraneReceiveMessage)
				{
					
						// ���õ���ǰ������
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Forward_Low_Speed;
					
						// ��������1��תĿ��ǶȲ���������ֵ
						CraneMasterDataSet.rotate_expect.Rotate_Expect1A = GM6020Turn(1,ObjectResult[0]);
					
						// ����״̬Ϊ���ػ�ǰ������״̬
						CraneMasterDataSet.CraneStatus = CraneForwardLowSpeed;
				}
				
				// ����������1(��ǰ),���ػ�ͣ��,����������
				if (CraneMasterDataSet.sensor1 == 0 && CraneMasterDataSet.CraneStatus == CraneForwardLowSpeed)
				{
						// ֹͣ�����˶�
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Stop;
						
						// �򿪸�������
						CraneMasterDataSet.target_air_pump.target_air_pump1 = 1;
						CraneMasterDataSet.target_air_pump.target_air_pump_middle = 1;
					
						// ����״̬Ϊ���ػ�ǰ��ֹͣ״̬
						CraneMasterDataSet.CraneStatus = CraneForwardStop;
				}
					
				// ��������ת����һ������λ��
				if (CraneMasterDataSet.CraneStatus == CraneForwardStop)
				{
						// �ж�����1��ת�Ƿ�ﵽĿ��Ƕȷ�Χ
						if(fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect1A-(float)motor_6020[2].total_ecd_out_6020) < 120)
						{
								// ����״̬Ϊ����1��ת��λ״̬
								CraneMasterDataSet.CraneStatus = CraneRotateExpect1A;
							
								// ����ʱ���־
								time_flag = 1;
						}
				}

				// �ȴ�2s,�������������˶�
				if (CraneMasterDataSet.CraneStatus == CraneRotateExpect1A && FreeRTOSRunTimeTicks >= 4)
				{
						// ���ú�ɫ����λ��
						Motor_Expect.Target_2006_Black = Black_Rack_Suction_Box1;
					
						// ����״̬Ϊ��ɫ������ȡ��һ������״̬
						CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox1;
					
						// ����ʱ���־�� FreeRTOS ����ʱ�����
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}
				
				// �������̵����һ������λ��
				if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox1)
				{
						// �жϺ�ɫ�����Ƿ񵽴�Ŀ��λ��
						if (fabs((float)Black_Rack_Suction_Box1 - (float)motor_2006[1].total_ecd_out_2006) < 40)
						{
								// ����״̬Ϊ��ɫ���̵�λ״̬
								CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox1Wait;
						}
				}
				
				// �ȴ�1�룬��������̧����һ��ڷŸ߶�
				if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox1Wait)
				{
						// ����ʱ���־
						time_flag = 1;

						// �ж�ʱ���Ƿ���������
						if (FreeRTOSRunTimeTicks >= 2)
						{
								// ���ú�ɫ����̧����һ��ڷŸ߶�
								Motor_Expect.Target_2006_Black = Black_Rack_Suction_One_Layer;

								// ����״̬Ϊ��ɫ����̧����һ��ڷŸ߶�״̬
								CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox1_OneLayer;

								// ����ʱ���־�� FreeRTOS ����ʱ�����
								time_flag = 0;
								FreeRTOSRunTimeTicks = 0;
						}
				}

				// ����������ڶ���������ת
				if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox1_OneLayer)
				{
						// ������ת�ǶȲ��趨�����Ƕ�
						CraneMasterDataSet.rotate_expect.Rotate_Expect2C = GM6020Turn(2, ObjectResult[1]);

						// �����ǰ�ǶȽӽ������Ƕ�
						if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2C - (float)motor_6020[2].total_ecd_out_6020) < 100)
						{
								// �л�״̬������������ת����2C״̬
								CraneMasterDataSet.CraneStatus = CraneRotateExpect2C;

								// ���ʱ���־λ
								time_flag = 1;
						}
				}

				// �ȴ�2s,�������������˶�
				if (CraneMasterDataSet.CraneStatus == CraneRotateExpect2C && FreeRTOSRunTimeTicks >= 4)
				{
						// ����Ŀ��λ��ʹ�������������˶�
						Motor_Expect.Target_2006_White = White_Rack_Suction_Box;

						// �л�״̬���������̰�ɫ������ȡ����״̬
						CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionBox;

						// ����ʱ���־λ��FreeRTOS����ʱ��
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}

				// �������̵���ڶ�������λ��
				if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionBox)
				{
						// ����������̵�λ�ýӽ�Ŀ��λ��
						if (fabs((float)White_Rack_Suction_Box - (float)motor_2006[3].total_ecd_out_2006) < 40)
						{
								// �л�״̬���������̰�ɫ������ȡ���ӵȴ�״̬
								CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionBoxWait;
						}
				}

				
				// �ȴ�2s,��������̧����һ��ڷŸ߶�,�������һ�ΰڷ�λ����ת
				if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionBoxWait)
				{
						time_flag = 1;

						// ����ȴ�ʱ�䳬��4��FreeRTOS����ʱ�䵥λ
						if (FreeRTOSRunTimeTicks >= 4)
						{
								// ������������Ŀ��λ�ã�ʹ��̧����һ��ڷŸ߶�
								Motor_Expect.Target_2006_White = White_Rack_Suction_One_Layer;

								// ���㳤����ת�ǶȲ��趨�����Ƕ�
								CraneMasterDataSet.rotate_expect.Rotate_Expect1Done = GM6020Turn(1, 1);

								// �л�״̬���������̰�ɫ������ȡ����һ��ڷŸ߶�״̬
								CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionBox_OneLayer;

								// ����ʱ���־λ��FreeRTOS����ʱ��
								time_flag = 0;
								FreeRTOSRunTimeTicks = 0;
						}
				}

				// ������ת����һ�ΰڷ�λ��,�������,���ֻص��ڷ�λ��
				if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionBox_OneLayer)
				{
						// ���������ת�ǶȽӽ������Ƕ�
						if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect1Done - (float)motor_6020[2].total_ecd_out_6020) < 200)
						{
								// ���õ���Ϊ�������˶�
								Motor_Expect.Target_Speed_3508_Chassis = Chassis_Backward_High_Speed;

								// �������
								TIM_SetCompare1(TIM2, Servo1_Placement);
								TIM_SetCompare2(TIM2, Servo3_Placement);

								// ���õ��̡���ɫ���̺Ͱ�ɫ���̵�Ŀ��λ�ã�ʹ�䵽��ڷ�λ��
								Motor_Expect.Target_3508_Black = Black_Placement_One;
								Motor_Expect.Target_3508_White = White_Placement_One;

								// �л�״̬���ڷŽ׶�һ
								CraneMasterDataSet.CraneStatus = CranePlacementOne;
						}
				}

				// ����������3(���),���ػ�����
				if (CraneMasterDataSet.sensor3 == 0 && CraneMasterDataSet.CraneStatus == CranePlacementOne)
				{
						// ���õ���Ϊ�������˶�
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Backward_Low_Speed;

						// �л�״̬�����ػ�������״̬
						CraneMasterDataSet.CraneStatus = CraneBackwardLowSpeed;
				}

				// ����������4(�Һ�),���ػ�ͣ��,�������������ƶ�(ʹ�ڷŸ���ƽ��)
				if (CraneMasterDataSet.sensor4 == 0 && CraneMasterDataSet.CraneStatus == CraneBackwardLowSpeed)
				{
						// ���õ���ֹͣ�˶�
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Stop;

						// ���ú�ɫ�Ͱ�ɫ����Ŀ��λ�ã�ʹ�������ƶ�
						Motor_Expect.Target_2006_Black = Black_Rack_Suction_Devolved_One;
						Motor_Expect.Target_2006_White = White_Rack_Suction_Devolved_One;

						// ���ʱ���־λ
						time_flag = 1;

						// �л�״̬�����ػ����ֹͣ״̬
						CraneMasterDataSet.CraneStatus = CraneBackwardStop;
				}

				
				// ��һ��,�ر�����;�ڶ���,��������̧��,�����������������ת;
				if (CraneMasterDataSet.CraneStatus == CraneBackwardStop)
				{
						// ��0����1��֮��ر�����
						if (FreeRTOSRunTimeTicks > 0 && FreeRTOSRunTimeTicks <= 2)
						{
								CraneMasterDataSet.target_air_pump.target_air_pump1 = 0;
								CraneMasterDataSet.target_air_pump.target_air_pump_middle = 0;
						}

						// ��1����2��֮�䣬ʹ��������̧����ͬʱ���Ƴ�����ת������������λ��
						if (FreeRTOSRunTimeTicks > 2 && FreeRTOSRunTimeTicks <= 4)
						{
								// ���ú�ɫ�Ͱ�ɫ����Ŀ��λ�ã�ʹ��̧��
								Motor_Expect.Target_2006_Black = Black_Rack_Suction_Lift1;
								Motor_Expect.Target_2006_White = White_Rack_Suction_Lift1;

								// ���㳤����ת�ǶȲ��趨�����Ƕ�
								CraneMasterDataSet.rotate_expect.Rotate_Expect1B = GM6020Turn(1, ObjectResult[2]);

								// �л�״̬���ڷŽ׶�һ���״̬
								CraneMasterDataSet.CraneStatus = CranePlacementOneFinish;

								// ����ʱ���־λ��FreeRTOS����ʱ��
								time_flag = 0;
								FreeRTOSRunTimeTicks = 0;
						}
				}

				// �����,���ػ���ǰ�����˶�,���ֻص�����λ��(��ֹ������ײ����е��λ,���ջز�����λλ��)
				if (CraneMasterDataSet.CraneStatus == CranePlacementOneFinish)
				{
						// ���õ���Ϊ��ǰ�����˶�
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Forward_High_Speed;

						// ���ú�ɫ�Ͱ�ɫ���ֵ�Ŀ��λ�ã�ʹ��ص�����λ��
						Motor_Expect.Target_3508_Black = Black_Gear_Buffer_Position;
						Motor_Expect.Target_3508_White = White_Gear_Buffer_Position;

						// �л�״̬�����ػ���ǰ����״̬
						CraneMasterDataSet.CraneStatus = CraneForwardHighSpeed;

						// ���ʱ���־λ
						time_flag = 1;
				}

				// ���ػ���ǰ�����˶�1.5s��,�������̻ص�׼��״̬,���ֻص���λλ��,�������
				if (CraneMasterDataSet.CraneStatus == CraneForwardHighSpeed && FreeRTOSRunTimeTicks >= 3)
				{
						// ���ú�ɫ�Ͱ�ɫ����Ŀ��λ�ã�ʹ��ص�׼��״̬
						Motor_Expect.Target_2006_Black = Black_Rack_Ready;
						Motor_Expect.Target_2006_White = White_Rack_Ready;

						// ���ú�ɫ�Ͱ�ɫ���ֵ�Ŀ��λ�ã�ʹ��ص���λλ��
						Motor_Expect.Target_3508_Black = Black_Gear_Limit_Position;
						Motor_Expect.Target_3508_White = White_Gear_Limit_Position;

						// �������
						TIM_SetCompare1(TIM2, Servo1_Suction);
						TIM_SetCompare2(TIM2, Servo3_Suction);

						// �л�״̬��׼��״̬1
						CraneMasterDataSet.CraneStatus = CraneReady1;

						// ����ʱ���־λ��FreeRTOS����ʱ��
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}

				// ����������2(��ǰ),���ػ�����,����������
				if (CraneMasterDataSet.sensor2 == 0 && CraneMasterDataSet.CraneStatus == CraneReady1)
				{
						// ���õ���Ϊ��ǰ�����˶�
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Forward_Low_Speed;

						// ����������
						CraneMasterDataSet.target_air_pump.target_air_pump1 = 1;
						CraneMasterDataSet.target_air_pump.target_air_pump_middle = 1;

						// �л�״̬�����ػ���ǰ����״̬
						CraneMasterDataSet.CraneStatus = CraneForwardLowSpeed1;
				}

				// ����������1(��ǰ),���ػ�ֹͣ
				if (CraneMasterDataSet.sensor1 == 0 && CraneMasterDataSet.CraneStatus == CraneForwardLowSpeed1)
				{
						// ���õ���ֹͣ�˶�
						Motor_Expect.Target_Speed_3508_Chassis = Chassis_Stop;

						// �л�״̬�����ػ���ǰֹͣ״̬
						CraneMasterDataSet.CraneStatus = CraneForwardStop1;
				}

				// ���ػ�ֹͣ��,����ת������������λ��
				if (CraneMasterDataSet.CraneStatus == CraneForwardStop1)
				{
						// ���������ת�ǶȽӽ������Ƕ�
						if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect1B - (float)motor_6020[2].total_ecd_out_6020) < 150)
						{
								// �л�״̬��������ת������������λ��״̬
								CraneMasterDataSet.CraneStatus = CraneRotate_Expect1B;

								// ���ʱ���־λ
								time_flag = 1;
						}
				}

				// �ȴ�2s
				if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect1B && FreeRTOSRunTimeTicks >= 4)
				{
						// �л�״̬���ȴ�������ת������������λ��״̬
						CraneMasterDataSet.CraneStatus = CraneRotate_Expect1BWait;

						// ����ʱ���־λ��FreeRTOS����ʱ��
						time_flag = 0;
						FreeRTOSRunTimeTicks = 0;
				}

				// ����ͣ�ڵ���������λ��
				if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect1BWait)
				{
						// ������˵ı�����ֵ�仯��С
						if (fabs((float)motor_6020[2].ecd - (float)motor_6020[2].last_ecd) <= 2)
						{
								// �л�״̬������ֹͣ�ڵ���������λ��״̬
								CraneMasterDataSet.CraneStatus = CraneRotate_Expect1BStop;

								// ���ʱ���־λ
								time_flag = 1;
						}
				}

				// �ȴ�1�룬�������������������λ���ƶ�
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect1BStop && FreeRTOSRunTimeTicks >= 2)
        {
            // ���õ������̵�Ŀ��λ��Ϊ����������λ��
            Motor_Expect.Target_2006_Black = Black_Rack_Suction_Box2;

            // �л�״̬�����������ƶ�������������λ��״̬
            CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox2;

            // ����ʱ���־λ�ͼ�ʱ��
            time_flag = 0;
            FreeRTOSRunTimeTicks = 0;
        }
        
        // ���������ƶ�������������λ��
        if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox2)
        {
            // �жϵ��������Ƿ�ﵽĿ��λ��
            if (fabs((float)Black_Rack_Suction_Box2 - (float)motor_2006[1].total_ecd_out_2006) < 25)
            {
                // �л�״̬�����������ƶ�������������λ�õȴ�״̬
                CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox2Wait;
            }
        }
        
        // �ȴ�1�룬�������̻ص�һ��ڷŸ߶�
        if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox2Wait)
        {
            // ����ʱ���־λ
            time_flag = 1;
            
            // �ڵȴ�ʱ���ڣ��ж��Ƿ񳬹��ȴ�ʱ��
            if (FreeRTOSRunTimeTicks >= 2)
            {
                // ���õ�������Ŀ��λ��Ϊһ��ڷŸ߶�
                Motor_Expect.Target_2006_Black = Black_Rack_Suction_One_Layer;
                
                // �л�״̬���������̻ص�һ��ڷŸ߶�״̬
                CraneMasterDataSet.CraneStatus = CraneBlackRackSuctionBox2OneLayer;
                
                // ����ʱ���־λ�ͼ�ʱ��
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
        
        // �������������ƿ����λ���ƶ�
        if (CraneMasterDataSet.CraneStatus == CraneBlackRackSuctionBox2OneLayer)
        {
            // ���㲢�����������̵���תĿ��λ��
            CraneMasterDataSet.rotate_expect.Rotate_Expect2D = GM6020Turn(2, ObjectResult[5]);
            
            // �ж������Ƿ�ﵽĿ��λ��
            if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2D - (float)motor_6020[2].total_ecd_out_6020) < 90)
            {
                // �л�״̬������������ת������ƿ����λ��״̬
                CraneMasterDataSet.CraneStatus = CraneRotate_Expect2D;
            }
        }
        
        // ��������ͣ�ڵ���ƿ����λ��
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2D && fabs((float)motor_6020[2].ecd - (float)motor_6020[2].last_ecd) < 3)
        {
            // �л�״̬����������ͣ�ڵ���ƿ����λ��״̬
            CraneMasterDataSet.CraneStatus = CraneRotate_Expect2DStop;
            
            // ���ʱ���־λ
            time_flag = 1;
        }
        
        // �ȴ�2�룬�������������ƿ����λ���ƶ�
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2DStop && FreeRTOSRunTimeTicks >= 4)
        {
            // �����������̵�Ŀ��λ��Ϊ����ƿ����λ��
            Motor_Expect.Target_2006_White = White_Rack_Suction_Coca_Cola;
            
            // �л�״̬���������������ƿ����λ���ƶ�״̬
            CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola3;
            
            // ����ʱ���־λ�ͼ�ʱ��
            time_flag = 0;
            FreeRTOSRunTimeTicks = 0;
        }

        // ���������ƶ�������ƿ����λ��
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola3)
        {
            // �ж����������Ƿ�ﵽ����ƿ����λ��
            if (fabs((float)White_Rack_Suction_Coca_Cola - (float)motor_2006[3].total_ecd_out_2006) < 25)
            {
                // �л�״̬�����������ƶ�������ƿ����λ�õȴ�״̬
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola3Wait;
            }
        }
        
        // �ȴ�2�룬�������̻ص�׼��λ��
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola3Wait)
        {
            // ����ʱ���־λ
            time_flag = 1;
            
            // �ڵȴ�ʱ���ڣ��ж��Ƿ񳬹��ȴ�ʱ��
            if (FreeRTOSRunTimeTicks >= 4)
            {
                // ��������Ŀ��λ��Ϊ׼��λ��
                Motor_Expect.Target_2006_White = White_Rack_Ready;
                
                // �л�״̬���������̻ص�׼��λ��״̬
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola3Finish;
                
                // ����ʱ���־λ�ͼ�ʱ��
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
        
        // �������̶��������ת��������������ã�������ڶ�ƿ����λ���ƶ�
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola3Finish)
        {
            // ���ö���Ƕȣ��������������
            TIM_SetCompare2(TIM2, Servo3_Left_Suction);
            CraneMasterDataSet.target_air_pump.target_air_pump_left = 1;
            
            // ���㲢���ó�����תĿ��λ��
            CraneMasterDataSet.rotate_expect.Rotate_Expect2E = GM6020Turn(2, ObjectResult[4]);
            
            // �жϳ����Ƿ�ﵽĿ��λ��
            if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2E - (float)motor_6020[2].total_ecd_out_6020) < 87)
            {
                // �л�״̬��������ת���ڶ�ƿ����λ��״̬
                CraneMasterDataSet.CraneStatus = CraneRotate_Expect2E;
            }
        }
        
        // ����ͣ�ڵڶ�ƿ����λ��
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2E && fabs((float)motor_6020[2].ecd - (float)motor_6020[2].last_ecd) < 3)
        {
            // �л�״̬������ͣ�ڵڶ�ƿ����λ��״̬
            CraneMasterDataSet.CraneStatus = CraneRotate_Expect2EStop;
            
            // ����ʱ���־λ
            time_flag = 1;
        }
        
        // �ȴ�2�룬����������ڶ�ƿ����λ���ƶ�
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2EStop && FreeRTOSRunTimeTicks >= 4)
        {
            // ��������Ŀ��λ��Ϊ�ڶ�ƿ����λ��
            Motor_Expect.Target_2006_White = White_Rack_Suction_Coca_Cola;
            
            // �л�״̬�����������ƶ����ڶ�ƿ����λ��״̬
            CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola2;
            
            // ����ʱ���־λ�ͼ�ʱ��
            time_flag = 0;
            FreeRTOSRunTimeTicks = 0;
        }
        
        // ���������ƶ����ڶ�ƿ����λ��
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola2)
        {
            // �ж����������Ƿ�ﵽ�ڶ�ƿ����λ��
            if (fabs((float)White_Rack_Suction_Coca_Cola - (float)motor_2006[3].total_ecd_out_2006) < 25)
            {
                // �л�״̬�����������ƶ����ڶ�ƿ����λ�õȴ�״̬
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola2Wait;
            }
        }
        
        // �ȴ�2�룬�������̻ص�׼��λ��
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola2Wait)
        {
            // ����ʱ���־λ
            time_flag = 1;
            
            // �ڵȴ�ʱ���ڣ��ж��Ƿ񳬹��ȴ�ʱ��
            if (FreeRTOSRunTimeTicks >= 4)
            {
                // ��������Ŀ��λ��Ϊ׼��λ��
                Motor_Expect.Target_2006_White = White_Rack_Ready;
                
                // �л�״̬���������̻ص�׼��λ��״̬
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola2Finish;
                
                // ����ʱ���־λ�ͼ�ʱ��
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
				
				// �������̶��������ת�����ұ��������ã��������һƿ����λ���ƶ�
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola2Finish)
        {
            // ���ö���Ƕȣ����ұ���������
            TIM_SetCompare2(TIM2, Servo3_Right_Suction);
            CraneMasterDataSet.target_air_pump.target_air_pump_right = 1;
            
            // ���㲢���ó�����תĿ��λ��
            CraneMasterDataSet.rotate_expect.Rotate_Expect2F = GM6020Turn(2, ObjectResult[3]);
            
            // �жϳ����Ƿ�ﵽĿ��λ��
            if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2F - (float)motor_6020[2].total_ecd_out_6020) < 72)
            {
                // �л�״̬��������ת����һƿ����λ��״̬
                CraneMasterDataSet.CraneStatus = CraneRotate_Expect2F;
            }
        }
        
        // ����ͣ�ڵ�һƿ����λ��
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2F && fabs((float)motor_6020[2].ecd - (float)motor_6020[2].last_ecd) < 2)
        {
            // �л�״̬������ͣ�ڵ�һƿ����λ��״̬
            CraneMasterDataSet.CraneStatus = CraneRotate_Expect2FStop;
            
            // ����ʱ���־λ
            time_flag = 1;
        }        
        
        // �ȴ�3�룬�����������һƿ����λ���˶�
        if (CraneMasterDataSet.CraneStatus == CraneRotate_Expect2FStop && FreeRTOSRunTimeTicks >= 6)
        {
            // ��������Ŀ��λ��Ϊ��һƿ����λ��
            Motor_Expect.Target_2006_White = White_Rack_Suction_Coca_Cola3;
            
            // �л�״̬�������������һƿ����λ���˶�״̬
            CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola1;
            
            // ����ʱ���־λ�ͼ�ʱ��
            time_flag = 0;
            FreeRTOSRunTimeTicks = 0;
        }
				
				// ���������˶�����һƿ����λ��
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola1)
        {
            if (fabs((float)White_Rack_Suction_Coca_Cola3 - (float)motor_2006[3].total_ecd_out_2006) < 25)
            {
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola1Wait;
            }
        }
        
        // �ȴ�2�룬�������̻ص�һ��ڷŸ߶�
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola1Wait)
        {
            // ����ʱ���־λ
            time_flag = 1;
            
            if (FreeRTOSRunTimeTicks >= 4)
            {
                // ��������Ŀ��λ��Ϊһ��ڷŸ߶�
                Motor_Expect.Target_2006_White = White_Rack_Suction_One_Layer;
                
                // �л�״̬���������̻ص�һ��ڷŸ߶����״̬
                CraneMasterDataSet.CraneStatus = CraneWhiteRackSuctionCoca_Cola1Finish;
                
                // ����ʱ���־λ�ͼ�ʱ��
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
        
        // �������̻ص�����ڷŸ߶ȣ����ֻص��ڷ�λ�ã�������ڶ��ΰڷ�λ����ת��
        // ��ת��������Χ�����ػ��������˶����������
        if (CraneMasterDataSet.CraneStatus == CraneWhiteRackSuctionCoca_Cola1Finish)
        {
            // �������̺ͳ���Ŀ��λ��
            Motor_Expect.Target_2006_Black = Black_Rack_Suction_Two_Layer;
            Motor_Expect.Target_2006_White = White_Rack_Suction_Two_Layer;
            Motor_Expect.Target_3508_Black = Black_Placement_Two;
            Motor_Expect.Target_3508_White = White_Placement_Two;
            
            // ���㲢���ó�����תĿ��λ��
            CraneMasterDataSet.rotate_expect.Rotate_Expect2Done = GM6020Turn(2, 1);
            
            // �жϳ����Ƿ���ת��������Χ��
            if (fabs((float)CraneMasterDataSet.rotate_expect.Rotate_Expect2Done - (float)motor_6020[2].total_ecd_out_6020) < 200)
            {
                // ���õ��̺��˸����˶����������
                Motor_Expect.Target_Speed_3508_Chassis = Chassis_Backward_High_Speed;
                TIM_SetCompare1(TIM2, Servo1_Placement);
                
                // �л�״̬�����ػ����˸����˶�״̬
                CraneMasterDataSet.CraneStatus = CraneBackwardHighSpeed;
            }
        }
				
				// ����������3(���)�����ػ�����
        if (CraneMasterDataSet.sensor3 == 0 && CraneMasterDataSet.CraneStatus == CraneBackwardHighSpeed)
        {
            // ���õ��̺��˵����˶�
            Motor_Expect.Target_Speed_3508_Chassis = Chassis_Backward_Low_Speed;
            
            // �л�״̬�����ػ����˵����˶�״̬
            CraneMasterDataSet.CraneStatus = CraneBackwardLowSpeed1;
        }
        
        // ����������4(�Һ�)�����ػ�ͣ�£��������������ƶ�(ʹ�ڷŸ���ƽ��)
        if (CraneMasterDataSet.sensor4 == 0 && CraneMasterDataSet.CraneStatus == CraneBackwardLowSpeed1)
        {
            // ���õ���ֹͣ�˶�
            Motor_Expect.Target_Speed_3508_Chassis = Chassis_Stop;
            
            // ��������Ŀ��λ��Ϊ�����ƶ�λ��
            Motor_Expect.Target_2006_Black = Black_Rack_Suction_Devolved_Two;
            Motor_Expect.Target_2006_White = White_Rack_Suction_Devolved_Two;
            
            // ����ʱ���־λ
            time_flag = 1;
            
            // �л�״̬�����ػ�����ֹͣ״̬
            CraneMasterDataSet.CraneStatus = CraneBackwardStop1;
        }
        
        // ���ػ�ֹͣ�󣬵ڶ��룬�ر��������ã������룬̧������(֤�����ػ��뿪��Ʒ)
        if (CraneMasterDataSet.CraneStatus == CraneBackwardStop1)
        {
            if (FreeRTOSRunTimeTicks >= 3 && FreeRTOSRunTimeTicks <= 4)
            {
                // �ر�������������
                CraneMasterDataSet.target_air_pump.target_air_pump1 = 0;
                CraneMasterDataSet.target_air_pump.target_air_pump_middle = 0;
                CraneMasterDataSet.target_air_pump.target_air_pump_left = 0;
                CraneMasterDataSet.target_air_pump.target_air_pump_right = 0;
            }
            
            if (FreeRTOSRunTimeTicks > 4 && FreeRTOSRunTimeTicks <= 6)
            {
                // ��������Ŀ��λ��Ϊ̧��λ��
                Motor_Expect.Target_2006_Black = Black_Rack_Suction_Lift2;
                Motor_Expect.Target_2006_White = White_Rack_Suction_Lift2;
                
                // �л�״̬���ڶ��ΰڷ����״̬
                CraneMasterDataSet.CraneStatus = CranePlacementTwoFinish;
                
                // ����ʱ���־λ�ͼ�ʱ��
                time_flag = 0;
                FreeRTOSRunTimeTicks = 0;
            }
        }
				
				// ��ʱ2���룬�ó�CPUʱ��Ƭ
				vTaskDelay(2);
		}
}

/**
 * @brief ͨ�� USART �����ַ���
 *
 * �ú���ͨ��ָ���� USART �ӿڷ��͸������ַ����������ַ����е�ÿ���ַ���
 * ������ͣ�����ÿ���ַ�������ɺ�ȴ�ȷ�ϡ�
 *
 * @param[in] USARTx USART �ӿڵ�ָ�룬�� USART1��USART2 ��
 * @param[in] str Ҫ���͵��ַ����ĵ�ַ
 */
void USART_SendString(USART_TypeDef* USARTx, const char* str)
{
    // �����ַ����е�ÿ���ַ�����һ��һ������
    for (size_t i = 0; i < strlen(str); i++)
    {
        // �ȴ����ݷ������
        while (!(USARTx->SR & USART_SR_TXE));
        
        // ���ַ����͵��������ݼĴ���
        USARTx->DR = (uint16_t)str[i];
        
        // �ȴ����ݷ������
        while (!(USARTx->SR & USART_SR_TC));
    }
}

/**
 * @brief ���ߺ����ж�
 *
 * �ú��������жϽ��յ��� USART �����Ƿ������ض������������жϽ����
 *
 * @param[in] USART_RX_BUF USART ���ջ�����
 * @param[in] length ����������
 * @return _Bool �жϽ����1 ��ʾ����������0 ��ʾ����������
 */
_Bool Decision_Function(u8 USART_RX_BUF[], int length)
{
    int count0 = 0; // ����������¼�ַ� '1' ������
    int count1 = 0; // ����������¼�ַ� '2' ������

    // �������ջ������е��ַ�
    for (int i = 0; i < length; i++) 
    {
        if (USART_RX_BUF[i] == '1') 
        {
            count0++; // ͳ���ַ� '1' ������
        } 
        else if (USART_RX_BUF[i] == '2') 
        {
            count1++; // ͳ���ַ� '2' ������
        }
        
        // �ж��Ƿ�ͬʱ������3���ַ� '1' �� 3���ַ� '2'
        if (count0 == 3 && count1 == 3)
        {
            return 1; // �������������� 1
        }
    }
    
    return 0; // ���������������� 0
}


/**
 * @brief GM6020 ת�����ú���
 *
 * �ú������ݴ���Ĳ������� GM6020 �����ת��Ŀ��Ƕȣ����������õ�Ŀ��Ƕ�ֵ��
 *
 * @param[in] Rod �����ţ�1 �� 2������ͬ�����̣�
 * @param[in] Position Ŀ��λ�ñ�ţ�1~6������ͬ��Ŀ��λ�ã�
 * @return int ���õ� GM6020 �����Ŀ��Ƕ�ֵ

 */
int GM6020Turn(int Rod, int Position)
{
    // ���� Rod �� Position �Ĳ�ͬ��ϣ�������Ӧ��Ŀ��Ƕ�ֵ
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
    
    return Motor_Expect.Angle_Target_6020; // �������õ�Ŀ��Ƕ�ֵ
}

