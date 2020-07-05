#pragma once



// Forward declaration
namespace Hotswap
{
    class HotswapSlot;
    class HotswapController;
}



// Included Headers
#include "Instruments.h"
#include <Arduino.h>

#undef min
#undef max

#include <vector>



// Declaration
namespace Hotswap
{
    enum class ElementId : uint8_t
    {
        Empty = 0b11111111,
        Steppermotor = 0b11111100,
        Piezos = 0b11110011,
    };



    // Defines an abstract base-class for hotswap-slots that can be controlled by an HotswapController.
    class HotswapSlot
    {
        friend class HotswapController;

        
        protected:
            uint8_t pin0, pin1, pin2, pin3;
            uint8_t number;
            HotswapController* controller;

            Orchestra::Instruments::Instrument** insturments;
            uint8_t insturmentsCount; 
            ElementId attachedElementId;
            
            void setAttachedElementId(ElementId id);
            void deleteInstruments();


        public:
            HotswapSlot(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t number = 0);
            ~HotswapSlot();

            void identifyElements();

            uint8_t getPin0() { return pin0; }
            uint8_t getPin1() { return pin1; }
            uint8_t getPin2() { return pin2; }
            uint8_t getPin3() { return pin3; }
            uint8_t getNumber() { return number; }
    };


    // Controlls an array of hotswap sockets and notfifies on interruption and element manipulation.
    class HotswapController
    {
        friend class HotswapSlot;

        private:
            bool interruptStateLast;
            bool interruptTriggered;
            uint8_t interruptPin;

            uint8_t cooldownState;
            uint32_t cooldownTimeLast;
            const uint32_t cooldownDelay = 1000;

            std::vector<HotswapSlot*> slots;
            
            void instrumentAdded(Orchestra::Instruments::Instrument* insturment);
            void instrumentRemoved(Orchestra::Instruments::Instrument* insturment);
            

        public:
            HotswapController(uint8_t interruptPin);
            ~HotswapController();
            void refresh();
            void registerSlot(HotswapSlot* slot);
            void run();

            void (*onInstrumentAdded)(Orchestra::Instruments::Instrument* insturment);
            void (*onInstrumentRemoved)(Orchestra::Instruments::Instrument* insturment);
            void (*onInterrupStarted)();
            void (*onInterrupCompleted)();
    };
}