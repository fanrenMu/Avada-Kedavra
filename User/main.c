#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "MPU6050.h"
#include "Data.h"
#include "nnom.h"
#include "weights.h"
#include "Key.h"

#define AVAILABLE 0
#define SAMPLING 1
#define SAMPLED 2
#ifdef NNOM_USING_STATIC_MEMORY
	uint8_t static_buf[1024 * 8];
#endif //NNOM_USING_STATIC_MEMORY

MPU6050Data mpu[150];
volatile int mpustatus;
int16_t AY_m = -40;

nnom_model_t *model;

void model_feed_data(void);

int main(void)
{
	OLED_Init();
	OLED_Clear();
	Serial_Init();
	MPU6050_Init();
	Key_Init();
	
	OLED_ShowString(1,1,"init");

    #ifdef NNOM_USING_STATIC_MEMORY
		nnom_set_static_buf(static_buf, sizeof(static_buf)); 
	#endif //NNOM_USING_STATIC_MEMORY

	model = nnom_model_create();
	OLED_ShowString(1,5,"create");
	mpustatus = AVAILABLE;
	
	while (1)
	{
		if(mpustatus == SAMPLING){
			OLED_ShowString(2,1,"ing");
			MPU6050_GetDataArray(mpu,150);
			mpustatus = SAMPLED;
			//OLED_Clear();
			OLED_ShowString(2,4,"ed");
//			OLED_ShowNum(3,2,mpu[2].AX,3);
//			OLED_ShowNum(3,6,mpu[2].AY,3);
//			OLED_ShowNum(3,10,mpu[2].AZ,3);
			model_feed_data();
			OLED_ShowString(3,1,"feed");
			model_run(model);
			OLED_ShowString(3,6,"run");
			
			int8_t max_output = -128;
			int8_t ret = 0;
			for(int i = 0; i < 13;i++){
				if(nnom_output_data[i] >= max_output){
					max_output = nnom_output_data[i] ;
					ret = i;
				}
				OLED_ShowNum(4,1,(uint32_t)ret,2);
			}

		}

	}

}

void model_feed_data(void)
{
	const double scale = 16;
	uint16_t i = 0;
	for(i = 0; i < 150;i++){
		nnom_input_data[i*3] = (int8_t)round(mpu[i].GX * scale);
		nnom_input_data[i*3+1] = (int8_t)round(mpu[i].GY * scale);
		nnom_input_data[i*3+2] = (int8_t)round(mpu[i].GZ * scale);
	}
}

void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line12)==SET){
		mpustatus = SAMPLING;
		//OLED_Clear();

		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}
