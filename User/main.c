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

volatile uint16_t *uuid = ((__IO uint16_t *)(0x1FFFF7E8));

void checkAlarm (void)
{
	if (AD_GetLx() <= 30) {
		LED_1_ON();
	} else {
		LED_1_OFF();
	}
	if (DHT_Buffer[2] >= 30 || 
			DHT_Buffer[0] >= 70
	) {
			Open_Fan();
	} else {
			Close_Fan();
	}
	if (
			AD_GetMq2() >= 20
	) {
			Buzzer_Solo2();
			LED_0_ON();
			Delay_ms(100);
			LED_0_OFF();
			Delay_ms(100);
	}
	sprintf(str, "AT+MQTTPUB=0,\"/device/set\",\"{\\\"uuid\\\":%d\\,\\\"temp\\\":%.2f\\,\\\"humi\\\":%.2f\\,\\\"mq2\\\":%.2f\\,\\\"lux\\\":%.2f}\",0,0",*uuid, ((float)DHT_Buffer[2] + (float)DHT_Buffer[3]/10), ((float)DHT_Buffer[0]+((float)DHT_Buffer[1]/10)), AD_GetMq2(),AD_GetLx());
	Esp8266_SEND_CMD(str, "OK", 50);
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
	OLED_ShowString(1, 1, "wifi connecting");

	OLED_ShowNum(1, 1, *uuid, 5);
	Esp8266_START_TRANS(*uuid);
	OLED_ShowString(1, 1, "               ");

//	Esp8266_SEND_CMD("AT+MQTTPUB=0\,\"/device/set\"\,\"{\"deviceId\":\"dong1\"\,\"temp\": 43}\"\,0\,0", "OK", 50);
//	Esp8266_STOP_TRANS();
	

	while(1){
		sprintf(str, "LUX: %.2f%%", AD_GetLx());
		OLED_ShowString(1, 1,str);
		sprintf(str, "PPM: %.2fppm", AD_GetMq2());
		OLED_ShowString(2, 1, str);
		
//		if (KEY_GetNum() == 1) {
//				Esp8266_START_TRANS(*uuid);
//		}
		

		if(DHT_Get_Temp_Humi_Data(DHT_Buffer))
		{
			sprintf(str, "Temp: %.2f", ((float)DHT_Buffer[2] + (float)DHT_Buffer[3]/10));
			OLED_ShowString(3, 1, str);
			sprintf(str, "Humi: %.2f", ((float)DHT_Buffer[0]+((float)DHT_Buffer[1]/10)));
			OLED_ShowString(4, 1, str);
		}
		
		checkAlarm();
		
		Delay_ms(1000);
	}
}
