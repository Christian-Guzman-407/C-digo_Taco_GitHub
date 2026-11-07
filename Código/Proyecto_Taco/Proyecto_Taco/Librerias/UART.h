/*
 * UART.h
 *
 * Created: 9/08/2026 10:42:49 p. m.
 *  Author: Christian
 */ 


#ifndef UART_H_
#define UART_H_
#include <avr/io.h>
#define tamano_buffer 64
void initUART(uint16_t ubrr);
void UART_writeChar(char c);
void UART_writeString(char* string);
void UART_writeUint(uint16_t valor);
uint8_t avilitar_UART(void);
char leer_UART(void);
#endif /* UART_H_ */
