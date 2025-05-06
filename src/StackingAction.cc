#include "StackingAction.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTypes.hh"
#include "G4VProcess.hh"

StackingAction::StackingAction() : G4UserStackingAction() , fCerenkovCounter(0)  {}

StackingAction::~StackingAction() {}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack) 
{
    // Example: Kill secondary particles below a certain energy threshold
  

    if (aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) 
    { // particle is optical photon
        if (aTrack->GetParentID() > 0) 
        {                // particle is secondary
        if (aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov") fCerenkovCounter++;
        }
    }


    return fUrgent;
}




void StackingAction::NewStage() {
    // Code to handle the beginning of a new stacking stage


    G4cout << "Cerenkov photons created in this event: " << fCerenkovCounter << G4endl;
}

void StackingAction::PrepareNewEvent() {
    // Code to prepare for a new event
    fCerenkovCounter = 0;

}