// Included Files
#include "../Orchestra.h"
#include "../Instruments.h"
#include "../System.h"
#include <Arduino.h>

#undef min
#undef max

#include <queue>
#include <vector>



// Using 
using namespace Orchestra;
using namespace Instruments;



Concertmaster::Concertmaster(Serial_& serial) : serial(serial), instruments(), tones()
{
	orchestraState = OrchestraState::Rest;
}


Concertmaster::~Concertmaster()
{
	for (auto& instrument : instruments)
	{
		if (instrument != nullptr)
			delete instrument;
	}
	instruments.clear();
}


void Concertmaster::run()
{
	receive();

	switch (orchestraState)
	{
		case OrchestraState::Rest:
			break;

		case OrchestraState::Perform:

			if (!isToneRequested && tones.size() < MAX_QUEUED_TONES)
			{
				isToneRequested = true;
				sendRequest();
			}

			if (tones.size() > 0)
			{
				auto tone = tones.front();

				if (!isWaitingDuration)
				{
					if (tone.instrument < tones.size())
					{
						auto instrument = instruments.at(tone.instrument);
						if (instrument != nullptr) 
							instrument->setNote(tone.note);
						isWaitingDuration = true;
					}
				}

				if (System::Timing::timeCycleMillis - durationTimeLast >= tone.duration)
				{
					durationTimeLast = System::Timing::timeCycleMillis;
					tones.pop();
					isWaitingDuration = false;
				}
			}
			break;

		case OrchestraState::Tune:
			break;
	}

	for (auto& instrument : instruments)
	{
		instrument->run();
	}
}


bool Concertmaster::setState(OrchestraState newState)
{
	// State didn't changed
	if (orchestraState == newState) 
	{
		sendState();
		return false;
	}

	// Previous state cleanup
	switch (orchestraState)
	{
	case OrchestraState::Interrupted:
		sendInstruments();
		break;
	
	case OrchestraState::Rest:
		break;
		
	case OrchestraState::Tune:
		mute();
		break;

	case OrchestraState::Perform:
		mute();
		isToneRequested = false;
		break;
	}

	// Change state
	orchestraState = newState;

	// New state setup
	switch (orchestraState)
	{
	case OrchestraState::Interrupted:
		mute();
		break;

	case OrchestraState::Rest:
		mute();
		break;

	case OrchestraState::Tune:
		break;

	case OrchestraState::Perform:
		break;
	}

	// Invoke onStateChanged
	sendState();
	return true;
}


OrchestraState Concertmaster::getState()
{
	return orchestraState;
}


void Concertmaster::mute()
{
	for (auto& musician : instruments)
	{
		if (musician != nullptr) musician->setNote(0);
	}
}


void Concertmaster::clearTones()
{
	tones.empty();
}


void Concertmaster::queueTone(Tone tone)
{
	isToneRequested = false;
	tones.push(tone);
}


void Concertmaster::tune(Tone tone)
{
	if (orchestraState != OrchestraState::Tune) 
		return;
	if(tone.instrument < instruments.size())
	{
		auto* insturment = instruments[tone.instrument];
		if (insturment != nullptr) 
			insturment->setNote(tone.note);
	}		
}


void Concertmaster::sendState()
{
	serial.println('M' + String(orchestraState));
}


void Concertmaster::sendRequest()
{
	serial.println('B');
}


void Concertmaster::sendInstruments()
{
	serial.print('I');
	for (auto& instrument : instruments)
	{
		serial.print(String((uint8_t)instrument->getId()) + '$');
	}
	serial.println();
}


void Concertmaster::receive()
{
	if (serial.available())
	{
		String data = Serial.readStringUntil('\n');
		char command = data.charAt(0);
		data.remove(0, 1);

		switch(command)
		{
		case 'M':
			
			switch (getNextValue(data))
			{
				
			case OrchestraState::Rest:
				setState(OrchestraState::Rest);
				break;
			
			case OrchestraState::Tune:
				setState(OrchestraState::Tune);
				break;

			case OrchestraState::Perform:
				setState(OrchestraState::Perform);
					break;

			default:
				serial.println("EM");
				break;
			}
			break;

		case 'T':
			tune(Tone(getNextValue(data), getNextValue(data), getNextValue(data)));
			break;

		case 'B':
			queueTone(Tone(getNextValue(data), getNextValue(data), getNextValue(data)));
			break;
		
		case 'C':
			clearTones();
			break;

		case 'H':
			serial.println("H");
			sendInstruments();
			break;

		default:
			serial.println("E0");
		}
	}
}


int Concertmaster::getNextValue(String& command)
{
	String piece = command;
	int8_t nextSperatorIndex = piece.indexOf('$');
	if(nextSperatorIndex >= 0)
	{
		command.remove(0, nextSperatorIndex + 1);
		piece.remove(nextSperatorIndex, piece.length());
		return(piece.toInt());
	}
	else
	{
		return(piece.toInt());
	}
}