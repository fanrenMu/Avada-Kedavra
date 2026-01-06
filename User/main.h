#include "stm32f10x.h"                  // Device header
#include "stm32f10x_pwr.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "MPU6050.h"
#include "Data.h"
#include "nnom.h"
#include "weights.h"
#include "Key.h"
#include "hongwai.h"
#include "IR_Capture.h"


#define AVAILABLE 0
#define SAMPLING 1
#define THRESHOLD 30
#define DURATION 7
#define MODE_ACTION_RECOGNITION 0
#define MODE_IR_LEARNING        1

#ifdef NNOM_USING_STATIC_MEMORY
	uint8_t static_buf[1024 * 8];
#endif //NNOM_USING_STATIC_MEMORY

nnom_model_t *model;			//nnom model
MPU6050Data mpu[150];     //记录的动作数据
volatile int mpustatus;   //mpu 状态         
volatile int8_t model_output = -128;  //模型输出
volatile uint8_t System_Mode = MODE_ACTION_RECOGNITION; //系统状态
typedef enum action{                  //actions
	Unrecognized = -1,
	RightAngle = 0,
	SharpAngle = 1,
	Lightning = 2,
	Triangle = 3,
	Letter_h = 4,
	letter_R = 5,
	letter_W = 6,
	letter_phi = 7,
	Circle = 8,
	UpAndDown = 9,
	Horn = 10,
	Wave = 11,
	NoMotion = 12
} action;



uint8_t turn_on[]={0x68,0x08,0x00,0xFF,0x12,0x00,0x11,0x16};  
uint8_t turn_off[]={0x68,0x08,0x00,0xFF,0x12,0x01,0x12,0x16};
	
#define MAX_RAW_SIZE 300      // 最大记录跳变次数
uint16_t IR_RawData[MAX_RAW_SIZE]; 
uint16_t IR_DataLen = 0;      // 当前记录的数据长度
uint8_t  IR_Learn_Finish = 0; // 学习完成标志
uint16_t Timer_Overflow_Cnt = 0;

void model_feed_data(void);
void SYS_INIT(void);
int8_t model_get_output(void);
void Start_Timer(void);
void Stop_And_Print_Timer(void);
void Enter_Lowpower_Mode(void);
void circle_handler(void);
void wave_handler(void);
