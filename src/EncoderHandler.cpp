#include <Arduino.h>
#include "EncoderHandler.h"
#include "PinsConfig.h"

EncoderHandler::EncoderHandler()
    : encoder(Pins::Encoder_CLK, Pins::Encoder_DT), pinSW(Pins::Encoder_SW) {
}

EncoderPosition EncoderHandler::readPosition() {
    long currentPosition = (encoder.read() / 4) * -1;
    bool changed = (currentPosition != lastPosition);

    if (changed) {
        lastPosition = currentPosition;
    }

    return EncoderPosition{currentPosition, changed};
}

EncoderButton EncoderHandler::readButton(unsigned long holdTime) {
    bool newButtonState = digitalRead(pinSW);
    bool pressed = false;
    bool held = false;

    // Detecta início do pressionamento
    if (lastButtonState == HIGH && newButtonState == LOW) {
        buttonPressStartTime = millis();
        heldEventFired = false;
    }

    // Se o botão está sendo segurado
    if (lastButtonState == LOW && newButtonState == LOW) {
        if ((millis() - buttonPressStartTime) >= holdTime && !heldEventFired) {
            held = true;
            heldEventFired = true;
        }
    }

    // Detecta soltura do botão
    if (lastButtonState == LOW && newButtonState == HIGH) {
        // Se foi solto antes do tempo de "held", considera "pressed"
        if ((millis() - buttonPressStartTime) < holdTime) {
            pressed = true;
        }
    }

    lastButtonState = newButtonState;

    return EncoderButton{pressed, held};
}