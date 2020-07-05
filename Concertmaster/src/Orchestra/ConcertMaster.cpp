#include "../Orchestra.h"
#include "../System.h"

using namespace Orchestra;

ConcertMaster::ConcertMaster() :
	onRequestTone(nullptr),
	onErrorOccurred(nullptr),
	onStateChanged(nullptr),
	tones(MAX_QUEUED_TONES)
{
}

void ConcertMaster::loop()
{
	switch (orchestraState)
	{
	case OrchestraState::Rest:
		break;

	case OrchestraState::Perform:

		if (!isToneRequested && tones.count < MAX_QUEUED_TONES)
		{
			if (onRequestTone != nullptr)
			{
				isToneRequested = true;
				(*onRequestTone)();
			}
		}

		if (tones.count > 0)
		{
			auto tone = tones.front();

			if (!isWaitingDuration)
			{
				auto* musician = instruments[tone.musician];
				if (musician != nullptr) musician->setNote(tone.note);
				isWaitingDuration = true;
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

	for (auto& musician : instruments)
	{
		if (musician != nullptr) musician->loop();
	}
}

bool ConcertMaster::setState(OrchestraState newState)
{
	// State didn't changed
	if (orchestraState == newState) 
	{
		if (onStateChanged != nullptr) (*onStateChanged)(false, orchestraState);
		return false;
	}

	// Previous state cleanup
	switch (orchestraState)
	{
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
	case OrchestraState::Rest:
		break;

	case OrchestraState::Tune:
		break;

	case OrchestraState::Perform:
		break;
	}

	// Invoke onStateChanged
	if (onStateChanged != nullptr) (*onStateChanged)(true, orchestraState);
	return true;
}

OrchestraState ConcertMaster::getState()
{
	return orchestraState;
}

void ConcertMaster::mute()
{
	for (auto& musician : instruments)
	{
		if (musician != nullptr) musician->setNote(0);
	}
}

void ConcertMaster::clearTones()
{
	tones.clear();
}

bool ConcertMaster::queueTone(Tone tone)
{
	isToneRequested = false;
	return tones.push(tone);
}

bool ConcertMaster::tune(Tone tone)
{
	if (orchestraState != OrchestraState::Tune) return false;
	auto* musician = instruments[tone.musician];
	if (musician == nullptr) return false;
	musician->setNote(tone.note);
	return true;
}
