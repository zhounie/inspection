#include "stm32f10x.h"                  // Device header
#include "USART3.h"
#include "TIMER.h"
#include <stdio.h>
#include <stdarg.h> 
#include <string.h>

// 串口缓冲区
uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN];
uint8_t USART3_TX_BUF[USART3_MAX_SEND_LEN];

volatile uint16_t USART3_RX_STA = 0;

void USART3_Init(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
		USART_DeInit(USART3);
			
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		
		USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART3, &USART_InitStructure);
		
		USART_Cmd(USART3, ENABLE); // 使能串口
		
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开中断
	
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_Init(&NVIC_InitStructure);
		
		
		TIM4_Int_Init(1000 - 1, 7200 - 1); //10ms中断
		USART3_RX_STA = 0;
		TIM_Cmd(TIM4, DISABLE);
	
		
}

void USART3_Printf(char *format, ...)
{
	uint16_t i, j;
	va_list arg;
	va_start(arg, format);
	vsprintf((char*)USART3_TX_BUF, format, arg);
	va_end(arg);
	i = strlen((const char*)USART3_TX_BUF);
	for(j = 0; j < i; j++){
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
		USART_SendData(USART3, USART3_TX_BUF[j]); 
	}
}

void USART3_IRQHandler(void)	
{
	uint8_t res;
	// 接收到数据
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		res = USART_ReceiveData(USART3);
		// 接收完的一批数据，还没有被处理，则不在接收其他数据
		if ((USART3_RX_STA&(1<<15)) == 0) {
			// 还可以接收数据
			if (USART3_RX_STA < USART3_MAX_RECV_LEN) {
				TIM_SetCounter(TIM4, 0); // 计数器清空
				if (USART3_RX_STA == 0) {
					TIM_Cmd(TIM4, ENABLE); // 使能定时器
				}
				USART3_RX_BUF[USART3_RX_STA++] = res; // 记录接收到的值
			} else {
				USART3_RX_STA |= 1<<15; // 强制标记接收完成
			}
		}
	}
}
