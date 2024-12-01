
#ifndef INC_UART_H_
#define INC_UART_H_

#include <control.h>
#include "usart.h"
#include <stdio.h>
#include "utils.h"
#include "gpio.h"

extern uint8_t flag_uart2;
extern uint8_t receive_buffer1;
extern uint8_t receive_buffer2;


void uart_init_rs232();

void uart_Rs232SendString(uint8_t* str);

void uart_Rs232SendBytes(uint8_t* bytes, uint16_t size);

void uart_Rs232SendNum(uint32_t num);

void uart_Rs232SendNumPercent(uint32_t num);

void uart_init_esp();
void uart_EspSendBytes(uint8_t* bytes, uint16_t size);
void uart_EspSendString(uint8_t* str);
#endif /* INC_UART_H_ */
