#include "IR_Capture.h"

void IR_Capture_Init(void)
{
    /* 1. 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* 2. 配置 PA7 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    // 虽然外部有上拉，但配置为 IPU 更稳妥
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 3. 时基单元配置 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;       
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;    // 1MHz，即 1us
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    /* 4. 输入捕获配置 (TIM3_CH2) */
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;     
    // F1系列不支持 BothEdge，先设为下降沿(红外头起始为低电平)
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; 
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x8;              // 增加数字滤波(0x0-0xF)，防杂散光干扰
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    /* 5. 开启中断和定时器 */
    TIM_ITConfig(TIM3, TIM_IT_CC2 | TIM_IT_Update, ENABLE); // 开启更新中断处理长电平
    TIM_Cmd(TIM3, ENABLE);

    /* 6. NVIC 配置 */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
