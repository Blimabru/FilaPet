#include "ThermistorHandler.h"

#define FILTER_SIZE 64

ThermistorHandler::ThermistorHandler() {
}

TemperatureResult ThermistorHandler::readTemperature() {
    static float readings[FILTER_SIZE] = {0};
    static int index = 0;
    static bool filled = false;

    float currentReading = analogRead(thermistorPin);
    readings[index] = currentReading;
    index = (index + 1) % FILTER_SIZE;
    if (index == 0)
        filled = true;

    // Calcula a média das leituras
    float sum = 0;
    int count = filled ? FILTER_SIZE : index;
    for (int i = 0; i < count; i++) {
        sum += readings[i];
    }
    float average = sum / count;

    // Calcula a tensão de saída (Vout)
    float Vout = (average / 1023.0) * Vin;

    // Verifica se a leitura está fora da faixa permitida
    if (Vout <= 0.1 || Vout >= (Vin - 0.1)) {
        Serial.println("⚠️ Erro: Leitura fora da faixa permitida! Verifique o sensor.");
        return TemperatureResult{0, true};
    }

    // Calcula a resistência do termistor
    float R_thermistor = R_fixed * ((Vin / Vout) - 1);

    // Verifica se a resistência calculada é inválida
    if (R_thermistor <= 0) {
        Serial.println("⚠️ Erro: Resistência do sensor inválida! Verifique o circuito.");
        return TemperatureResult{0, true};
    }

    // Calcula a temperatura em Celsius
    float temperature = 1.0 / (1.0 / T0 + (1.0 / BETA) * log(R_thermistor / R0)) - 273.15;

    // Define um limite de tolerância para mudanças de temperatura
    const float tolerance = 1;            // Mudanças menores que 1 ºC serão ignoradas
    static float lastTemperature = -999;  // Inicializa com um valor impossível

    // Verifica se a mudança de temperatura é significativa
    if (abs(temperature - lastTemperature) < tolerance) {
        return TemperatureResult{lastTemperature, false};  // Retorna a última temperatura válida
    }

    // Atualiza a última temperatura registrada
    lastTemperature = temperature;

    // Verifica se a temperatura está fora do intervalo permitido
    if (temperature < -1 || temperature > 250) {  // Ajustado para o intervalo típico de termistores
        Serial.println("⚠️ Erro: Temperatura fora do intervalo permitido (-1 a 250 ºC).");
        return TemperatureResult{0, true};
    }

    // Retorna a temperatura calculada
    return TemperatureResult{temperature, false};
}