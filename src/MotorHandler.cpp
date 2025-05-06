#include <Arduino.h>
#include "MotorHandler.h"
#include "PinsConfig.h"
#include "DebugMacros.h"

MotorHandler::MotorHandler()
    : driver(AccelStepper::DRIVER, Pins::Driver_STEP, Pins::Driver_DIR), currentMicrostepping(FullStep) {
    driver.setMaxSpeed(5000);
    driver.setAcceleration(500);

    setMicrostepping(currentMicrostepping);
}

void MotorHandler::setMicrostepping(MicrosteppingMode microstepping) {
    if (microstepping == currentMicrostepping)
        return;

    currentMicrostepping = microstepping;

    switch (microstepping) {
        case HalfStep:
            DEBUG_PRINTLN("Microstepping: 1/2 passo");
            digitalWrite(Pins::Driver_M0, LOW);
            digitalWrite(Pins::Driver_M1, LOW);
            digitalWrite(Pins::Driver_M2, HIGH);
            driver.setMaxSpeed(4000);
            driver.setAcceleration(400);
            microstepFactor = 2;
            break;

        case QuarterStep:
            DEBUG_PRINTLN("Microstepping: 1/4 passo");
            digitalWrite(Pins::Driver_M0, LOW);
            digitalWrite(Pins::Driver_M1, HIGH);
            digitalWrite(Pins::Driver_M2, LOW);
            driver.setMaxSpeed(3000);
            driver.setAcceleration(300);
            microstepFactor = 4;
            break;

        case EighthStep:
            DEBUG_PRINTLN("Microstepping: 1/8 passo");
            digitalWrite(Pins::Driver_M0, LOW);
            digitalWrite(Pins::Driver_M1, HIGH);
            digitalWrite(Pins::Driver_M2, HIGH);
            driver.setMaxSpeed(2000);
            driver.setAcceleration(200);
            microstepFactor = 8;
            break;

        case SixteenthStep:
            DEBUG_PRINTLN("Microstepping: 1/16 passo");
            digitalWrite(Pins::Driver_M0, HIGH);
            digitalWrite(Pins::Driver_M1, LOW);
            digitalWrite(Pins::Driver_M2, LOW);
            driver.setMaxSpeed(1000);
            driver.setAcceleration(100);
            microstepFactor = 16;
            break;

        default:
            DEBUG_PRINTLN("Microstepping: Passo completo - Default");
            digitalWrite(Pins::Driver_M0, LOW);
            digitalWrite(Pins::Driver_M1, LOW);
            digitalWrite(Pins::Driver_M2, LOW);
            driver.setMaxSpeed(5000);
            driver.setAcceleration(500);
            microstepFactor = 1;
            break;
    }
}

const char* MotorHandler::getMicrostepping() const {
    switch (currentMicrostepping) {
        case MotorHandler::FullStep:
            return "Passo completo - Default";
        case MotorHandler::HalfStep:
            return "1/2 passo";
        case MotorHandler::QuarterStep:
            return "1/4 passo";
        case MotorHandler::EighthStep:
            return "1/8 passo";
        case MotorHandler::SixteenthStep:
            return "1/16 passo";
        default:
            return "Configuração de passo inválida";
    }
}

void MotorHandler::setRotation(int value) {
    driver.setSpeed(value * 10);
}

int MotorHandler::getRPM() {
    constexpr int stepsPerRevolution = 200;

    if (microstepFactor == 0) {
        return 0;  // Evita divisão por zero
    }

    float stepsPerSecond = driver.speed();
    float rpm = (stepsPerSecond * 60.0) / (stepsPerRevolution * microstepFactor);

    return round(rpm);
}

void MotorHandler::run() {
    driver.runSpeed();
}