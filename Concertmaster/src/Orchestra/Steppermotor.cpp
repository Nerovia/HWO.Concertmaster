// Included Files
#include "../Orchestra.h"
#include "../Instruments.h"
#include "../Hotswap.h"
#include "../System.h"
#include <Arduino.h>



// Using
using namespace Orchestra;
using namespace Instruments;
using namespace Hotswap;



Steppermotor::Steppermotor(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t number)
{
    this->pin0 = pin0;
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->pin3 = pin3;
    this->number = number;

    pinMode(pin0, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
}


Steppermotor::Steppermotor(HotswapSlot* slot) : 
    Steppermotor(slot->getPin0(), slot->getPin1(), slot->getPin2(), slot->getPin3(), slot->getNumber())
{

}



void Steppermotor::run()
{
    switch (state)
    {
        case 0: // Initialize
            if (currentNote > 0)
            {
                delayStep = 1000L * 1000L / pitches[currentNote];
                timeLast = System::Timing::timeCycleMicros;
                state = 1;
            }
            else
            {
                digitalWrite(pin0, LOW);
                digitalWrite(pin1, LOW);
                digitalWrite(pin2, LOW);
                digitalWrite(pin3, LOW);
                state = 2;
            }
            break;

        case 1: // Play Note
            if (System::Timing::timeCycleMicros - timeLast >= delayStep)
            {
                timeLast = System::Timing::timeCycleMicros;
                if(direction)
                {
                    currentStep++;
                    if(currentStep >= 4) { currentStep = 0; }
                }
                else
                {
                    if(currentStep <= 0) { currentStep = 4; }
                    currentStep--;
                }
                stepMotor(currentStep);
            }
            break;

        case 2: // Play Rest or Idle
            break;
    }
}


void Steppermotor::setNote(uint8_t note)
{
    if (currentNote == note) return;
    if (note >= PITCHES_COUNT) return;
    if (note != 0) direction = !direction;
    currentNote = note;
    state = 0;
}


void Steppermotor::stepMotor(uint8_t stepNumber)
{
    switch (stepNumber)
    {
    case 0:  // 1010
        digitalWrite(pin0, HIGH);
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        digitalWrite(pin3, LOW);
        break;
    case 1:  // 0110
        digitalWrite(pin0, LOW);
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, HIGH);
        digitalWrite(pin3, LOW);
        break;
    case 2:  // 0101
        digitalWrite(pin0, LOW);
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, HIGH);
        break;
    case 3:  // 1001
        digitalWrite(pin0, HIGH);
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, HIGH);
        break;
    }
}