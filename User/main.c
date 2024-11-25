#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"
#include "CountSensor.h"
#include "OLED.h"
// #include "Serial.h"
#include "USART.h"
#include "USART3.h"
#include "ESP8266.h"
#include "DHT11.h"
#include "Buzzer.h"
#include "Fan.h"
#include "AD.h"
#include <stdio.h>
#include <math.h>

uint8_t KeyNum;

uint32_t LEDNum;

uint8_t RxData;

char str[20];

uint8_t DHT_Buffer[5];

uint8_t isOpenAlarm = 1;

void checkAlarm (void)
{
	if (
			DHT_Buffer[2] >= 30 ||
			DHT_Buffer[0] >= 70 ||
			AD_GetMq2() >= 20 ||
			AD_GetLx() <= 30
	) {
			Buzzer_Solo2();
			Open_Fan();
			LED_1_ON();
			Delay_ms(100);
			LED_1_OFF();
			Delay_ms(100);
	} else {
			Close_Fan();
	}
}


int main(void) {
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	LED_Init();
	KEY_Init();
	CountSensor_Init();
	OLED_Init();
	USART1_Init();
	USART3_Init();
	Buzzer_Init();
	Fan_Init();
	AD_Init();

	Esp8266_START_TRANS();
//	Esp8266_SEND_CMD("AT+MQTTPUB=0\,\"/device/set\"\,\"{\"deviceId\":\"dong1\"\,\"temp\": 43}\"\,0\,0", "OK", 50);
//	Esp8266_STOP_TRANS();
	

	while(1){
		sprintf(str, "LUX: %.2f%%", AD_GetLx());
		OLED_ShowString(1, 1, str);
		sprintf(str, "PPM: %.2fppm", AD_GetMq2());
		OLED_ShowString(2, 1, str);
		
//  OLED_ShowNum(3, 1, AD_GetLx(), 4);
//	OLED_ShowNum(3, 6, AD_Value[1], 4);
//	OLED_ShowNum(3, 11, AD_Value[2], 4);

		if(DHT_Get_Temp_Humi_Data(DHT_Buffer))
		{
			
			printf("Temp %d %d  Humi %d %d \r\n", DHT_Buffer[2], DHT_Buffer[3], DHT_Buffer[0] ,DHT_Buffer[1]);
			sprintf(str, "Temp: %d %d", DHT_Buffer[2], DHT_Buffer[3]);
			OLED_ShowString(3, 1, str);
			sprintf(str, "Humi: %d %d", DHT_Buffer[0], DHT_Buffer[1]);
			OLED_ShowString(4, 1, str);
		}
		
		checkAlarm();
		
		Delay_ms(100);
	}
}
