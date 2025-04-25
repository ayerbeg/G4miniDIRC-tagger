#ifndef RunData_h
#define RunData_h 1

#include "G4Run.hh"
#include "globals.hh"
#include <vector>
#include <tuple>
#include <array>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4int kAbs = 0;
const G4int kGap = 1;
const G4int kDim = 2;

///  Run data class
///
/// It defines data members to hold the energy deposit and track lengths
/// of charged particles in Absober and Gap layers.
///
/// In order to reduce the number of data members a 2-dimensions array
/// is introduced for each quantity:
/// - fEdep[], fTrackLength[].
///
/// The data are collected step by step in SteppingAction, and
/// the accumulated values are filled in histograms and a Ntuple
/// event by event in EventAction.

class RunData : public G4Run
{
public:
  RunData();
  ~RunData() override;


    // Add event data
    void AddEventData(G4int eventID, G4int nhit, const std::vector<G4double>& enePho);

    // Access collected data
    const std::vector<std::tuple<G4int, G4int, std::vector<G4double>>>& GetEventData() const;

    // Merge data from worker threads
    void Merge(const G4Run* run) override;

  private:
    std::vector<std::tuple<G4int, G4int, std::vector<G4double>>> fEventData;

};
/*
  void Add(G4int id, G4double de, G4double dl);
  void FillPerEvent();

  void Reset();

  // Get methods
  G4String  GetVolumeName(G4int id) const;
  G4double  GetEdep(G4int id) const;
  G4double  GetTrackLength(G4int id) const;

private:
  std::array<G4String, kDim>  fVolumeNames = { "Absorber", "Gap" };
  std::array<G4double, kDim>  fEdep = { 0., 0. };
  std::array<G4double, kDim>  fTrackLength = { 0., 0. };
};

// inline functions

inline void RunData::Add(G4int id, G4double de, G4double dl) {
  fEdep[id] += de;
  fTrackLength[id] += dl;
}

inline G4String  RunData::GetVolumeName(G4int id) const {
  return fVolumeNames[id];
}

inline G4double  RunData::GetEdep(G4int id) const {
  return fEdep[id];
}

inline G4double  RunData::GetTrackLength(G4int id) const {
  return fTrackLength[id];
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

