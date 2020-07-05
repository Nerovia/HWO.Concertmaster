// Included Files
#include "../Hotswap.h"
#include "../Instruments.h"
#include "../System.h"
#include <Arduino.h>



// Using
using namespace Hotswap;
using namespace Orchestra::Instruments;



HotswapController::HotswapController(uint8_t interruptPin)
{
    this->interruptPin = interruptPin;
    this->onInterrupCompleted = nullptr;
    this->onInterrupStarted = nullptr;
    this->onInstrumentAdded = nullptr;
    this->onInstrumentRemoved = nullptr;
    pinMode(this->interruptPin, INPUT_PULLDOWN);
}


HotswapController::~HotswapController()
{
    for (auto& slot : slots)
    {
        if (slot != nullptr)
            delete slot;
    }
    slots.clear();
}


void HotswapController::registerSlot(HotswapSlot* slot)
{
    if (slot == nullptr)
        return;

    slot->controller = this;
    slots.push_back(slot);
}


void HotswapController::refresh()
{
    if (onInterrupStarted != nullptr)
        (*onInterrupStarted)();
    for (auto &slot : slots)
    {
        slot->identifyElements();
    }
    if (onInterrupCompleted != nullptr)
        (*onInterrupCompleted)();
}


void HotswapController::instrumentAdded(Instrument* insturment)
{
    if (onInstrumentAdded != nullptr)
        (*onInstrumentAdded)(insturment);
}


void HotswapController::instrumentRemoved(Instrument* insturment)
{
    if (onInstrumentRemoved != nullptr)
        (*onInstrumentRemoved)(insturment);
}

void HotswapController::run()
{
    bool interruptState = digitalRead(interruptPin);
    if (interruptState != interruptStateLast)
    {
        interruptTriggered = true;
    }


    switch (cooldownState)
    {
    // Wait for interrupt
    case 0:
        if (interruptTriggered)
        {
            if ((*onInterrupStarted) != nullptr) 
                (*onInterrupStarted)();
            cooldownState = 1;
        }
        break;

    // Initialise
    case 1:
        interruptTriggered = false;
        cooldownTimeLast = System::Timing::timeCycleMillis;
        cooldownState = 2;
        break;

    // Wait for Cooldown
    case 2:
        if (interruptTriggered)
            cooldownState = 1;
        if (cooldownTimeLast + cooldownDelay < System::Timing::timeCycleMillis)
        {
            for (auto &slot : slots)
            {
                cooldownState = 0;
                slot->identifyElements();
                if (interruptTriggered)
                {
                    cooldownState = 1;
                    return;
                }
            }
            if ((*onInterrupCompleted) != nullptr) 
                (*onInterrupCompleted)();
        }
        break;
    }
}