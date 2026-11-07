/*
 * DC.c
 *
 * Created: 9/08/2026 10:48:51 p. m.
 *  Author: Christian
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "DC.h"
#include <avr/io.h>
#include <util/delay.h>

#define ENA_MASK   (1 << PD3)  /* D3  / OC2B */
#define IN1_MASK   (1 << PD4)  /* D4         */
#define IN2_MASK   (1 << PD7)  /* D7         */
#define ENB_MASK   (1 << PB3)  /* D11 / OC2A */
#define IN3_MASK   (1 << PB0)  /* D8         */
#define IN4_MASK   (1 << PB4)  /* D12        */
#define IZQ_MASK   (IN1_MASK | IN2_MASK)
#define DER_MASK   (IN3_MASK | IN4_MASK)
#define COM_A_MASK ((1 << COM2A1) | (1 << COM2A0))
#define COM_B_MASK ((1 << COM2B1) | (1 << COM2B0))

static void pwm_izquierdo(uint8_t speed)
{
    /* Preparar LOW en PORT antes de desconectar la salida PWM.
     * En Fast PWM, OCR=0 solo no garantiza un LOW continuo.
     */
    PORTD &= ~ENA_MASK;
    if (speed == 0) {
        TCCR2A &= ~COM_B_MASK;
        OCR2B = 0;
    } else {
        OCR2B = speed;
        TCCR2A = (TCCR2A & ~COM_B_MASK) | (1 << COM2B1);
    }
}

static void pwm_derecho(uint8_t speed)
{
    PORTB &= ~ENB_MASK;
    if (speed == 0) {
        TCCR2A &= ~COM_A_MASK;
        OCR2A = 0;
    } else {
        OCR2A = speed;
        TCCR2A = (TCCR2A & ~COM_A_MASK) | (1 << COM2A1);
    }
}

void DC_Init(void)
{
    /* Timer2 es exclusivo de los motores. Timer1 queda para servos. */
    TCCR2B = 0;
    TCCR2A = 0;
    TIMSK2 = 0;

    PORTD &= ~(ENA_MASK | IZQ_MASK);
    PORTB &= ~(ENB_MASK | DER_MASK);
    DDRD |= ENA_MASK | IZQ_MASK;
    DDRB |= ENB_MASK | DER_MASK;

    TCNT2 = 0;
    OCR2A = 0;
    OCR2B = 0;
    /* Fast PWM, TOP=255, prescaler=64: 976.5625 Hz a 16 MHz.
     * OC2A/OC2B permanecen desconectados hasta pedir speed > 0.
     */
    TCCR2A = (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS22);
}

void DC_SetIzquierdo(uint8_t speed, uint8_t direction)
{
    uint8_t adelante = (direction != 0) ^ (DC_INVERTIR_IZQUIERDO != 0);
    uint8_t valor = adelante ? IN1_MASK : IN2_MASK;

    if (speed == 0) {
        pwm_izquierdo(0);
        PORTD &= ~IZQ_MASK;
        return;
    }

    if ((PORTD & IZQ_MASK) != valor) {
        pwm_izquierdo(0);
        _delay_us(20);
        PORTD = (PORTD & ~IZQ_MASK) | valor;
        _delay_us(20);
    }
    /* Esta pausa solo separa la conmutacion electrica.
     * NO detiene la inercia del motor ni limita la corriente al invertir.
     */
    pwm_izquierdo(speed);
}

void DC_SetDerecho(uint8_t speed, uint8_t direction)
{
    uint8_t adelante = (direction != 0) ^ (DC_INVERTIR_DERECHO != 0);
    uint8_t valor = adelante ? IN3_MASK : IN4_MASK;

    if (speed == 0) {
        pwm_derecho(0);
        PORTB &= ~DER_MASK;
        return;
    }

    if ((PORTB & DER_MASK) != valor) {
        pwm_derecho(0);
        _delay_us(20);
        PORTB = (PORTB & ~DER_MASK) | valor;
        _delay_us(20);
    }
    pwm_derecho(speed);
}

void DC_Stop(void)
{
    /* Ambas entradas ENABLE quedan fisicamente en LOW.
     * Parada libre, sin frenado activo.
     */
    PORTD &= ~ENA_MASK;
    PORTB &= ~ENB_MASK;
    TCCR2A &= ~(COM_A_MASK | COM_B_MASK);
    OCR2A = 0;
    OCR2B = 0;
    PORTD &= ~IZQ_MASK;
    PORTB &= ~DER_MASK;
}
