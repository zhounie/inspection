#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Esp8266.h"
#include "USART.h"
#include "USART3.h"

void Esp8266_START_TRANS(void)
{
	Esp8266_SEND_CMD("AT+CWMODE=1", "OK", 50);

	
	Esp8266_SEND_CMD("AT+RST", "ready", 20);
	
	Delay_ms(2000);
//	
	Esp8266_SEND_CMD("AT+CWJAP=\"Xiaomi_2302\",\"15870970217\"", "WIFI DISCONNECT", 600);

	Delay_ms(2000);
	
	Esp8266_SEND_CMD("AT+CIPMUX=0", "OK", 20);
	
	//建立TCP连接  这四项分别代表了 要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
//	while(Esp8266_SEND_CMD("AT+CIPSTART=\"TCP\",\"192.168.31.234\",5500", "CONNECT", 200));
	
	Esp8266_SEND_CMD("AT+CIPMODE=1", "OK", 200);
	Delay_ms(2000);
	
	Esp8266_SEND_CMD("AT+MQTTUSERCFG=0\,1\,\"dong1\",\"user\",\"123456\"\,0\,0\,\"\"", "OK", 50);
		Delay_ms(2000);
	Esp8266_SEND_CMD("AT+MQTTCONN=0\,\"192.168.31.14\"\,1883\,1", "OK", 50);
		Delay_ms(2000);
	Esp8266_SEND_CMD("AT+MQTTPUB=0,\"/device/set\",\"{\"deviceId\":\"dong1\"\,\"temp\": 22}\",0,0", "OK", 50);
	
	
	
	// Esp8266_SEND_CMD("AT+CIPSEND", "OK", 50);
	
	
}

uint8_t Esp8266_STOP_TRANS(void)
{
	uint8_t result = 1;
	USART3_Printf("+++");
	Delay_ms(1000);
	result = Esp8266_SEND_CMD("AT", "OK", 50); // 退出透传判断
	if (result) {
		printf("quit_trans failed!");
	} else {
		printf("quit_trans success!");
	}
	return result;
}

// 向ESP8266发送数据
// cmd：命令字符串
// ack：期待应答结果
// waittime：等待时间
// 返回值：0发送成功，1发送失败
uint8_t Esp8266_SEND_CMD(uint8_t *cmd, uint8_t *ack, uint16_t waittime)
{
	uint8_t res = 0;
	USART3_RX_STA = 0;
	printf("send= cmd:%s ack:%s\r\n", (uint8_t*)cmd, (uint8_t*)ack);
	USART3_Printf("%s\r\n", cmd);
	//需要等待应答
	if (ack&&waittime) {
		while(--waittime) {
			Delay_ms(10);
			if (USART3_RX_STA&0x8000) { //接收到期待的应答结果
//				if (Esp8266_CHECK_CMD(ack)) {
					printf("revice= cmd:%s ack:%s\r\n", (uint8_t*)cmd, (uint8_t*)ack);
					break;
//				}
				USART_RX_STA = 0;
			}
		}
		if (waittime == 0) {
			res = 1;
		}
	}
	return res;
}

// str：期待的响应结果
// 返回值：0没有得到期待的应答结果；其他，期待应答结果的位置；
uint8_t* Esp8266_CHECK_CMD(uint8_t *str)
{
	char *strx = 0;
	if (USART3_RX_STA&0x8000) { // 接收到一次数据
		USART3_RX_BUF[USART3_RX_STA&0x7FFF] = 0;
		printf("%s\r\n",(const char*)USART3_RX_BUF);
		strx = strstr((const char*)USART3_RX_BUF, (const char*)str);
	}
	return (uint8_t*)strx;
}

uint8_t* Esp8266_SEND_DATA(uint8_t *cmd, uint16_t waittime)
{
	char temp[5];
	char *ack = temp;
	USART3_RX_STA = 0;
	USART3_Printf("%s", cmd); // 发送命令
	if (waittime) { // 等待应答
		while(--waittime) {
			Delay_ms(10);
			if (USART3_RX_STA&0x8000) { // 接收到期待的结果
				USART3_RX_BUF[USART3_RX_STA&0x7FFF] = 0; // 添加结束符
				ack = (char*)USART3_RX_BUF;
				printf("ack:%s\r\n", (uint8_t*)ack);
				USART3_RX_STA = 0;
				break;
			}
		}
	}
	return (uint8_t*)ack;
}


