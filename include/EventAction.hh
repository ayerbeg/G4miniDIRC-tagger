#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "HistoManager.hh"
#include "taggerPMTHit.hh"
/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy
/// deposit and track lengths of charged particles in Absober and Gap layers
/// stored in RunData object.

class RunAction;
class HistoManager;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction*, HistoManager*);
    ~EventAction();

    void  BeginOfEventAction(const G4Event* event) override;
    void    EndOfEventAction(const G4Event* event) override;
   
  // Method to set the Cerenkov counter
     void SetCerenkovCounter(G4int counter)
    { 
      fCerenkovCounter = counter;
    }

    void SetCheEnePho(vector<G4double> v){CheEnePho = v;}

  private:
    // methods
   
  RunAction* fRunAct = nullptr;
  HistoManager* fHistoManager  = nullptr;
                            
  G4int ftaggerHCID[1];

  G4int fCerenkovCounter = 0; // Store the Cerenkov counter value

  vector<G4double> EnePho;
  vector<G4double> CheEnePho;
  vector<G4int> eParentID;
  
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


