#ifndef MENU_HANDLER_H
#define MENU_HANDLER_H

#include <Arduino.h>
#include "EncoderHandler.h"
#include "MotorHandler.h"
#include "ThermistorHandler.h"
#include "ExtruderHandler.h"

class MenuHandler {
   public:
    MenuHandler(ThermistorHandler* termistor, EncoderHandler* encoder, MotorHandler* motor, ExtruderHandler* extruder);

    void navigateMenu();
    void printMenu();
    void updateEncoder();

   private:
    enum class Screen : uint8_t {
        Inicio = 0,
        Submenu,
        Configuracao,
        TotalScreens
    };

    static constexpr uint8_t menuSize = 3;
    static const char* const menuOptions[menuSize];

    uint8_t currentOption = 0;
    Screen currentScreen = Screen::Inicio;
    bool settingSaved = false;
    int oldRpm = 0;

    ThermistorHandler* termistor;
    EncoderHandler* encoder;
    MotorHandler* motor;
    ExtruderHandler* extruder;

    EncoderButton encoderButton;
    EncoderPosition encoderPosition;
    EncoderPosition lastEncoderPosition;
    MotorHandler::MicrosteppingMode microstepping = MotorHandler::FullStep;

    void alterarTemperatura();
    void alterarVelocidade();
    void alterarMicrostepping();

    void printSubmenu();
};

#endif