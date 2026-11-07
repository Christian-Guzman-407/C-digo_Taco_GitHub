/*
 * DC.h
 *
 * Created: 9/08/2026 10:51:48 p. m.
 *  Author: Christian
 */ 

#ifndef DC_H
#define DC_H

#include <stdint.h>

/* Arduino Nano clasico, ATmega328P, 16 MHz.
 * ENA = D3, IN1 = D4, IN2 = D7: par izquierdo.
 * ENB = D11, IN3 = D8, IN4 = D12: par derecho.
 * Retirar los jumpers ENA y ENB antes de conectar el Arduino.
 */

/* Cambiar a 1 si TODO un lado gira al reves al mandar F.
 * Si solo un motor del par gira al reves, invertir sus dos cables.
 */
#ifndef DC_INVERTIR_IZQUIERDO
#define DC_INVERTIR_IZQUIERDO 0
#endif
#ifndef DC_INVERTIR_DERECHO
#define DC_INVERTIR_DERECHO 0
#endif

void DC_Init(void);
/* speed: 0..255. direction: 1 = adelante, 0 = atras.
 * speed = 0 y DC_Stop() deshabilitan el puente: parada libre.
 * Las funciones deben llamarse desde main, no desde interrupciones.
 */
void DC_SetIzquierdo(uint8_t speed, uint8_t direction);
void DC_SetDerecho(uint8_t speed, uint8_t direction);
void DC_Stop(void);

#endif
