#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include <AccelStepper.h>

class MotorHandler {
   public:
    enum MicrosteppingMode {
        FullStep = 0,      // Passo completo
        HalfStep = 1,      // 1/2 passo
        QuarterStep = 2,   // 1/4 passo
        EighthStep = 3,    // 1/8 passo
        SixteenthStep = 4  // 1/16 passo
    };

    MotorHandler();
    void setMicrostepping(MicrosteppingMode microstepping);
    void setRotation(int value);
    void run();
    const char* getMicrostepping() const;
    int getRPM();

   private:
    AccelStepper driver;
    MicrosteppingMode currentMicrostepping;
    int microstepFactor = 1;
};

#endif