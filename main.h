#ifndef __MAIN_H
#define __MAIN_H

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

#endif
