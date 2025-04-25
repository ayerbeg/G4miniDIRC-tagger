
#include "RunData.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


RunData::RunData() : G4Run() {}

RunData::~RunData() {}

void RunData::AddEventData(G4int eventID, G4int nhit, const std::vector<G4double>& enePho)
{
    fEventData.emplace_back(eventID, nhit, enePho);
}

const std::vector<std::tuple<G4int, G4int, std::vector<G4double>>>& RunData::GetEventData() const
{
    return fEventData;
}

void RunData::Merge(const G4Run* run)
{
    const auto* localRun = dynamic_cast<const RunData*>(run);
    if (localRun) {
        fEventData.insert(fEventData.end(), localRun->fEventData.begin(), localRun->fEventData.end());
    }
}






/*







void RunData::FillPerEvent()
{
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // accumulate statistic
  // in the order of the histograms, ntuple columns declarations
  G4int counter = 0;
  for ( auto edep : fEdep ) {
    analysisManager->FillH1(counter, edep);
    analysisManager->FillNtupleDColumn(counter++, edep);
  }
  for ( auto trackLength : fTrackLength ) {
    analysisManager->FillH1(counter, trackLength);
    analysisManager->FillNtupleDColumn(counter++, trackLength);
  }
  
  analysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunData::Reset()
{
  for ( auto& edep : fEdep ) {
    edep = 0.;
  }
  for ( auto& trackLength : fTrackLength ) {
    trackLength = 0.;
  }
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
