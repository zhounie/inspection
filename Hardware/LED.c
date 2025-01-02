#include "stm32f10x.h"                  // Device header

uint32_t Led_Count;

void LED_Init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

void LED_0_ON(void) {
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void LED_1_ON(void) {
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void LED_0_OFF(void) {
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}

void LED_1_OFF(void) {
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

void LED_0_REVERSE(void) {
	// 读取当前GPIO输出状态
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0) == 0) {
		LED_0_OFF();
	} else {
		LED_0_ON();
	}
}

void LED_1_REVERSE(void) {
	// 读取当前GPIO输出状态
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1) == 0) {
		LED_1_OFF();
	} else {
		LED_1_ON();
	}
}

uint32_t Led_Get(void)
{
	return Led_Count;
}

void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2) == SET) {
		Led_Count++;
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}


