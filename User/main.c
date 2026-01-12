#include "main.h"

//uint16_t ArrayWrite[] = {0x0001, 0x0002, 0x0003, 0x0004};	//定义要写入数据的测试数组
//uint16_t ArrayRead[4];							//定义要读取数据的测试数组
//uint8_t MID;							//定义用于存放MID号的变量
//uint16_t DID;							//定义用于存放DID号的变量

int main(void)
{
	SYS_INIT();
	//circle_handler();

//	W25Q64_ReadID(&MID, &DID);			//获取W25Q64的ID号
//	Serial_Printf("MID:d%  DID:d% \n",MID,DID);
	
//	W25Q64_Array2Buffer(ArrayWrite,4);
//	W25Q64_Print_Buffer();
//	W25Q64_Write_Sector(0x000000);
//	
//	W25Q64_Buffer2Array(ArrayWrite,4);
//	W25Q64_Print_Buffer();

	while (1)
	{
		if(mpustatus == SAMPLING){
			MPU6050_GetDataArray(mpu,150);
			mpustatus = AVAILABLE;

			model_feed_data();
			model_run(model);
			model_output = model_get_output();
			
			switch(model_output){
				case Unrecognized:
					Serial_Printf("Unrecognized\n");
					break;
				case Circle:
					Serial_Printf("circle\n");
					circle_handler();
				  break;
				case Wave:
					Serial_Printf("Wave\n");	
					wave_handler();
				  break;
				case Letter_h:
					Serial_Printf("Letterh\n");	
  				Letterh_handler();
				  break;
				case letter_R:
					Serial_Printf("LetterR\n");	
  				LetterR_handler();
				  break;
				default:
					Serial_Printf("none");
					//wave_handler();
					break;
			}
		}
		
		if(System_Mode == MODE_IR_LEARNING && IR_Learn_Finish == 1) {
			Serial_Printf("IR Learning Success! Data Len: %d\n", IR_DataLen);
			
			switch(model_output){  //存储至flash
			case Circle:
				W25Q64_Array2Buffer(IR_RawData,IR_DataLen);
				W25Q64_Write_Sector(W25Q64_IR_ON);
				break;
			case Wave:
				W25Q64_Array2Buffer(IR_RawData,IR_DataLen);
				W25Q64_Write_Sector(W25Q64_IR_OFF);
			  break;
			default:
				Serial_Printf("chaos\n");
				break;
		}

			// 学习完成后，切回动作识别模式
			System_Mode = MODE_ACTION_RECOGNITION;
		}
		
	}
		
}


void model_feed_data(void){
	const double scale = 32;
	uint16_t i = 0;
//	Serial_SendString("IMU\n");
	for(i = 0; i < 150;i++){
		float Gx = mpu[i].GX/8192.0;
		float Gy = mpu[i].GY/8192.0;
		float Gz = mpu[i].GZ/8192.0;
		nnom_input_data[i*3] = (int8_t)round(Gx * scale);
		nnom_input_data[i*3+1] = (int8_t)round(Gy * scale);
		nnom_input_data[i*3+2] = (int8_t)round(Gz * scale);
		
//		Serial_SendFloat(mpu[i].AX/8192.0);
//		Serial_SendString(" ");
//		Serial_SendFloat(mpu[i].AY/8192.0);
//		Serial_SendString(" ");		
//		Serial_SendFloat(mpu[i].AZ/8192.0);
//		Serial_SendString(" ");		
//		
//		Serial_SendFloat(Gx);
//		Serial_SendString(" ");
//		Serial_SendFloat(Gy);
//		Serial_SendString(" ");
//		Serial_SendFloat(Gz);
//		Serial_SendString(" ");
//		
//		Serial_SendString("\n");
	}
}

void SYS_INIT(void){
	  #ifdef NNOM_USING_STATIC_MEMORY
			nnom_set_static_buf(static_buf, sizeof(static_buf)); 
		#endif //NNOM_USING_STATIC_MEMORY
	
		Serial_Init();
		MPU6050_Init();
		Key_Init();
	  IR_Capture_Init();
		W25Q64_Init();
		mpustatus = AVAILABLE; 
		System_Mode = MODE_ACTION_RECOGNITION;	
	  //learning_action = -1;
		model = nnom_model_create();
}

int8_t model_get_output(void){
		int8_t max_output = -128;
			int8_t ret = 0;
			for(int i = 0; i < 13;i++){
				if(nnom_output_data[i] >= max_output){
					max_output = nnom_output_data[i] ;
					ret = i;
				}
			}
		return ret;
}

void EXTI0_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line0)==SET){  //action int
		
		mpustatus = SAMPLING;

		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	
}

