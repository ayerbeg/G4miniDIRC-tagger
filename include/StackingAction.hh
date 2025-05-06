#ifndef StackingAction_h
#define StackingAction_h

#include <G4UserStackingAction.hh>
#include <G4ClassificationOfNewTrack.hh>

class StackingAction : public G4UserStackingAction {
public:
    StackingAction();
    virtual ~StackingAction();

    // Called for each new track
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;

    // Called at the beginning of each event
    virtual void NewStage() override;

    // Called at the end of each event
    virtual void PrepareNewEvent() override;

    private:

    int fCerenkovCounter;

};

#endif // STACKINGACTION_HH