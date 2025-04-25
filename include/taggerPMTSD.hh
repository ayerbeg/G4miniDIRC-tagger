#ifndef taggerPMTSD_h
#define ctaggerPMTSD_h 1

#include "G4VSensitiveDetector.hh"

#include "taggerPMTHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;


/// Calorimeter sensitive detector class
///
/// In Initialize(), it creates one hit for each calorimeter layer and one more
/// hit for accounting the total quantities in all layers.
///
/// The values are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step.


/*
  class taggerPMTSD: public G4VSensitiveDetector
  {
  public:
  taggerPMTSD(const G4String& name,
  const G4String& hitsCollectionName, G4int );
  ~taggerPMTSD() override = default;

  // methods from base class
  void   Initialize(G4HCofThisEvent* hitCollection) override;
  G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
  void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

  private:
  GEMHitsCollection* fHitsCollection = nullptr;
  G4int fNofCells = 0;
  };
*/

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
  //  DriftChamberHitsCollection* fHitsCollection = nullptr;
  G4int fHCID = -1;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

