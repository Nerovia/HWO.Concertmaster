#include "../Sockets.h"
#include <Arduino.h>

using namespace Sockets;

Socket::Socket()
{
}

BoardSocket::BoardSocket(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3)
{
    this->pin0 = pin0;
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->pin3 = pin3;
    pinMode(pin0, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
}

ElementID BoardSocket::identify()
{
    uint8_t id = ElementID::Empty;

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

    this->id = ElementID(id);

    return (this->id);
}