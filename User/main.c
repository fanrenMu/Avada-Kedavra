#include "main.h"

int main(void)
{
	SYS_INIT();
	
	while (1)
	{
		if(mpustatus == SAMPLING){
			//Start_Timer();
			OLED_ShowString(2,1,"ing");
			MPU6050_GetDataArray(mpu,150);
			mpustatus = AVAILABLE;
			OLED_ShowString(2,4,"ed");       //get data from imu
			//Stop_And_Print_Timer();

			model_feed_data();
			//OLED_ShowString(3,1,"feed");
			model_run(model);
			//OLED_ShowString(3,6,"run");      //feed data to model
			
			
			model_output = model_get_output();
			OLED_ShowNum(4,1,(uint32_t)model_output,2);
			
			switch(model_output){
				case Unrecognized:
					OLED_ShowString(4,4,"Unrecognized");
					break;
				case Circle:
					Serial_SendArray(turn_on,8);
					OLED_ShowString(4,4,"Circle");
				  break;
				case Wave:
					Serial_SendArray(turn_off,8);
					OLED_ShowString(4,4,"Wave");
				  break;
				default:
					OLED_ShowString(4,4,"none");
					break;
			}

		}
//		if(mpustatus == AVAILABLE){
//			Enter_Lowpower_Mode();
//		}
	}
		
}

void model_feed_data(void){
	const double scale = 32;
	uint16_t i = 0;
	Serial_SendString("IMU\n");
	for(i = 0; i < 150;i++){
		float Gx = mpu[i].GX/8192.0;
		float Gy = mpu[i].GY/8192.0;
		float Gz = mpu[i].GZ/8192.0;
		nnom_input_data[i*3] = (int8_t)round(Gx * scale);
		nnom_input_data[i*3+1] = (int8_t)round(Gy * scale);
		nnom_input_data[i*3+2] = (int8_t)round(Gz * scale);
		
		Serial_SendFloat(mpu[i].AX/8192.0);
		Serial_SendString(" ");
		Serial_SendFloat(mpu[i].AY/8192.0);
		Serial_SendString(" ");		
		Serial_SendFloat(mpu[i].AZ/8192.0);
		Serial_SendString(" ");		
		
		Serial_SendFloat(Gx);
		Serial_SendString(" ");
		Serial_SendFloat(Gy);
		Serial_SendString(" ");
		Serial_SendFloat(Gz);
		Serial_SendString(" ");
		
		Serial_SendString("\n");
	}
}

void SYS_INIT(void){
	  #ifdef NNOM_USING_STATIC_MEMORY
			nnom_set_static_buf(static_buf, sizeof(static_buf)); 
		#endif //NNOM_USING_STATIC_MEMORY
	
		OLED_Init();
		OLED_Clear();
		Serial_Init();
		MPU6050_Init();
		Key_Init();
//		MPU6050_MotionDetection_Init(THRESHOLD,DURATION);
		mpustatus = AVAILABLE;         
		OLED_ShowString(1,1,"init");   //hardware init
	
		model = nnom_model_create();
		OLED_ShowString(1,5,"create"); //model init
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

void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line12)==SET){  //action int
		
		mpustatus = SAMPLING;

		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	
//	if(EXTI_GetITStatus(EXTI_Line13)==SET){  //mpu6050 wake up int
//		if (MPU6050_ReadReg(0x3A) & 0x40) {
//			OLED_ShowString(3,1,"wake");
////			mpustatus = SAMPLING;
//		}
//		EXTI_ClearITPendingBit(EXTI_Line13);
//	}
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

void Enter_Lowpower_Mode(void){
		OLED_Clear();
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
    PWR_ClearFlag(PWR_FLAG_WU);
    
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    
    SystemInit();
}
