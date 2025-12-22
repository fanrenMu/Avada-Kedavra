#include "main.h"

int main(void)
{
	SYS_INIT();
	
	while (1)
	{
		if(mpustatus == SAMPLING){
			OLED_ShowString(2,1,"ing");
			MPU6050_GetDataArray(mpu,150);
			mpustatus = SAMPLED;
			OLED_ShowString(2,4,"ed");       //get data from imu
			
			model_feed_data();
			OLED_ShowString(3,1,"feed");
			model_run(model);
			OLED_ShowString(3,6,"run");      //feed data to model
			
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
	if(EXTI_GetITStatus(EXTI_Line12)==SET){
		
		mpustatus = SAMPLING;

		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}
