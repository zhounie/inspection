#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"
#include "CountSensor.h"
#include "OLED.h"

uint8_t KeyNum;

uint32_t LEDNum;

int main(void) {
	
	LED_Init();
	KEY_Init();
	CountSensor_Init();
	OLED_Init();

	while(1){
		KeyNum = KEY_GetNum();
		LEDNum = Led_Get();
		if (KeyNum == 1) {
			LED_0_REVERSE();
			OLED_ShowNum(1, 1, LEDNum, 2);
		}
		
	}
}
