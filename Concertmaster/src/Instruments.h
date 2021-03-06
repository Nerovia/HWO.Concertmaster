#pragma once



// Forward Declaration
namespace Orchestra
{
    namespace Instruments
    {
        class Instrument;
        class Steppermotor;
        class Piezo;
    }   
}




// Included Files
#include "Hotswap.h"
#include <Arduino.h>



// Macros
#define PITCHES_COUNT 128



// Declaration
namespace Orchestra
{
    namespace Instruments
    {   
        // Defines the id for each instrument.
        enum class InstrumentId : uint8_t
        {
            Unidentified = 0,
            Steppermotor = 1,
            Piezo = 2,
        };


        
        // Serves as a abstract base class for all instruments.
        class Instrument
        {
            protected:
                uint8_t number;

                Instrument(uint8_t number = 0) {}
                

            public:
                virtual ~Instrument() {}

                virtual void setNote(uint8_t note) {}
                virtual void run() {}
                virtual InstrumentId getId() { return InstrumentId::Unidentified; }
                uint8_t getNumber() { return number; }
        };



        class Steppermotor : public Instrument
        {
            private:
                uint8_t pin0, pin1, pin2, pin3;

                uint8_t state = 2;
                uint8_t currentNote = 0;
                uint16_t currentStep = 0;
                uint32_t delayStep = 0;
                uint32_t timeLast = 0;
                bool direction = false;

                void stepMotor(uint8_t stepNumber);


            public:
                Steppermotor(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t number = 0);
                Steppermotor(Hotswap::HotswapSlot* slot);

                void run() override;
                void setNote(uint8_t note) override;
                InstrumentId getId() override { return InstrumentId::Steppermotor; }
        };

        

        class Piezo : public Instrument
        {
            private:
                uint8_t pin0, pin1;

                uint8_t currentNote = 0;
                uint8_t state = 0;
                uint32_t timeLast;
                uint32_t delayPeriod;
                bool polarityState;

            public:
                Piezo(uint8_t pin0, uint8_t pin1, uint8_t number = 0);
                Piezo(Hotswap::HotswapSlot* slot, uint8_t index);

                void run() override;
                void setNote(uint8_t note) override;
                InstrumentId getId() override { return InstrumentId::Piezo; }
        };



