/////////////////////////////////////////////////////////////////////////
//
//
//
// This class collects information at three different levels: 
// 1 - Energy deposited in the sensitive volume (LaBr3 crystal)
// 2 - Number of photons absorbed by the photocathode (or just generated!)
// 3 - Time at which each photon is absorbed
//
// This information is passed to the Event Action Class via 
// the eventAction pointer
//
/////////////////////////////////////////////////////////////////////////

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4SteppingManager.hh"

#include "G4RunManager.hh"

#include "fstream"
#include "iomanip"

using namespace std;	 

SteppingAction::SteppingAction(EventAction* EvAct)
:eventAction(EvAct)
{ }

SteppingAction::~SteppingAction()
{ }

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	const G4String currentPhysicalName 
    = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
	
	const G4String particleName
	= aStep->GetTrack()->GetDefinition()->GetParticleName();

	if (currentPhysicalName == "Target"){
		
		G4double EdepStep = aStep->GetTotalEnergyDeposit();
		
        std::cout << currentPhysicalName << EdepStep << std::endl;
        if (EdepStep > 0.) eventAction->EdepInLHe = eventAction->EdepInLHe + EdepStep;
	}
}

