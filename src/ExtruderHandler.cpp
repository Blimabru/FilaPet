#include "ExtruderHandler.h"

ExtruderHandler::ExtruderHandler(ThermistorHandler* termistor)
    : termistor(termistor) {
    pinMode(mosfetPin, OUTPUT);
    digitalWrite(mosfetPin, LOW);  // Começa desligado
}

uint8_t ExtruderHandler::setTemperature(uint8_t t) {
    temperature = t;
}

void ExtruderHandler::control() {
    TemperatureResult temperatureResult = termistor->readTemperature();
    if (temperatureResult.error) {
        digitalWrite(mosfetPin, LOW);  // Segurança: desliga o aquecedor em caso de erro
        return;
    }

    float currentTemperature = temperatureResult.value;

    // Controle ON/OFF com histerese
    if (currentTemperature < (temperature - tolerance)) {
        // Serial.println("Aquecendo ♨️");
        digitalWrite(mosfetPin, HIGH);  // Liga aquecedor
    } else if (currentTemperature > (temperature + tolerance)) {
        digitalWrite(mosfetPin, LOW);  // Desliga aquecedor
    }
}