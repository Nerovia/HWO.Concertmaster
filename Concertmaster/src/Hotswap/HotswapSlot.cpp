// Included Files
#include "../Hotswap.h"
#include "../Instruments.h"



// Using
using namespace Hotswap;
using namespace Orchestra::Instruments;



HotswapSlot::HotswapSlot(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t number)
{
    this->pin0 = pin0;
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->pin3 = pin3;
    this->number = number;

    this->insturmentsCount = 0;
    this->insturments = nullptr;
    this->attachedElementId = ElementId::Empty;
}


HotswapSlot::~HotswapSlot()
{
    deleteInstruments();
}


void HotswapSlot::deleteInstruments()
{
    if (insturments != nullptr)
    {
        for (uint8_t n = 0; n < insturmentsCount; n++)
        {
            if (insturments[n] != nullptr)
            {
                if (controller != nullptr) 
                    controller->instrumentRemoved(insturments[n]);
                delete insturments[n];
            }
        }
        insturmentsCount = 0;
        delete[] insturments;
    }
}


void HotswapSlot::setAttachedElementId(ElementId id)
{
    if (attachedElementId == id)
        return;

    attachedElementId = id;

    // Delete the previous elements and insure elements point to null.
    deleteInstruments();


    switch (attachedElementId)
    {
        default:
        case ElementId::Empty:
            return;
    
        case ElementId::Steppermotor:
            insturmentsCount = 1;
            insturments = new Instrument*[insturmentsCount]
            { 
                new Steppermotor(this),
            };
            break;

        case ElementId::Piezos:
            insturmentsCount = 2;
            insturments = new Instrument*[insturmentsCount]
            { 
                new Piezo(this, 0),
                new Piezo(this, 1),
            };
            break;
    }


    // Notify about new Elements 
    for (uint8_t n = 0; n < insturmentsCount; n++)
    {
        if (controller != nullptr)
            controller->instrumentAdded(insturments[n]);
    }
}


void HotswapSlot::identifyElements()
{
    uint8_t id = (uint8_t)ElementId::Empty;

    pinMode(pin0, INPUT_PULLUP);
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
    pinMode(pin3, INPUT_PULLUP);

    if (!digitalRead(pin0))
        id &= 0b11111110;
    if (!digitalRead(pin1))
        id &= 0b11111101;
    if (!digitalRead(pin2))
        id &= 0b11111011;
    if (!digitalRead(pin3))
        id &= 0b11110111;

    pinMode(pin0, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);

    Serial.println(id);
    setAttachedElementId((ElementId)id); // Argument validation included
}