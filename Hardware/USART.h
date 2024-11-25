#ifndef __USART_H
#define __USART_H	 

#include <stdio.h>
#include <stdint.h>

#define USART_REC_LEN 200 //最大接收缓存字节数
#define USART1_RX_EN 1 // 0不接收，1接受

extern uint8_t USART_RX_BUF[USART_REC_LEN];
extern uint16_t USART_RX_STA;

void USART1_Init(void);

#endif
