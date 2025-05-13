#ifndef taggerPMTSD_h
#define ctaggerPMTSD_h 1

#include "G4VSensitiveDetector.hh"

#include "taggerPMTHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;


class taggerPMTSD : public G4VSensitiveDetector
{
public:
  taggerPMTSD(G4String name);
  ~taggerPMTSD() override = default;
  
  void Initialize(G4HCofThisEvent*HCE) override;
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;
  void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

private:
  taggerPMTHitsCollection* fHitsCollection = nullptr;
  G4int fHCID = -1;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

