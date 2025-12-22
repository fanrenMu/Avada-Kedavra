#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

void Serial_AutoSendNumber(uint32_t Number)
{
    uint8_t temp[10]; // uint32_t 最大是 4,294,967,295，共 10 位
    uint8_t i = 0;

    // 处理 Number 为 0 的特殊情况
    if (Number == 0)
    {
        Serial_SendByte('0');
        return;
    }

    // 循环取模，存入数组（此时是倒序：个位、十位...）
    while (Number > 0)
    {
        temp[i++] = Number % 10 + '0';
        Number /= 10;
    }

    // 反向遍历数组，发送数据
    while (i > 0)
    {
        Serial_SendByte(temp[--i]);
    }
}

void Serial_SendFloat(float Number)
{
    // 1. 处理负数
    if (Number < 0)
    {
        Serial_SendByte('-');
        Number = -Number;
    }

    // 2. 提取并发送整数部分
    uint32_t IntPart = (uint32_t)Number;
    Serial_AutoSendNumber(IntPart); // 调用之前写好的整数发送函数

    // 3. 发送小数点
    Serial_SendByte('.');

    // 4. 提取并发送小数部分（假设保留2位）
    // +0.005 是为了四舍五入，防止 0.199999 变成 0.19
    uint32_t DecPart = (uint32_t)((Number - IntPart) * 1000000 + 0.5);

    // 5. 特殊处理：补齐小数位的 0
    // 如果小数部分是 0.05，乘以100后得到 5，直接打印会变成 .5，所以需要补0
    if (DecPart < 10)
    {
        Serial_SendByte('0');
    }
    
    Serial_AutoSendNumber(DecPart);
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}
