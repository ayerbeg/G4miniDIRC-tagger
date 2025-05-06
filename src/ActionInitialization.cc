#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "G4MTRunManager.hh"
#include "DetectorConstruction.hh"
#include "StackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction)
 : fDetConstruction(detConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
    // Create a shared HistoManager for the master thread
    HistoManager* histo = new HistoManager();

    // Set the RunAction for the master thread
    SetUserAction(new RunAction(histo));
 // SetUserAction(new RunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  HistoManager *
  histo = new HistoManager();


  SetUserAction(new PrimaryGeneratorAction);
  //SetUserAction(new RunAction);

  RunAction *  runAction = new RunAction(histo);
  SetUserAction(runAction);
  
  EventAction *  eventAction = new EventAction(runAction, histo);
  SetUserAction(eventAction);
//  SetUserAction(new EventAction);
  SetUserAction(new SteppingAction(fDetConstruction));
  SetUserAction(new StackingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

