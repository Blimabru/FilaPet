#ifndef EXTRUDER_HANDLER_H
#define EXTRUDER_HANDLER_H

#include <Arduino.h>
#include "PinsConfig.h"
#include "ThermistorHandler.h"

class ExtruderHandler {
   public:
    ExtruderHandler(ThermistorHandler* termistor);

    uint8_t setTemperature(uint8_t temp);
    void control();

   private:
    ThermistorHandler* termistor;
    const uint8_t mosfetPin = Pins::Mosfet;
    float temperature = 0;
    const float tolerance = 2.0;
};

#endif