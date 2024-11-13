#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include <stdio.h>

uint8_t USART_RX_BUF[USART_REC_LEN];
uint16_t USART_RX_STA;

int fputc(int ch, FILE *f)
{
	while((USART1->SR&0X40)==0); //循环发送,直到发送完毕   
	USART1->DR = (u8)ch;      
	return ch;
}

void USART1_Init(void)
{	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		
		USART_Init(USART1, &USART_InitStructure);
		
		USART_Cmd(USART1, ENABLE); // 使能串口
		
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开中断
	
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_Init(&NVIC_InitStructure);
		
}

void USART1_IRQHandler(void)
{
	uint8_t res;
	// 接收中断(接收到的数据必须是0x0d 0x0a结尾)
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		// 读取接收到的数据
		res = USART_ReceiveData(USART1);
		// 接受未完成
		if ((USART_RX_STA&0x8000) == 0) {
			// 接收到了0x0d
			if (USART_RX_STA&0x4000) {
				if (res != 0x0a) { // 接收错误，重新开始
					USART_RX_STA = 0;
				} else { // 接收完成
					USART_RX_STA |= 0x8000;
				}
			} else { //还没有收到0x0d
				if (res == 0x0d) {
					USART_RX_STA |= 0x4000;
				} else {
					USART_RX_BUF[USART_RX_STA&0x3FFF] = res;
					USART_RX_STA++;
					// 接收数据错误，重新开始接收
					if (USART_RX_STA > (USART_REC_LEN - 1)) {
						USART_RX_STA = 0;
					}
				}
			}
		}
	}
}
