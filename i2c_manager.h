// i2c_manager.h
// ============================================
// RESPONSABILIDAD: Hablar con el bus I2C (pines, velocidad, escaneo y verificacion).
// No sabe nada de: OLED, logos, ojos ni comandos del Monitor Serie.
// ============================================

#ifndef I2C_MANAGER_H
#define I2C_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include "config.h"

// TODO 1.1: Levanta el bus I2C compartido con los pines y la velocidad declarados en config.h.
// Pregunta Guía: ¿Qué dos pines y qué velocidad necesita el bus antes de buscar el panel?
// Pista: Los valores viven en config.h; el resultado esperado se describe en la guía §05.
inline void initI2C() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(I2C_FREQUENCY_HZ);
    Serial.println(F("[I2C] bus listo SDA=21 SCL=22"));
}

// TODO 1.2: Barre el rango completo de direcciones e informa cada dispositivo hallado y el conteo final.
// Pregunta Guía: ¿Cómo sabes que el barrido cubrió todo el rango si el monitor solo muestra un conteo?
// Pista: La guía §05 muestra el barrido esperado línea por línea.
inline void scanI2C() {
    int count = 0;
    Serial.println(F("[I2C] escaneo de direcciones"));

    for (uint16_t address = 0; address <= 0x7F; ++address) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.print(F("[I2C] dispositivo encontrado en 0x"));
            Serial.println(address);
            count++;
        }
    }

    Serial.print(F("[I2C] total de dispositivos: "));
    Serial.println(count);
}

// TODO 1.3: Sondea la dirección del panel e informa si responde o si el arranque debe detenerse.
// Pregunta Guía: ¿Qué debe imprimir el arranque cuando el panel no responde?
// Pista: Hay dos caminos, uno de éxito y uno fatal; la guía §05 los muestra.
inline void testI2CDevice() {
    Wire.beginTransmission(OLED_I2C_ADDRESS);
    int error = Wire.endTransmission();

    if (error == 0) {
        Serial.print(F("[I2C] panel OLED responde en 0x"));
        Serial.println(OLED_I2C_ADDRESS);
        return;
    }

    Serial.print(F("[I2C] panel OLED no responde en 0x"));
    Serial.println(OLED_I2C_ADDRESS);
    Serial.println(F("[I2C] arranque detenido"));
    while (true) {
        delay(1000);
    }
}

#endif
