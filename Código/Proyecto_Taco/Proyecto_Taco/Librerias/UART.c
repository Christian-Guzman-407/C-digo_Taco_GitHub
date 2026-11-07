/*
 * UART.c
 *
 * Created: 9/08/2026 10:42:29 p. m.
 *  Author: Christian
 */ 

#include "UART.h"
#include <avr/io.h>
#include <avr/interrupt.h> // ¡NECESARIO PARA EL ISR!

volatile char rx_buffer[tamano_buffer];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

void initUART(uint16_t ubrr)
{
	DDRD &= ~(1<<DDD0); // D0 = RX -> entrada
	DDRD |= (1<<DDD1);  // D1 = Tx -> salida
	
	UCSR0A = 0;
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UBRR0 = ubrr;
}

void UART_writeChar(char c)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}

void UART_writeString(char* string)
{
	while (*string) UART_writeChar(*string++);
}

uint8_t avilitar_UART(void)
{
	return rx_head != rx_tail;
}

char leer_UART(void)
{
	if (rx_head == rx_tail) return 0;
	char c = rx_buffer[rx_tail];
	rx_tail = (rx_tail + 1) % tamano_buffer;
	return c;
}

// ✨ ESTA ES LA PIEZA QUE FALTABA ✨
ISR(USART_RX_vect)
{
	char c = UDR0;
	uint8_t siguiente = (rx_head + 1) % tamano_buffer;
	if (siguiente != rx_tail)
	{
		rx_buffer[rx_head] = c;
		rx_head = siguiente;
	}
}
