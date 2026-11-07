/*
 * Servo.c
 *
 * Created: 9/08/2026 10:44:21 p. m.
 *  Author: Christian
 */ 

#include "Servo.h"
#include <avr/io.h>
#include <stdint.h>

// Tabla de pulsos actuales de los 2 servos
// Ya no necesitan ser 'volatile' porque no hay ISR modificándolos
uint16_t pulso_servo[contador_servo] = {
	servo_centrado,  // Servo 1
	servo_centrado   // Servo 2
};

void init_servo(void)
{
	// Configurar pines de salida solo para los 2 servos
	DDRB |= (1 << DDB1);   // D9  -> Servo 1 (OC1A)
	DDRB |= (1 << DDB2);   // D10 -> Servo 2 (OC1B)

	// Timer1: Fast PWM con ICR1 como TOP
	// WGM13:WGM12:WGM11:WGM10 = 1110 -> Fast PWM, TOP = ICR1
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13)  | (1 << WGM12)  | (1 << CS11);  // Prescaler /8

	// Período de 20ms para servos
	ICR1 = 39999;

	// Posición inicial de los servos de hardware
	OCR1A = servo_centrado;   // D9
	OCR1B = servo_centrado;   // D10
}

void pulso_servo_set(uint8_t idx, uint16_t pulso)
{
	// Primero validar índice
	if (idx >= contador_servo) return;

	// Limitar ANTES de guardar
	if (pulso < inicio_servo) pulso = inicio_servo;
	if (pulso > maximo_servo) pulso = maximo_servo;

	// Guardar ya limitado
	pulso_servo[idx] = pulso;

	// Escribir directamente en los registros del Timer1
	if (idx == servo_1) OCR1A = pulso;
	if (idx == servo_2) OCR1B = pulso;
}

uint16_t pulso_servo_get(uint8_t idx)
{
	if (idx >= contador_servo)
	{
		return servo_centrado;
	}
	return pulso_servo[idx];
}

uint16_t pulso_ADC(uint16_t adc)
{
	// 0–1023 -> inicio_servo–maximo_servo
	return inicio_servo + (uint16_t)(((uint32_t)adc * (maximo_servo - inicio_servo)) / 1023);
}