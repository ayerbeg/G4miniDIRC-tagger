#ifndef RunAction_h
#define RunAction_h 1
#include <mutex>
#include <vector>
#include "G4UserRunAction.hh"
#include "globals.hh"
#include "RunData.hh"
#include "HistoManager.hh"
class G4Run;
class HistoManager;

/// Run action class
///
/// It accumulates statistic and computes dispersion of the energy deposit
/// and track lengths of charged particles with use of analysis tools:
/// H1D histograms are created in BeginOfRunAction() for the following
/// physics quantities:
/// - Edep in absorber
/// - Edep in gap
/// - Track length in absorber
/// - Track length in gap
/// The same values are also saved in the ntuple.
/// The histograms and ntuple are saved in the output file in a format
/// according to a specified file extension.
///
/// In EndOfRunAction(), the accumulated statistic and computed
/// dispersion is printed.
///

class RunAction : public G4UserRunAction
{
  public:
    RunAction(HistoManager* );
    ~RunAction() override = default;

   

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;
    // Thread-safe method to add event data
   

    private: 
  
    G4Run *aRun; // pointer to the current Run
   
   
    HistoManager* fHistoManager = nullptr;


};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

