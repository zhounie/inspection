#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#define time1 50
#define hz1 1
#define time2 200
#define hz2 500

void Buzzer_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void Buzzer_Solo1(void) {
		uint16_t i;
    for(i = 0; i < time1; i++){
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
			Delay_ms(hz1);
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
      Delay_ms(hz1);
    }
}


void Buzzer_Solo2(void) {
		uint16_t i;
    for(i = 0; i < time2; i++){
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
			Delay_us(hz2);
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
      Delay_us(hz2);
    }
}
