#include "../Orchestra.h"
#include "../Instruments.h"
#include "../System.h"
#include <Arduino.h>

using namespace Orchestra;
using namespace Instruments;

BoardPiezo::BoardPiezo(uint8_t pin0, uint8_t pin1)
{
    this->pin0 = pin0;
    this->pin1 = pin1;
    pinMode(pin0, OUTPUT);
    pinMode(pin1, OUTPUT);
}

void BoardPiezo::loop()
{
    switch (state)
    {
        case 0: // Initialize
            if (currentNote > 0)
            {
                delayPeriod = 1000L * 1000L / (pitches[currentNote]);
                timeLast = System::Timing::timeCycleMicros;
                state = 1;
            }
            else
            {
                digitalWrite(pin0, LOW);
                digitalWrite(pin1, LOW);
                state = 2;
            }
            break;

        case 1: // Play Note
            if (System::Timing::timeCycleMicros - timeLast >= delayPeriod)
            {
                timeLast = System::Timing::timeCycleMicros;
                if(polarityState)
                {
                    digitalWrite(pin0, HIGH);
                    digitalWrite(pin1, LOW);
                    polarityState = false;
                }
                else
                {
                    digitalWrite(pin0, LOW);
                    digitalWrite(pin1, HIGH);
                    polarityState = true;
                }
            }
            break;

        case 2: // Play Rest or Idle
            break;
    }
}

 
void BoardPiezo::setNote(uint8_t note)
{
    if (currentNote == note) return;
    if (note >= PITCHES_COUNT) return;
    currentNote = note;
    state = 0;
}