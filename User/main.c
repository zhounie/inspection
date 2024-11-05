#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"
#include "CountSensor.h"

uint8_t KeyNum;

int main(void) {
	
	LED_Init();
	KEY_Init();
	CountSensor_Init();
	
	while(1){
		KeyNum = KEY_GetNum();
		if (KeyNum == 1) {
			LED_0_REVERSE();
		}
		
	}
}
