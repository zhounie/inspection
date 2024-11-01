#include "stm32f10x.h"                  // Device header

void LED_Init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
}

void LED_0_ON(void) {
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void LED_1_ON(void) {
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void LED_0_OFF(void) {
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void LED_1_OFF(void) {
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void LED_0_REVERSE(void) {
	// 读取当前GPIO输出状态
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0) == 0) {
		LED_0_OFF();
	} else {
		LED_0_ON();
	}
}

void LED_1_REVERSE(void) {
	// 读取当前GPIO输出状态
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0) {
		LED_1_OFF();
	} else {
		LED_1_ON();
	}
}

