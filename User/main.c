#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "MPU6050.h"
#include "Data.h"

MPU6050Data mpu[10];
int16_t AY_m = -40;

int main(void)
{
	
	OLED_Init();
	OLED_Clear();
	Serial_Init();
	MPU6050_Init();
	
	//uint8_t order[]={0x68,0x08,0x00,0xFF,0x10,0x00,0x0F,0x16};
	//uint8_t order[]={0x68,0x08,0x00,0xFF,0x10,0x01,0x10,0x16};
	//uint8_t order_2[]={0x68,0x08,0x00,0xFF,0x12,0x00,0x11,0x16};
	uint8_t order[]={0x68,0x08,0x00,0xFF,0x12,0x00,0x11,0x16};
	uint8_t order_2[]={0x68,0x08,0x00,0xFF,0x12,0x01,0x12,0x16};
	//Serial_SendArray(order,8);
	Delay_s(1);
	//Serial_SendArray(order_2,8);
	while (1)
	{
//		if(AY-AY_m>300){
//			Serial_SendArray(order,8);
//		}
//		if(AY-AY_m<-300){
//			Serial_SendArray(order_2,8);
//		}
		MPU6050_GetDataArray(mpu,10);
		OLED_ShowSignedNum(2, 1, mpu[0].AX, 5);
		OLED_ShowSignedNum(3, 1, mpu[0].AY-AY_m, 5);
		OLED_ShowSignedNum(4, 1, mpu[0].AZ, 5);
		OLED_ShowSignedNum(2, 8, mpu[0].GX, 5);
		OLED_ShowSignedNum(3, 8, mpu[0].GY, 5);
		OLED_ShowSignedNum(4, 8, mpu[0].GZ, 5);
	}
}
