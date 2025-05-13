
#include "EventAction.hh"
#include "RunData.hh"
#include "RunAction.hh" 
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include "G4SDManager.hh"
#include "G4Threading.hh"

#include "G4SystemOfUnits.hh"

// Don't ask!! no clue how this piece of code works.
// I would like to modify it to make it more readable
// I understand it is the getter for the GEMs hitcollection
// but not sure

namespace {

  G4VHitsCollection* GetHC(const G4Event* event, G4int collId) {
      auto hce = event->GetHCofThisEvent();
      if (!hce) {
          G4ExceptionDescription msg;
          msg << "No hits collection of this event found." << G4endl;
          G4Exception("EventAction::EndOfEventAction()",
                      "Code001", JustWarning, msg);
          return nullptr;
      }
  
      auto hc = hce->GetHC(collId);
      if (!hc) {
          G4ExceptionDescription msg;
          msg << "Hits collection " << collId << " of this event not found." << G4endl;
          G4Exception("EventAction::EndOfEventAction()",
                      "Code001", JustWarning, msg);
      }
      return hc;
  }
  
  } // End of namespace



EventAction::EventAction(RunAction* RunAction, HistoManager* histo)
  : fRunAct(RunAction)
{
  // I have to pay more attention to the initializers. 
  // Not initializing the HistoManager was giving me problems
  // This is a way to initialize!
  if (histo!=NULL) fHistoManager=histo;

  // This make me think if I need to carry all the machinery of the handlers as I've been doing
  // G4cout<<">>>>>>>>>>>>>LUND generator (EA)<<<<<<<<<<<"<<Variables->GeneratorCase<<G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
}


void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
  


 // tagger Analysis data
 auto sdManager = G4SDManager::GetSDMpointer();

 // I want to modify these lines in order to have explicit names of the variables
 G4String dHCName = { "taggerPMT/taggerPMTHitsCollection" };
 ftaggerHCID[0] = sdManager->GetCollectionID(dHCName);




}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{

  auto eventID = event->GetEventID();
 

// a bit complicated, but I want to keep the same structure
// I did for the GEM analysis
 // for (G4int iDet = 0; iDet < tDim; ++iDet)
  
   
auto hc = GetHC(event, ftaggerHCID[0]);
if ( ! hc ) return;
 
auto nhit = hc->GetSize();
   


for (unsigned long i = 0; i < nhit; ++i)
  {
   taggerPMTHit *hit = static_cast<taggerPMTHit*>(hc->GetHit(i));
    
  EnePho.push_back(hit -> GetEdep());

  if(hit -> GetHitParentID()>1)
    G4cout << "ParentID: " << hit -> GetHitParentID() << G4endl;

    eParentID.push_back(hit -> GetHitParentID()); // this is the parent ID of the photon DETECTED

  }



//G4cout<<"EventAction::EndOfEventAction "<< nhit <<G4endl;
G4cout<<"Event #: "<<eventID<<" fCerenkovCounter "<<  fCerenkovCounter <<G4endl;

fHistoManager -> Filltagger(eventID, nhit, fCerenkovCounter, CheEnePho, EnePho, eParentID);




EnePho.clear();
fCerenkovCounter = 0;
CheEnePho.clear();
eParentID.clear();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

