// Included Files
#include "../Orchestra.h"
#include "../Instruments.h"
#include "../System.h"
#include <Arduino.h>



// Using
using namespace Orchestra;
using namespace Orchestra::Instruments;



Piezo::Piezo(uint8_t pin0, uint8_t pin1, uint8_t number) : Instrument(number)
{
    this->pin0 = pin0;
    this->pin1 = pin1;
    this->number = number;
    pinMode(this->pin0, OUTPUT);
    pinMode(this->pin1, OUTPUT);
}


Piezo::Piezo(Hotswap::HotswapSlot* slot, uint8_t index)
{
    if (index = 0)
    {
        this->pin0 = slot->getPin0();
        this->pin1 = slot->getPin1();  
    }
    else
    {
        this->pin0 = slot->getPin2();
        this->pin1 = slot->getPin3();
    }
    this->number = slot->getNumber();
    pinMode(this->pin0, OUTPUT);
    pinMode(this->pin1, OUTPUT);
}


void Piezo::run()
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

 
void Piezo::setNote(uint8_t note)
{
    if (currentNote == note) return;
    if (note >= PITCHES_COUNT) return;
    currentNote = note;
    state = 0;
}