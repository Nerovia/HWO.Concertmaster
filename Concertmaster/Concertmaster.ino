// Included Files
#include "src/System.h"
#include "src/Orchestra.h"
#include "src/Instruments.h"
#include "src/Hotswap.h"



// Using
using namespace Orchestra;
using namespace Orchestra::Instruments;
using namespace Hotswap;



Concertmaster concertmaster = Concertmaster(Serial);
HotswapController hotswapController = HotswapController(6);


void interruptStarted()
{
    concertmaster.setState(OrchestraState::Interrupted);
}


void interruptComplete()
{
    concertmaster.setState(OrchestraState::Rest);
}


void instrumentAdded(Instrument* instrument)
{
    std::vector<Instrument*>& instruments = concertmaster.instruments;

    for (uint8_t n = 0; n < instruments.size(); n++)
    {
        if (instrument->getNumber() < instruments[n]->getNumber())
        {
            instruments.insert(instruments.begin() + n, instrument);
            return;
        }            
    }
    instruments.push_back(instrument);
}


void instrumentRemoved(Instrument* instrument)
{
    std::vector<Instrument*>& instruments = concertmaster.instruments;

    // Find and remove pointer from vector
    for (uint8_t n = 0; n < instruments.size(); n++)
	{
		if (instruments[n] == instrument) 
			instruments.erase(instruments.begin() + n, instruments.begin() + n + 1);
	}
}


void setup()
{
    Serial.begin(115200);

    concertmaster.instruments.push_back(new Instruments::Piezo(11, 12, 0));
    concertmaster.instruments.push_back(new Instruments::Piezo(13, 14, 2));
    hotswapController.registerSlot(new HotswapSlot(7, 4, 8, 3, 1));
    hotswapController.registerSlot(new HotswapSlot(9, 2, 10, 1, 3));
    hotswapController.onInterrupStarted = &interruptStarted;
    hotswapController.onInterrupCompleted = &interruptComplete;
    hotswapController.onInstrumentAdded = &instrumentAdded;
    hotswapController.onInstrumentRemoved = &instrumentRemoved;
    hotswapController.refresh();
}


void loop()
{
    System::Timing::run();
    concertmaster.run();
    hotswapController.run();
}