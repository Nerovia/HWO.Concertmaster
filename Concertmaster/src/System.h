#pragma once
#include <Arduino.h>

namespace System
{
	namespace Timing
	{
		// Timestamps
		extern uint32_t timespanCycleMillis;
		extern uint32_t timespanCycleMicros;

		// Times
		extern uint32_t timeCycleMillis;
		extern uint32_t timeCycleMicros;
		extern uint32_t timeCyclePreviousMillis;
		extern uint32_t timeCyclePreviousMicros;

		// Methodes
		extern void run();
	}
}