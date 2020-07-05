#include "src/System.h"
#include "src/Orchestra.h"
#include "src/Instruments.h"
#include "src/Interface.h"
#include "src/Sockets.h"

using namespace Interface;
using namespace Orchestra;
using namespace Instruments;
using namespace Sockets;

ConcertMaster concertMaster = ConcertMaster();
SocketController socketController = SocketController(6);


#pragma region Events

void elementsManipulated()
{
    Serial.println("Interrupted!");
    if (concertMaster.getState() != OrchestraState::Rest)
        concertMaster.setState(OrchestraState::Rest);
}

void elementAdded(Socket *socket, uint8_t index)
{
    Serial.println("Added ID: " + String(socket->getID()) + " to Index: " + String(index));
    if(concertMaster.instruments[index] != nullptr)
        delete concertMaster.instruments[index];

    switch (socket->getID())
    {
        case ElementID::BoardStepper:
            concertMaster.instruments[index] = new Instruments::BoardStepper((BoardSocket*)socket);
            break;
    }
}

void elementRemoved(uint8_t index)
{
    Serial.println("Removed Index: " + String(index));
    delete concertMaster.instruments[index];
    concertMaster.instruments[index] = nullptr;
}

#pragma endregion


void setup()
{
    SerialInterface::setup(250000, &concertMaster);
    concertMaster.instruments[0] = new Instruments::BoardPiezo(11, 12);
    concertMaster.instruments[1] = new Instruments::BoardPiezo(13, 14);
    socketController.sockets[2] = new BoardSocket(7, 4, 8, 3);
    socketController.sockets[3] = new BoardSocket(9, 2, 10, 1);
    socketController.onElementAdded = &elementAdded;
    socketController.onElementRemoved = &elementRemoved;
    socketController.onElementsManipulated = &elementsManipulated;
    socketController.identifyElements();
}

void loop()
{
    SerialInterface::loop();
    System::Timing::loop();
    socketController.loop();
    concertMaster.loop();
}