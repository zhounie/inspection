#ifndef __ESP8266_H
#define __ESP8266_H

#include <string.h>

#define AT          "AT\r\n"	
#define CWMODE      "AT+CWMODE=1\r\n"		//STA+AP模式
#define RST         "AT+RST\r\n"
#define CIFSR       "AT+CIFSR\r\n"
#define CWJAP       "AT+CWJAP=\"nb\",\"123456789\"\r\n"	//ssid: onenet 密码：空
#define CIPSTART    "AT+CIPSTART=\"TCP\",\"192.168.23.1\",4001\r\n"		//HTTP服务器183.230.40.33/80
#define CIPMODE0    "AT+CIPMODE=0\r\n"		//非透传模式
#define CIPMODE1    "AT+CIPMODE=1\r\n"		//透传模式
#define CIPSEND     "AT+CIPSEND\r\n"
#define CIPSTATUS   "AT+CIPSTATUS\r\n"		//网络状态查询

uint8_t* Esp8266_CHECK_CMD(uint8_t *str);
uint8_t Esp8266_SEND_CMD(uint8_t *cmd, uint8_t *ack, uint16_t waittime);
void Esp8266_START_TRANS(uint16_t clientId);
uint8_t Esp8266_STOP_TRANS(void);
uint8_t* Esp8266_SEND_DATA(uint8_t *cmd, uint16_t waittime);

#endif
