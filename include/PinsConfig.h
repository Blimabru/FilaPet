#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

#include <Arduino.h>

struct Pins {
    static const uint8_t Thermistor = A0;  // Sensor de temperatura.

    static const uint8_t Mosfet = 5;  // Controle do aquecedor via Mosfet.

    static const uint8_t Driver_STEP = 6;  // STEP: Emissão de pulso, cada pulso move o motor em um passo.
    static const uint8_t Driver_DIR = 7;   // DIR: Define o sentido de rotação (horário/anti-horário).

    static const uint8_t Encoder_CLK = 2;  // Clock: Detecta a mudança de posição e direção.
    static const uint8_t Encoder_DT = 3;   // Data: Indica a direção da rotação em conjunto com o CLK.
    static const uint8_t Encoder_SW = 4;   // Switch: Botão integrado ao encoder rotativo.

    static const uint8_t Display_CS = 10;   // Chip Select: Seleciona o display para comunicação SPI.
    static const uint8_t Display_RSE = 8;   // Reset Enable: Reseta o display.
    static const uint8_t Display_RS = 9;    // Register Select: Seleciona entre enviar comandos ou dados para o display.
    static const uint8_t Display_SCL = 13;  // Serial Clock: Clock para a comunicação serial SPI com o display.
    static const uint8_t Display_SI = 11;   // Serial Input: Comunicação SPI com o display.

    static void setup() {
        pinMode(Thermistor, INPUT);
        pinMode(Mosfet, OUTPUT);
        pinMode(Driver_STEP, OUTPUT);
        pinMode(Driver_DIR, OUTPUT);
        pinMode(Encoder_CLK, INPUT);
        pinMode(Encoder_DT, INPUT);
        pinMode(Encoder_SW, INPUT_PULLUP);
        pinMode(Display_CS, OUTPUT);
        pinMode(Display_RSE, OUTPUT);
        pinMode(Display_RS, OUTPUT);
        pinMode(Display_SCL, OUTPUT);
        pinMode(Display_SI, OUTPUT);
    }
};

#endif