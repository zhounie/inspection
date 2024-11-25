#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void KEY_Init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

uint8_t KEY_GetNum(void) {
	uint8_t KEY_Num = 0;
	
	// 读取 GPIO
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0) {
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0);
		Delay_ms(20);
		KEY_Num = 1;
	}
	
	return KEY_Num;
};