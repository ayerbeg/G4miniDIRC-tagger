#ifndef SteppingAction_h
#define teppingAction_h 1

#include "G4UserSteppingAction.hh"

class DetectorConstruction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in Absober and Gap layers and
/// updated in RunData object.

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(const DetectorConstruction* detConstruction);
  ~SteppingAction() override = default;

  void UserSteppingAction(const G4Step* step) override;

private:
  const DetectorConstruction* fDetConstruction = nullptr;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
