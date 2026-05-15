#pragma once
// ============================================================
//  config.h — Configuración central del proyecto
//  Modifica aquí sin tocar main.cpp
// ============================================================

// ── Pines del 74HC4067 ───────────────────────────────────────
//  En el Blue Pill: A0=PA0, A1=PA1, A2=PA2, A3=PA3, A4=PA4
#define PIN_S0          A0
#define PIN_S1          A1
#define PIN_S2          A2
#define PIN_S3          A3
#define PIN_SIG         A4      // Salida analógica del mux → ADC
#define PIN_LON   PB0    // ← esta línea faltaba

// ── Hardware ────────────────────────────────────────────────
#define NUM_SENSORES    16      // Canales QRE1113 en el módulo
#define DELAY_MUX_US    15     // µs de estabilización tras cambiar canal
#define DELAY_LOOP_MS   100    // ms entre lecturas en el monitor serial

// ── Blue Pill: LED integrado ─────────────────────────────────
//  PC13 es activo LOW: LOW=encendido, HIGH=apagado
#define LED             PC13

// ── ADC ─────────────────────────────────────────────────────
#define ADC_BITS        12      // STM32F103 → 12 bits → 0-4095
#define ADC_MAX         4095

// ── Detección de línea ───────────────────────────────────────
//  QRE1113 en pull-up:
//    Negro  → poca reflexión → transistor OFF → voltaje alto → ADC alto
//    Blanco → mucha reflexión → transistor ON  → voltaje bajo → ADC bajo
//
//  UMBRAL: valor que separa negro de blanco.
//  Ajústalo después de ver tus lecturas en el monitor serial.
#define UMBRAL_DEFECTO  2000

//  1 = robot sigue línea NEGRA sobre fondo blanco  (más común)
//  0 = robot sigue línea BLANCA sobre fondo negro
#define LINEA_NEGRA     1

// ── Calibración ─────────────────────────────────────────────
//  Tiempo en ms que dura la calibración automática en setup()
//  Pon en 0 para deshabilitar calibración al encender
#define TIEMPO_CALIBRACION_MS   0      // 0 = sin calibración automática
