#ifndef THERMISTOR_HANDLER_H
#define THERMISTOR_HANDLER_H

#include "PinsConfig.h"

struct TemperatureResult {
    float value;
    bool error;
};

class ThermistorHandler {
   private:
    const float Vin = 5.0;                           // Tensão de entrada (5V)
    const float R_fixed = 4700.0;                    // Resistor fixo de 4.7kΩ
    const float BETA = 3950.0;                       // Constante BETA do termistor
    const float T0 = 298.15;                         // Temperatura de referência (25 ºC em Kelvin)
    const float R0 = 100000.0;                       // Resistência do termistor a 25 ºC
    const uint8_t thermistorPin = Pins::Thermistor;  // Pino do termistor (obtido do arquivo PinsConfig.h)

   public:
    ThermistorHandler();
    TemperatureResult readTemperature();
};

#endif