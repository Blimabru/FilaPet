#ifndef ENCODER_HANDLER_H
#define ENCODER_HANDLER_H

#include <Encoder.h>

struct EncoderPosition {
    long position;
    bool changed;
};

struct EncoderButton {
    bool pressed;
    bool held;
};

class EncoderHandler {
   private:
    Encoder encoder;
    uint8_t pinSW;
    long lastPosition = 0;
    bool lastButtonState = HIGH;
    unsigned long buttonPressStartTime = 0;
    bool heldEventFired = false;

   public:
    EncoderHandler();
    EncoderPosition readPosition();
    EncoderButton readButton(unsigned long holdTime);
};

#endif