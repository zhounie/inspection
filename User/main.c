#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"
#include "CountSensor.h"
#include "OLED.h"
#include "Serial.h"

uint8_t KeyNum;

uint32_t LEDNum;

uint8_t RxData;

int main(void) {
	
	LED_Init();
	KEY_Init();
	CountSensor_Init();
	OLED_Init();

	Serial_Init();
	
	OLED_ShowString(1, 1, "TXPACKET");
	
	OLED_ShowString(3, 1, "RXPACKET");
	
	Serial_TxPacket[0] = 0x01;
	Serial_TxPacket[1] = 0x02;
	Serial_TxPacket[2] = 0x03;
	Serial_TxPacket[3] = 0x04;
//	Serial_SendPacket();
//	
//	Serial_SendByte(0x41);
//	
//	uint8_t MyArray[] = {0x41, 0x42, 0x44, 0x45};
//	Serial_SendArray(MyArray, 4);
//	
//	Serial_SendString("Hellonima!\r\n");
//	Serial_SendNumber(123456, 4);
//	
//	Serial_Printf("Num:%d\r\n", 666);
//	
//	OLED_ShowString(1, 1, "RxData:");
	while(1){
		KeyNum = KEY_GetNum();
//		LEDNum = Led_Get();
		if (KeyNum == 1) {
			Serial_TxPacket[0]++;
			Serial_TxPacket[1]++;
			Serial_TxPacket[2]++;
			Serial_TxPacket[3]++;
			Serial_SendPacket();
			OLED_ShowHexNum(2, 1, Serial_TxPacket[0], 2);
			OLED_ShowHexNum(2, 4, Serial_TxPacket[1], 2);
			OLED_ShowHexNum(2, 7, Serial_TxPacket[2], 2);
			OLED_ShowHexNum(2, 10, Serial_TxPacket[3], 2);
		}
		if (Serial_GetRxFlag() == 1) {
//			RxData = Serial_GetRxData();
//			Serial_SendByte(RxData);
			OLED_ShowHexNum(4, 1, Serial_RxPacket[0], 2);
			OLED_ShowHexNum(4, 4, Serial_RxPacket[1], 2);
			OLED_ShowHexNum(4, 7, Serial_RxPacket[2], 2);
			OLED_ShowHexNum(4, 10, Serial_RxPacket[3], 2);
		}
		
	}
}
