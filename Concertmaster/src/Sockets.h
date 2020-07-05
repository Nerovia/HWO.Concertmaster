#include "stdint.h"

#define MAX_SOCKETS 5
#define TYPE_SOCKET 0
#define TYPE_BOARDSOCKET 1

#pragma once

namespace Sockets
{
    // Defines the identification codes for socket-elements
    enum ElementID : uint8_t
    {
        BoardStepper = 0b11111100,
        BoardPiezo = 0b11110011,
        Empty = 0b11111111,
    };

    // Serves as an abstract base class for a socket of any type, to witch a socket-element can be connected to.
    // A socket must define the identify function to get the ElementID of the connected element.
    class Socket
    {
    protected:
        ElementID id = ElementID::Empty;
        Socket();

    public:
        ElementID getID() { return id; }
        virtual uint8_t getType() { return TYPE_SOCKET; }
        virtual ElementID identify();
    };

    // Defines a socket using digital I/O pins on the board.
    class BoardSocket : public Socket
    {
    public:
        uint8_t pin0, pin1, pin2, pin3;

        BoardSocket(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3);
        uint8_t getType() override { return TYPE_BOARDSOCKET; }
        ElementID identify() override;
    };

    class SocketController
    {
    protected:
        static bool elementsManipulatedTrigger;

        uint8_t cooldownState;
        uint32_t cooldownTimeLast;
        const uint32_t cooldownDelay = 1000;

        uint8_t interruptPin;

        static void elementsManipulated();

    public:
        void identifyElements();

        SocketController(uint8_t interruptPin);
        ~SocketController();

        Socket *sockets[MAX_SOCKETS];

        void loop();

        void (*onElementAdded)(Socket *socket, uint8_t index);
        void (*onElementRemoved)(uint8_t index);
        void (*onElementsManipulated)();
    };

} // namespace Sockets