#include "hongwai.h"

void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // PA6 对应 TIM3_CH1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    // 频率计算：72MHz / 38000 ≈ 1895
    TIM_TimeBaseInitStructure.TIM_Period = 1895 - 1; 
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; // 不分频
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    
    // 初始 CCR1 设为 0（关闭发射）
    TIM_OCInitStructure.TIM_Pulse = 0; 
    TIM_OC1Init(TIM3, &TIM_OCInitStructure); // 必须用 OC1，因为 PA6 是通道 1
    
    TIM_Cmd(TIM3, ENABLE);
}


// 开启载波 (根据之前计算，CCR1=632 为 38kHz 1/3 占空比)
void IR_ON(void) {
    TIM_SetCompare1(TIM3, 632); 
}

// 关闭载波
void IR_OFF(void) {
    TIM_SetCompare1(TIM3, 0);
}

void IR_Send_Raw(uint16_t *data, uint16_t length)
{

    
    for (uint16_t i = 0; i < length; i++)
    {
				if (i % 2 == 0) {
							// 偶数索引 (0, 2, 4...)：数据对应的是“有信号”的时间 (Mark)
							IR_ON(); 
					} else {
							// 奇数索引 (1, 3, 5...)：数据对应的是“无信号”的时间 (Space)
							IR_OFF();
        }
        
        // 延时对应的微秒数
        Delay_us(*(data+i));
    }
		IR_OFF();

}
