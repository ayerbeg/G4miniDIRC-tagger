#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "HistoManager.hh"
#include "RunAction.hh"


class RunAction;
class HistoManager;
class DetectorConstruction;


/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(DetectorConstruction*);
    ~ActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;

  private:
    DetectorConstruction* fDetConstruction = nullptr;
};


#endif


