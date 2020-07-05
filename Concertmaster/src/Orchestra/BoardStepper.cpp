#include "../Orchestra.h"
#include "../Instruments.h"
#include "../Sockets.h"
#include "../System.h"
#include <Arduino.h>

using namespace Orchestra;
using namespace Instruments;
using namespace Sockets;

BoardStepper::BoardStepper(BoardSocket *socket)
{
    this->socket = socket;
}

void BoardStepper::loop()
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
                digitalWrite(socket->pin0, LOW);
                digitalWrite(socket->pin1, LOW);
                digitalWrite(socket->pin2, LOW);
                digitalWrite(socket->pin3, LOW);
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

void BoardStepper::setNote(uint8_t note)
{
    if (currentNote == note) return;
    if (note >= PITCHES_COUNT) return;
    if (note != 0) direction = !direction;
    currentNote = note;
    state = 0;
}

void BoardStepper::stepMotor(uint8_t stepNumber)
{
    switch (stepNumber)
    {
    case 0:  // 1010
        digitalWrite(socket->pin0, HIGH);
        digitalWrite(socket->pin1, LOW);
        digitalWrite(socket->pin2, HIGH);
        digitalWrite(socket->pin3, LOW);
        break;
    case 1:  // 0110
        digitalWrite(socket->pin0, LOW);
        digitalWrite(socket->pin1, HIGH);
        digitalWrite(socket->pin2, HIGH);
        digitalWrite(socket->pin3, LOW);
        break;
    case 2:  // 0101
        digitalWrite(socket->pin0, LOW);
        digitalWrite(socket->pin1, HIGH);
        digitalWrite(socket->pin2, LOW);
        digitalWrite(socket->pin3, HIGH);
        break;
    case 3:  // 1001
        digitalWrite(socket->pin0, HIGH);
        digitalWrite(socket->pin1, LOW);
        digitalWrite(socket->pin2, LOW);
        digitalWrite(socket->pin3, HIGH);
        break;
    }
}