/*
 * Proyecto_Taco.c
 *
 * Created: 9/08/2026 10:40:24 p. m.
 * Author : Christian
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Librerías propias
#include "Librerias/UART.h"
#include "Librerias/DC.h"
#include "Librerias/Servo.h"

// Variables globales para recordar en qué posición están los servos
// 0 significa que está en 0 grados, 1 significa que está en 180 grados
uint8_t estado_triangulo = 0; 
uint8_t estado_circulo = 0;   

int main(void)
{
    // 1. Inicialización de periféricos y pines
    initUART(103); // Configurar Bluetooth a 9600 baudios
    DC_Init();     // Inicializar Motores DC (Shield L293D)
    init_servo();  // Inicializar Servos (Timer1 - Pines D9 y D10)
    
    // 2. Mover servos a su posición inicial (0 grados)
    pulso_servo_set(servo_1, inicio_servo);
    pulso_servo_set(servo_2, inicio_servo);

    // 3. Habilitar interrupciones globales (CRÍTICO para recibir datos Bluetooth)
    sei();

    while (1)
    {
        if (avilitar_UART()) // Revisa si hay un comando nuevo en el buffer
        {
            char c = leer_UART(); // Leer el carácter recibido

            // Responder a la terminal de la app para confirmar
            UART_writeString("Recibido: [");
            UART_writeChar(c);
            UART_writeString("]\r\n");

            // 4. Lógica de control basada en el botón presionado
            switch(c) {
                
                // --- CONTROL DE MOTORES DC ---
                case 'F': // Flecha Arriba (Forward)
                    DC_SetIzquierdo(200, 1);
                    DC_SetDerecho(200, 1);
                    break;
                case 'B': // Flecha Abajo (Backward)
                    DC_SetIzquierdo(200, 0);
                    DC_SetDerecho(200, 0);
                    break;
                case 'L': // Flecha Izquierda (Left)
                    DC_SetIzquierdo(200, 0);
                    DC_SetDerecho(200, 1);
                    break;
                case 'R': // Flecha Derecha (Right)
                    DC_SetIzquierdo(200, 1);
                    DC_SetDerecho(200, 0);
                    break;
                case 'S': // Botón Pause o comando de detener
                case 'X': // Botón Equis (Cruz)
                    DC_Stop();
                    break;

                // --- CONTROL DE SERVOMOTORES ---
                case 'T': // Botón Triángulo -> Controla servo_1 (Pin D9)
                    if (estado_triangulo == 0) {
                        pulso_servo_set(servo_1, maximo_servo); // Mover a 180°
                        estado_triangulo = 1;                   // Guardar nuevo estado
                    } else {
                        pulso_servo_set(servo_1, inicio_servo); // Mover a 0°
                        estado_triangulo = 0;                   // Guardar nuevo estado
                    }
                    break;

                case 'C': // Botón Círculo -> Controla servo_2 (Pin D10)
                    if (estado_circulo == 0) {
                        pulso_servo_set(servo_2, maximo_servo); // Mover a 180°
                        estado_circulo = 1;                     // Guardar nuevo estado
                    } else {
                        pulso_servo_set(servo_2, inicio_servo); // Mover a 0°
                        estado_circulo = 0;                     // Guardar nuevo estado
                    }
                    break;
                    
                default:
                    DC_Stop();
                    break;
            }
        }
    }

    return 0;
}