#pragma once


// Forward Decleration
namespace Orchestra
{
    struct Tone;
    class Concertmaster;
}



// Included Files
#include "Instruments.h"
#include <Arduino.h>

#undef min
#undef max

#include <queue>
#include <vector>



// Macros
#define MAX_QUEUED_TONES 8



// Decelration
namespace Orchestra
{
	enum OrchestraState
	{
		Rest,
		Tune,
		Perform,
        Interrupted,
	};



	struct Tone
	{
        public:
            uint8_t note;
            uint8_t instrument;
            uint16_t duration;

            Tone(uint8_t note = 0, uint8_t instrument = 0, uint16_t duration = 0)
            {
                this->note = note;
                this->instrument = instrument;
                this->duration = duration;
            }
	};



	class Concertmaster
	{
	    private:
            Serial_& serial;
            OrchestraState orchestraState;
            std::queue<Tone> tones;

		    uint32_t durationTimeLast = 0;
            bool isToneRequested = false;
		    bool isWaitingDuration = false;

            void sendRequest();
            void sendInstruments();
            void sendState();
            void receive();
            int getNextValue(String& command);

            void mute();
		    void tune(Tone tone);
		    void queueTone(Tone tone);
		    void clearTones();


	    public:
            std::vector<Instruments::Instrument*> instruments;
            
            Concertmaster(Serial_& serial);
            ~Concertmaster();

            OrchestraState getState();
            bool setState(OrchestraState orchestraState);
            
		    void run();    
	};   
}