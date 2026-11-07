/*
 * Servo.h
 *
 * Created: 9/08/2026 10:44:10 p. m.
 *  Author: Christian
 */ 

#ifndef SERVO_H_
#define SERVO_H_
#include <avr/io.h>
#include <stdint.h>

// Índices de cada servo
#define servo_1 0   // D9  (PB1, OC1A)
#define servo_2 1   // D10 (PB2, OC1B)

// Límites de pulso en ticks (Timer1, prescaler /8 a 16MHz -> 0.5us por tick)
#define inicio_servo 1000     // Posición 0°
#define maximo_servo 5000     // Posición 180°
#define servo_centrado 3000   // Posición 90°
#define contador_servo 2

void     init_servo(void);
void     pulso_servo_set(uint8_t idx, uint16_t pulso);
uint16_t pulso_servo_get(uint8_t idx);
uint16_t pulso_ADC(uint16_t valor_ADC);

#endif /* SERVO_H_ */