#include <Arduino.h>
#include "config.h"

int  sensores[NUM_SENSORES];
int  sensorMin[NUM_SENSORES];
int  sensorMax[NUM_SENSORES];
int  umbral = UMBRAL_DEFECTO;

void seleccionarCanal(uint8_t canal) {
    digitalWrite(PIN_S0, (canal >> 0) & 1);
    digitalWrite(PIN_S1, (canal >> 1) & 1);
    digitalWrite(PIN_S2, (canal >> 2) & 1);
    digitalWrite(PIN_S3, (canal >> 3) & 1);
    delayMicroseconds(DELAY_MUX_US);
}

int leerSensor(uint8_t canal) {
    seleccionarCanal(canal);
    return analogRead(PIN_SIG);
}

void leerTodos() {
    for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        sensores[i] = leerSensor(i);
    }
}

void leerConLED() {
    digitalWrite(PIN_LON, HIGH);
    delayMicroseconds(200);
    leerTodos();
    digitalWrite(PIN_LON, LOW);
}

bool esSobreLinea(uint8_t i) {
    if (LINEA_NEGRA) return sensores[i] > umbral;
    else             return sensores[i] < umbral;
}

int calcularPosicion() {
    long suma = 0, total = 0;
    for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        int peso = LINEA_NEGRA ? max(0, sensores[i] - umbral)
                               : max(0, umbral - sensores[i]);
        suma  += (long)i * 1000 * peso;
        total += peso;
    }
    if (total == 0) return -1;
    return (int)(suma / total);
}

void imprimirAnalogico() {
    Serial.print("ANALOG: ");
    for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        Serial.print("S");
        if (i < 10) Serial.print("0");
        Serial.print(i); Serial.print(":");
        if (sensores[i] < 1000) Serial.print(" ");
        if (sensores[i] < 100)  Serial.print(" ");
        if (sensores[i] < 10)   Serial.print(" ");
        Serial.print(sensores[i]);
        Serial.print("  ");
    }
    Serial.println();
}

void imprimirBinario() {
    Serial.print("BIN:    ");
    for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        Serial.print(esSobreLinea(i) ? "1" : "0");
        Serial.print(" ");
    }
    Serial.println();
}

void imprimirBarra() {
    Serial.print("BARRA:  ");
    for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        Serial.print(esSobreLinea(i) ? "#" : ".");
        Serial.print(" ");
    }
    Serial.println();
}

void imprimirPosicion() {
    int pos = calcularPosicion();
    Serial.print("POS:    ");
    if (pos == -1) { Serial.println("NO DETECTADA"); return; }
    Serial.print(pos);
    Serial.print("\t(centro=7500)  [");
    int cursor = map(pos, 0, 15000, 0, 15);
    for (uint8_t i = 0; i <= 15; i++)
        Serial.print(i == (uint8_t)cursor ? "|" : "-");
    Serial.println("]");
}

void setup() {
    Serial.begin(115200);
    while (!Serial);          // espera que la PC detecte el USB
    delay(500);

    pinMode(PIN_S0,  OUTPUT);
    pinMode(PIN_S1,  OUTPUT);
    pinMode(PIN_S2,  OUTPUT);
    pinMode(PIN_S3,  OUTPUT);
    pinMode(PIN_LON, OUTPUT);
    pinMode(LED,     OUTPUT);

    digitalWrite(PIN_LON, HIGH);
    digitalWrite(LED,     HIGH);

    analogReadResolution(ADC_BITS);
    seleccionarCanal(0);

    Serial.println("\n============================================");
    Serial.println("  16x QRE1113 + 74HC4067 | Blue Pill STM32");
    Serial.println("============================================");
    Serial.print("  Umbral: "); Serial.println(umbral);
    Serial.print("  Linea:  "); Serial.println(LINEA_NEGRA ? "NEGRA" : "BLANCA");
    Serial.println("============================================\n");

    for (uint8_t i = 0; i < 3; i++) {
        digitalWrite(LED, LOW);  delay(150);
        digitalWrite(LED, HIGH); delay(150);
    }
}

void loop() {
    leerConLED();
    imprimirAnalogico();
    imprimirBinario();
    imprimirBarra();
    imprimirPosicion();
    Serial.println("--------------------------------------------");
    delay(DELAY_LOOP_MS);
}