void TIM3_IRQHandler(void){
	
    // --- 1. 处理输入捕获中断 (PA7 / TIM3_CH2) ---
    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) //学习模式且中断触发才处理
    {
			if (System_Mode == MODE_IR_LEARNING){
				uint32_t CurrentTick = TIM_GetCapture2(TIM3);
        static uint32_t LastTick = 0;

        if (IR_Learn_Finish == 0)
        {
            if (IR_DataLen == 0) 
            {
                // 第一次捕获：红外起始（通常是下降沿）
                LastTick = CurrentTick;
                Timer_Overflow_Cnt = 0;
                IR_DataLen = 1;
            } 
            else 
            {
                // 计算时间差：当前计数值 + 溢出掉的满量程值 - 上一次值
                uint32_t Duration = (CurrentTick + (Timer_Overflow_Cnt * 65536)) - LastTick;
                
                if (IR_DataLen < MAX_RAW_SIZE)
                {
                    IR_RawData[IR_DataLen - 1] = Normalization((uint16_t)Duration);
                    IR_DataLen++;
                }
                
                LastTick = CurrentTick;
                Timer_Overflow_Cnt = 0; // 计算完差值，清除溢出累加
            }

            // --- 动态翻转极性 (F1系列必须手动翻转) ---
						if ((TIM3->CCER & TIM_CCER_CC2P) == 0) 
						{
								// 当前是上升沿（CC2P=0），切换到下降沿
								TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
						}
						else 
						{
								// 当前是下降沿（CC2P=1），切换到上升沿
								TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
						}
        }
        
        
			}
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    }

    // --- 2. 处理定时器溢出中断 (用于超时判断和长脉冲计算) ---
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        Timer_Overflow_Cnt++; // 累加溢出次数

        // 超时判断：如果在接收过程中，超过 100ms (1.5次溢出左右) 没有捕获
        // 1MHz 频率下，65535us 溢出一次，溢出 2 次约为 131ms
        if (IR_DataLen > 1 && Timer_Overflow_Cnt > 2) 
        {
            IR_Learn_Finish = 1; // 标记接收结束
            // 结束后恢复初始极性：下降沿
            TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
        }
        
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
		
		
		
}


void Start_Timer(void) {
    SysTick->LOAD = 72000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL |= 1;
}

void Stop_And_Print_Timer(void) {
    uint32_t elapsed_ticks = 72000 - SysTick->VAL;
    float elapsed_ms = (float)elapsed_ticks / 72000.0f * 1000.0f;

    SysTick->CTRL &= ~1;

    //printf("Reasoning Time: %.3f ms\n", elapsed_ms);
		OLED_ShowNum(3,1,elapsed_ms,4);
}

void Switch_To_Send(uint16_t *data, uint16_t len) {
    TIM_DeInit(TIM3);      // 清除之前的接收配置
    PWM_Init();            // 重新配置为 38kHz 发射模式
    IR_Send_Raw(data, len); // 执行发送
    TIM_DeInit(TIM3);      // 清除发射配置
    IR_Capture_Init();     // 重新切回接收模式
}

void circle_handler(void){ //学习开机波形
	// 1. 切换模式
  System_Mode = MODE_IR_LEARNING;
   
  // 2. 准备学习变量
  IR_DataLen = 0;
  IR_Learn_Finish = 0;
	
}

void wave_handler(void){   //学习关机波形
	
	// 1. 切换模式
  System_Mode = MODE_IR_LEARNING;
   
  // 2. 准备学习变量
  IR_DataLen = 0;
  IR_Learn_Finish = 0;
	
}

void Letterh_handler(void){ //发送开机波形
	W25Q64_Read_Sector(W25Q64_IR_ON);
	int len = W25Q64_Buffer2Array(IR_SendData,MAX_RAW_SIZE);
	Serial_Printf("read flash success,addr:%x \n len = %d",W25Q64_IR_ON,len);
	//W25Q64_Print_Buffer();
	Switch_To_Send(IR_SendData,len-1);
	Serial_Printf("send over \n");
}

void LetterR_handler(void){ //发送关机波形
	W25Q64_Read_Sector(W25Q64_IR_OFF);
	int len = W25Q64_Buffer2Array(IR_SendData,MAX_RAW_SIZE);
	Serial_Printf("read flash success,addr:%x \n len = %d",W25Q64_IR_OFF,len);
	//W25Q64_Print_Buffer();
	Switch_To_Send(IR_SendData,len-1);
	Serial_Printf("send over \n");
}



uint16_t Normalization(uint16_t data){
	if(data>=400 && data<=700){
		return 560;
	}
	if(data>=701 && data<=2000){
		return 1680;
	}
	if(data>=2001 && data<=3500){
		return 3000;
	}
	if(data>=3501){
		return 4500;
	}
	return 0;
}

