#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "Serial.h"
#include "Data.h"
#define MPU6050_ADDRESS		0xD0

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			//Serial_Printf("I2C Timeout!\n");
			break;
		}
	}
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, RegAddress);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C1, Data);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, RegAddress);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C1, ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, MPU6050_ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return Data;
}

void MPU6050_Init(void)
{
	I2C_DeInit(I2C1);
	
	 /*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*I2C初始化*/
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	 /*I2C使能*/
   I2C_Cmd(I2C1, ENABLE);                                                                        //使能I2C1，开始运行
   
   MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);                                //电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
   MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);                                //电源管理寄存器2，保持默认值0，所有轴均不待机
   MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);                                //采样率分频寄存器，配置采样率
   MPU6050_WriteReg(MPU6050_CONFIG, 0x06);                                        //配置寄存器，配置DLPF
   MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);                        //陀螺仪配置寄存器，选择满量程为±2000°/s
   MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);                        //加速度计配置寄存器，选择满量程为±16g

}



uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
void MPU6050_GetDataArray(MPU6050Data *mpu6050data,int16_t count){
	int16_t i;
	for(i=0;i<count;i++){
        MPU6050_GetData(&mpu6050data[i].AX, &mpu6050data[i].AY, &mpu6050data[i].AZ,
                       &mpu6050data[i].GX, &mpu6050data[i].GY, &mpu6050data[i].GZ);
	}
}

void MPU6050_MotionDetection_Init(uint8_t Threshold, uint8_t Duration)
{
    uint8_t accel_cfg = MPU6050_ReadReg(MPU6050_ACCEL_CONFIG);
    accel_cfg |= 0x01; 
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, accel_cfg);


    MPU6050_WriteReg(MPU6050_MOT_THR, Threshold);


    MPU6050_WriteReg(MPU6050_MOT_DUR, Duration);


    MPU6050_WriteReg(MPU6050_MOT_DETECT_CTRL, 0x15);


    MPU6050_WriteReg(MPU6050_INT_PIN_CFG, 0x10);

    MPU6050_WriteReg(MPU6050_INT_ENABLE, 0x40);
}
