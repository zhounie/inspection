#ifndef __USART3_H
#define __USART3_H	 

#include <stdint.h>
#define USART3_MAX_RECV_LEN 600 //最大接收缓存字节数
#define USART3_MAX_SEND_LEN 600 //最大发送缓存字节数
#define USART3_RX_EN 1 // 0不接收，1接受

extern uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN];
extern uint8_t USART3_TX_BUF[USART3_MAX_SEND_LEN];
extern volatile uint16_t USART3_RX_STA;

void USART3_Init(void);
void USART3_Printf(char *format, ...);

#endif
