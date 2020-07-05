#pragma once

#include "Orchestra.h"
#include <stdint.h>

namespace Interface
{
	// - Every Request must be followed up by a response from either side.
	enum Inst : uint8_t
	{
		FlagCMD = '$',
		FlagESC = '%',
		FlagSEP = '&',

		HumanChangeState = 'S',
		HumanTuneMusician = 'T',
		HumanClearQueue = 'C',
		HumanMusicianInfo = 'M',

		ClearQueue,
		QueueTone,
		TuneMusician,
		ChangeState,
		MusicianInfo,
		Handshake,
	};

	class SerialInterface
	{
	private:

		SerialInterface();

		static Orchestra::ConcertMaster* concertMaster;

		static String getNextParameter(String *command);



		static void sendRequest(Inst instruction, void* parameters, uint8_t parameterSize)
		{
			Serial.write(Inst::FlagCMD);
			Serial.write(instruction);
			sendParameters(parameters, parameterSize);
			Serial.write(Inst::FlagCMD);
		}

		static void sendResponse(Inst instruction, bool outcome, void* parameters, uint8_t parameterSize)
		{
			Serial.write(Inst::FlagCMD);
			Serial.write(instruction);
			if(outcome) Serial.write(255);
			else Serial.write(uint8_t(0));
			sendParameters(parameters, parameterSize);
			Serial.write(Inst::FlagCMD);
		}

		static void sendResponse(Inst instruction, bool outcome)
		{
			Serial.write(Inst::FlagCMD);
			Serial.write(instruction);
			if(outcome) Serial.write(255);
			else Serial.write(uint8_t(0));
			Serial.write(Inst::FlagCMD);
		}

		static void sendParameters(void* parameters = nullptr, uint8_t parameterSize = 0)
		{
			// If there are parameters attached.
			if(parameters != nullptr && parameterSize != 0)
			{
				uint8_t* parameterBytes { static_cast<uint8_t*>(parameters) };
				for(uint8_t n = 0; n < parameterSize; n++)
				{
					uint8_t data = parameterBytes[n];
					if(data >= Inst::FlagSEP && data <= Inst::FlagCMD) Serial.write(Inst::FlagESC);
					Serial.write(data);
				}
			}
		}

		
	public:

		static void setup(uint32_t baudrate, Orchestra::ConcertMaster* concertMaster);

		static void loop();

		static void requestTone();
		static void feedbackOrchestraState(bool changed, Orchestra::OrchestraState newState);
	};
}