        // Defines the freqeuncy for each playable note.
        static float pitches[PITCHES_COUNT] = {

            0.00,  // 000  ->  Null

            8.66,  // 001  ->  C#   -1
            9.18,  // 002  ->  D    -1
            9.72,  // 003  ->  D#   -1
            10.30, // 004  ->  E    -1
            10.91, // 005  ->  F    -1
            11.56, // 006  ->  F#   -1
            12.25, // 007  ->  G    -1
            12.98, // 008  ->  G#   -1
            13.75, // 009  ->  A    -1
            14.57, // 010  ->  A#   -1
            15.43, // 011  ->  B    -1

            16.35, // 012  ->  C     0
            17.32, // 013  ->  C#    0
            18.35, // 014  ->  D     0
            19.45, // 015  ->  D#    0
            20.60, // 016  ->  E     0
            21.83, // 017  ->  F     0
            23.12, // 018  ->  F#    0
            24.50, // 019  ->  G     0
            25.96, // 020  ->  G#    0
            27.50, // 021  ->  A     0
            29.14, // 022  ->  A#    0
            30.87, // 023  ->  B     0

            32.70, // 024  ->  C     1
            34.65, // 025  ->  C#    1
            36.71, // 026  ->  D     1
            38.89, // 027  ->  D#    1
            41.20, // 028  ->  E     1
            43.65, // 029  ->  F     1
            46.25, // 030  ->  F#    1
            49.00, // 031  ->  G     1
            51.91, // 032  ->  G#    1
            55.00, // 033  ->  A     1
            58.27, // 034  ->  A#    1
            61.74, // 035  ->  B     1

            65.41,  // 036  ->  C     2
            69.30,  // 037  ->  C#    2
            73.42,  // 038  ->  D     2
            77.78,  // 039  ->  D#    2
            82.41,  // 040  ->  E     2
            87.31,  // 041  ->  F     2
            92.50,  // 042  ->  F#    2
            98.00,  // 043  ->  G     2
            103.83, // 044  ->  G#    2
            110.00, // 045  ->  A     2
            116.54, // 046  ->  A#    2
            123.47, // 047  ->  B     2

            130.81, // 048  ->  C     3
            138.59, // 049  ->  C#    3
            146.83, // 050  ->  D     3
            155.56, // 051  ->  D#    3
            164.81, // 052  ->  E     3
            174.61, // 053  ->  F     3
            185.00, // 054  ->  F#    3
            196.00, // 055  ->  G     3
            207.65, // 056  ->  G#    3
            220.00, // 057  ->  A     3
            233.08, // 058  ->  A#    3
            246.94, // 059  ->  B     3

            261.63, // 060  ->  C     4
            277.18, // 061  ->  C#    4
            293.66, // 062  ->  D     4
            311.13, // 063  ->  D#    4
            329.63, // 064  ->  E     4
            349.23, // 065  ->  F     4
            369.99, // 066  ->  F#    4
            392.00, // 067  ->  G     4
            415.30, // 068  ->  G#    4
            440.00, // 069  ->  A     4
            466.16, // 070  ->  A#    4
            493.88, // 071  ->  B     4

            523.25, // 072  ->  C     5
            554.37, // 073  ->  C#    5
            587.33, // 074  ->  D     5
            622.25, // 075  ->  D#    5
            659.26, // 076  ->  E     5
            698.46, // 077  ->  F     5
            739.99, // 078  ->  F#    5
            783.99, // 079  ->  G     5
            830.61, // 080  ->  G#    5
            880.00, // 081  ->  A     5
            932.33, // 082  ->  A#    5
            987.77, // 083  ->  B     5

            1046.50, // 084  ->  C     6
            1108.73, // 085  ->  C#    6
            1174.66, // 086  ->  D     6
            1244.51, // 087  ->  D#    6
            1318.51, // 088  ->  E     6
            1396.91, // 089  ->  F     6
            1479.98, // 090  ->  F#    6
            1567.98, // 091  ->  G     6
            1661.22, // 092  ->  G#    6
            1760.00, // 093  ->  A     6
            1864.66, // 094  ->  A#    6
            1975.53, // 095  ->  B     6

            2093.00, // 096  ->  C     7
            2217.46, // 097  ->  C#    7
            2349.32, // 098  ->  D     7
            2489.02, // 099  ->  D#    7
            2637.02, // 100  ->  E     7
            2793.83, // 101  ->  F     7
            2959.96, // 102  ->  F#    7
            3135.96, // 103  ->  G     7
            3322.44, // 104  ->  G#    7
            3520.00, // 105  ->  A     7
            3729.31, // 106  ->  A#    7
            3951.07, // 107  ->  B     7

            4186.01, // 108  ->  C     8
            4434.92, // 109  ->  C#    8
            4698.64, // 110  ->  D     8
            4978.03, // 111  ->  D#    8
            5274.04, // 112  ->  E     8
            5587.65, // 113  ->  F     8
            5919.91, // 114  ->  F#    8
            6271.93, // 115  ->  G     8
            6644.88, // 116  ->  G#    8
            7040.00, // 117  ->  A     8
            7458.62, // 118  ->  A#    8
            7902.13, // 119  ->  B     8

            8372.02,  // 120  ->  C     9
            8869.84,  // 121  ->  C#    9
            9397.27,  // 122  ->  D     9
            9956.06,  // 123  ->  D#    9
            10548.08, // 124  ->  E     9
            11175.30, // 125  ->  F     9
            11839.82, // 126  ->  F#    9
            12543.85, // 127  ->  G     9
        };
    }
}