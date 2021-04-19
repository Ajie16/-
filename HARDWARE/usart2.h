#ifndef   _USART2_H
#define   _USART2_H
#include "stdint.h"
#include "usart.h"

extern u8 USART2_RX_BUF[USART_REC_LEN];
void usart2_init(uint32_t bound);
void usart2_send(uint8_t *data,uint8_t len);

#endif

