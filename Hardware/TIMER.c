#include "stm32f10x.h"                  // Device header
//#include <stdint.h>

extern volatile uint16_t USART3_RX_STA;

void TIM4_Int_Init(uint16_t arr, uint16_t psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIM4时钟
	
	// 定时器初始化
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); // 使能制定的TIM4中断，允许更新中断
	
	TIM_Cmd(TIM4, ENABLE); // 开启定时器
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM4_IRQHandler(void)
{
	// 是更新中断
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		USART3_RX_STA |= 1<<15; // 标记接收完成
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // 清除TIM4更新终端标志
		TIM_Cmd(TIM4, DISABLE); //关闭TIM4
	}
}
