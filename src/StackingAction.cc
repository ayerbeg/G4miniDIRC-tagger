#include "StackingAction.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTypes.hh"
#include "G4VProcess.hh"

#include "EventAction.hh"


StackingAction::StackingAction(EventAction* eventAction)
    : G4UserStackingAction(), fEventAction(eventAction), fCerenkovCounter(0)

//StackingAction::StackingAction() : G4UserStackingAction() , fCerenkovCounter(0) 
{
    EneChePho.clear();
}

StackingAction::~StackingAction() {}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack) 
{
    // Example: Kill secondary particles below a certain energy threshold
  

    if (aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) 
    { // particle is optical photon
   //     if (aTrack->GetParentID() == 1) 
        {                // particle is secondary
         if (aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")
           {   
            // the size of the vector is the number of photons created, I could save code with this
            fCerenkovCounter++;
            EneChePho.push_back(aTrack->GetTotalEnergy() / eV);
             // particle is primary
         //   G4cout << "Optical photon created in this event. KineE: " << aTrack->GetKineticEnergy() / eV << " eV "<< "TotalE: " << aTrack->GetTotalEnergy ()/eV << " eV "<< G4endl;
            }
         }
    }

    return fUrgent;
}




void StackingAction::NewStage() {
    // Code to handle the beginning of a new stacking stage


G4cout << "Optical photon created in this event: " << fCerenkovCounter <<" "<< EneChePho.size()<<G4endl;

    // Pass the Cerenkov counter to EventAction
  if (fEventAction)
  {
      fEventAction->SetCerenkovCounter(fCerenkovCounter);
      fEventAction->SetCheEnePho(EneChePho);
  }


}

void StackingAction::PrepareNewEvent() {
    // Code to prepare for a new event
    fCerenkovCounter = 0;
    EneChePho.clear();
    // Reset any other variables or data structures as needed
    // G4cout << "Preparing for a new event" << G4endl;
}