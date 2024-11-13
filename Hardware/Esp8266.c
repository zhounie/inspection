#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Esp8266.h"
#include "USART3.h"

void Esp8266_START_TRANS(void)
{

}

void Esp8266_STOP_TRANS(void)
{

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
	USART3_Printf("%s\r\n", cmd);
	//需要等待应答
	if (ack&&waittime) {
		while(--waittime) {
			Delay_ms(10);
			if (USART3_RX_STA&0x8000) {
				
			}
		}
		if (waittime == 0) {
			res = 1;
		}
	}
	return res;
}

uint8_t* Esp8266_CHECK_CMD()
{

}

uint8_t* Esp8266_SEND_DATA()
{

}


