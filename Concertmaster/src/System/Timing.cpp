#include "../System.h"
#include <Arduino.h>

using namespace System;

// Timespans
uint32_t Timing::timespanCycleMillis = 0;
uint32_t Timing::timespanCycleMicros = 0;

// Times
uint32_t Timing::timeCycleMillis = 0;
uint32_t Timing::timeCycleMicros = 0;
uint32_t Timing::timeCyclePreviousMillis = 0;
uint32_t Timing::timeCyclePreviousMicros = 0;

// Methodes
void Timing::loop()
{
	// Set previous cycle time
	timeCyclePreviousMillis = timeCycleMillis;
	timeCyclePreviousMicros = timeCycleMicros;

	// Set current cycle time
	timeCycleMillis = millis();
	timeCycleMicros = micros();

	// Set cycle timespan
	timespanCycleMillis = timeCycleMillis - timeCyclePreviousMillis;
	timespanCycleMicros = timeCycleMicros - timeCyclePreviousMicros;
}