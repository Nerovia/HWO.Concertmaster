#include "../Interface.h"
#include "../Orchestra.h"
#include <stdint.h>

using namespace Orchestra;
using namespace Interface;

ConcertMaster* SerialInterface::concertMaster = nullptr;

SerialInterface::SerialInterface()
{

}

void SerialInterface::setup(uint32_t baudrate, ConcertMaster* concertMaster)
{
	Serial.begin(baudrate);
	SerialInterface::concertMaster = concertMaster;
	SerialInterface::concertMaster->onRequestTone = &requestTone;
	SerialInterface::concertMaster->onStateChanged = &feedbackOrchestraState;
}

void SerialInterface::loop()
{
	if (Serial.available())
	{
		bool result = false;
		String data = Serial.readStringUntil('\n');
		char command = data.charAt(0);
		data.remove(0, 1);

		switch(command)
		{
		case 'M':
			
			switch (getNextParameter(&data).toInt())
			{
				
			case 0:
				result = concertMaster->setState(OrchestraState::Rest);
				break;
			
			case 1:
				result = concertMaster->setState(OrchestraState::Tune);
				break;

			case 2:
				result = concertMaster->setState(OrchestraState::Perform);
					break;

			default:
				Serial.println("EM");
				break;
			}
			break;

		case 'T':
			result = concertMaster->tune(Tone(getNextParameter(&data).toInt(), getNextParameter(&data).toInt(), getNextParameter(&data).toInt()));
			Serial.println("T" + String(result));
			break;

		case 'B':
			result = concertMaster->queueTone(Tone(getNextParameter(&data).toInt(), getNextParameter(&data).toInt(), getNextParameter(&data).toInt()));
			Serial.println("B" + String(result));
			break;
		
		case 'C':
			concertMaster->clearTones();
			Serial.println("C1");
			break;

		case 'H':
			Serial.println("H");
			break;

		default:
			Serial.println("E0");
		}
	}
}

void SerialInterface::requestTone()
{
	Serial.println('b');
}

void SerialInterface::feedbackOrchestraState(bool changed, Orchestra::OrchestraState newState)
{
	Serial.println('M' + String(changed) + '$' + String(newState));
}


String SerialInterface::getNextParameter(String *command)
{
	String piece = *command;
	int8_t nextSperatorIndex = piece.indexOf('$');
	if(nextSperatorIndex >= 0)
	{
		command->remove(0, nextSperatorIndex + 1);
		piece.remove(nextSperatorIndex, piece.length());
		return(piece);
		
	}
	else
	{
		return(piece);
	}
}