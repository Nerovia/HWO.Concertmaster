#include "../Sockets.h"
#include "../System.h"

using namespace Sockets;

SocketController::SocketController(uint8_t interruptPin)
{
    for (uint8_t n = 0; n < MAX_SOCKETS; n++)
    {
        sockets[n] = nullptr;
    }

    this->interruptPin = interruptPin;
    pinMode(interruptPin, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(this->interruptPin), elementsManipulated, CHANGE);
}

SocketController::~SocketController()
{
    detachInterrupt(interruptPin);
}

bool SocketController::elementsManipulatedTrigger = false;

void SocketController::identifyElements()
{
    for (uint8_t n = 0; n < MAX_SOCKETS; n++)
    {
        if (sockets[n] == nullptr) continue;
        if (sockets[n]->getID() != sockets[n]->identify())
        {
            // After the call of identify the field id has updated.
            if (sockets[n]->getID() == ElementID::Empty)
            {
                if (onElementRemoved != nullptr) (*onElementRemoved)(n); 
            }
                
            else
            {
                if (onElementAdded != nullptr) (*onElementAdded)(sockets[n], n);
            }
        }
    }
}

void SocketController::loop()
{
    switch (cooldownState)
    {
    // Wait for interrupt
    case 0:
        if (elementsManipulatedTrigger)
        {
            if (onElementsManipulated != nullptr) (*onElementsManipulated)();
            cooldownState = 1;
        }
        break;

    // Initialise
    case 1:
        elementsManipulatedTrigger = false;
        cooldownTimeLast = System::Timing::timeCycleMillis;
        cooldownState = 2;
        break;

    // Wait for Cooldown
    case 2:
        if (elementsManipulatedTrigger)
            cooldownState = 1;
        if (cooldownTimeLast + cooldownDelay < System::Timing::timeCycleMillis)
        {
            identifyElements();
            cooldownState = 0;
        }
        break;
    }
}

void SocketController::elementsManipulated()
{
    elementsManipulatedTrigger = true;
}