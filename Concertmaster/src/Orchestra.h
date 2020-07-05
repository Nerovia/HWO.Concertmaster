#pragma once
#include "stdint.h"
#include <Arduino.h>

#define MAX_MUSICIANS 4
#define MAX_QUEUED_TONES 8


namespace Orchestra
{
    template <typename T>
    class UnsafeQueue
    {
    private:
        uint8_t pushIndex;
        uint8_t popIndex;
        uint8_t size;
        T* collection;
        
    public:
        uint8_t count;

        UnsafeQueue(uint8_t size)
        {
            this->count = 0;
            this->pushIndex = 0;
            this->popIndex = 0;
            this->size = size;
            this->collection = new T[size];
        }

        ~UnsafeQueue()
        {
            delete[] collection; 
        }

        void clear()
        {
            if (count > 0)
            {
                this->count = 0;
                this->popIndex = pushIndex;
            }
        }


        bool push(T element)
        {
            if (count < size)
            {
                collection[pushIndex] = element;
                count++;
                pushIndex++;
                if (pushIndex >= size) pushIndex = 0;
                return true;
            }
            return false;
        }

        T front()
        {
            return collection[popIndex];
        }

        void pop()
        {
            if (count > 0)
            {
                count--;
                popIndex++;
                if (popIndex >= size) popIndex = 0;
            }
        }
    };

	
	enum OrchestraState
	{
		Rest,
		Tune,
		Perform,
	};


	struct Tone
	{
		Tone(uint8_t note = 0, uint8_t musician = 0, uint16_t duration = 0)
		{
			this->note = note;
			this->musician = musician;
			this->duration = duration;
		}

		uint8_t note;
		uint8_t musician;
		uint16_t duration;
	};


    
	class Instrument
    {
    protected:
        Instrument()
        {
        }

    public:
        virtual void setNote(uint8_t note);
        virtual void loop();
    };



	class ConcertMaster
	{

	private:
		// Buffers the next index to assign a musician.
		OrchestraState orchestraState;
		uint32_t durationTimeLast = 0;


		bool isToneRequested = false;
		bool isWaitingDuration = false;


		
		UnsafeQueue<Tone> tones;

	public:
        Instrument* instruments[MAX_MUSICIANS];


		ConcertMaster();
    

		void loop();

		OrchestraState getState();
		bool setState(OrchestraState orchestraState);



		void mute();
		bool tune(Tone tone);
		bool queueTone(Tone tone);
		void clearTones();



		void (*onRequestTone)();
		void (*onErrorOccurred)();
		void (*onStateChanged)(bool changed, OrchestraState newState);

	private:

		// Returns by value because queue gets overritten


	};



    
}

