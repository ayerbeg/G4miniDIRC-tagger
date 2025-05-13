#ifndef StackingAction_h
#define StackingAction_h

#include <G4UserStackingAction.hh>
#include <G4ClassificationOfNewTrack.hh>
#include <vector>
#include "globals.hh"

using namespace std;

class EventAction;

class StackingAction : public G4UserStackingAction {
public:
  //  StackingAction();
    StackingAction(EventAction* eventAction);
    virtual ~StackingAction();

    // Called for each new track
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;

    // Called at the beginning of each event
    virtual void NewStage() override;

    // Called at the end of each event
    virtual void PrepareNewEvent() override;

    private:
    EventAction* fEventAction = nullptr; // Pointer to the EventAction
    int fCerenkovCounter;

    vector<G4double> EneChePho;

};

#endif // STACKINGACTION_HH