#include "MenuHandler.h"
#include "DebugMacros.h"

const char* const MenuHandler::menuOptions[MenuHandler::menuSize] = {
    "Temperatura",
    "Velocidade",
    "Microstepping"};

MenuHandler::MenuHandler(ThermistorHandler* termistor, EncoderHandler* encoder, MotorHandler* motor, ExtruderHandler* extruder)
    : currentOption(0), termistor(termistor), encoder(encoder), motor(motor), extruder(extruder) {}

void MenuHandler::navigateMenu() {
    encoderButton = encoder->readButton(1000);
    encoderPosition = encoder->readPosition();

    switch (currentScreen) {
        case Screen::Inicio:
            if (encoderPosition.changed) {
                if (encoderPosition.position > lastEncoderPosition.position) {
                    currentOption = (currentOption + 1) % menuSize;
                } else if (encoderPosition.position < lastEncoderPosition.position) {
                    currentOption = (currentOption - 1 + menuSize) % menuSize;
                }
                lastEncoderPosition = encoderPosition;
                printMenu();
            }
            if (encoderButton.pressed) {
                currentScreen = Screen::Submenu;
                settingSaved = false;
                printSubmenu();
            }
            break;

        case Screen::Submenu:
            switch (currentOption) {
                case 0:
                    alterarTemperatura();
                    break;
                case 1:
                    alterarVelocidade();
                    break;
                case 2:
                    alterarMicrostepping();
                    break;
                default:
                    currentScreen = Screen::Inicio;
                    break;
            }
            break;

        default:
            currentScreen = Screen::Inicio;
            break;
    }
}

void MenuHandler::printMenu() {
    Serial.println("=== Menu Principal ===");
    for (uint8_t i = 0; i < menuSize; ++i) {
        if (i == currentOption)
            Serial.print("> ");
        else
            Serial.print("  ");
        Serial.println(menuOptions[i]);
    }
    Serial.println();
}

void MenuHandler::printSubmenu() {
    Serial.print("Ajustando: ");
    Serial.println(menuOptions[currentOption]);
}

void MenuHandler::alterarTemperatura() {
    printSubmenu();
    while (!settingSaved) {
        encoderButton = encoder->readButton(1000);
        encoderPosition = encoder->readPosition();
        motor->run();

        if (encoderButton.pressed) {
            DEBUG_PRINTLN("Temperatura definida!");
            settingSaved = true;
            currentScreen = Screen::Inicio;
            break;
        }
        if (encoderPosition.changed) {
            Serial.print("Temperatura: ");
            Serial.print(encoderPosition.position);
            Serial.println(" °C");
            extruder->setTemperature(encoderPosition.position);
        }
    }
}

void MenuHandler::alterarVelocidade() {
    printSubmenu();

    while (!settingSaved) {
        encoderButton = encoder->readButton(1000);
        encoderPosition = encoder->readPosition();
        motor->setRotation(encoderPosition.position);
        motor->run();

        int rpm = motor->getRPM();

        if (rpm != oldRpm) {
            Serial.print("RPM: ");
            Serial.println(rpm);

            oldRpm = rpm;
        }

        if (encoderButton.pressed) {
            DEBUG_PRINTLN("Velocidade definida!");
            settingSaved = true;
            currentScreen = Screen::Inicio;
            break;
        }
    }
}

void MenuHandler::alterarMicrostepping() {
    printSubmenu();

    const char* microstepOptions[] = {
        "Passo completo",
        "1/2 passo",
        "1/4 passo",
        "1/8 passo",
        "1/16 passo"};
    const int numOptions = sizeof(microstepOptions) / sizeof(microstepOptions[0]);
    int selected = static_cast<int>(microstepping);

    bool updated = true;
    while (!settingSaved) {
        encoderButton = encoder->readButton(1000);
        encoderPosition = encoder->readPosition();
        motor->run();

        // Navegação pelas opções
        if (encoderPosition.changed) {
            if (encoderPosition.position > lastEncoderPosition.position) {
                selected = (selected + 1) % numOptions;
            } else if (encoderPosition.position < lastEncoderPosition.position) {
                selected = (selected - 1 + numOptions) % numOptions;
            }
            lastEncoderPosition = encoderPosition;
            updated = true;

            // Altera o microstepping imediatamente ao mudar a seleção
            microstepping = static_cast<MotorHandler::MicrosteppingMode>(selected);
            motor->setMicrostepping(microstepping);
            DEBUG_PRINT("Microstepping alterado: ");
            DEBUG_PRINTLN(microstepOptions[selected]);
        }

        // Exibe opções se mudou
        if (updated) {
            Serial.println("Selecione o microstepping:");
            for (int i = 0; i < numOptions; ++i) {
                if (i == selected)
                    Serial.print("> ");
                else
                    Serial.print("  ");
                Serial.println(microstepOptions[i]);
            }
            Serial.println();
            updated = false;
        }

        // Confirma seleção
        if (encoderButton.pressed) {
            DEBUG_PRINT("Microstepping definido: ");
            DEBUG_PRINTLN(microstepOptions[selected]);
            settingSaved = true;
            currentScreen = Screen::Inicio;
            break;
        }
    }
}