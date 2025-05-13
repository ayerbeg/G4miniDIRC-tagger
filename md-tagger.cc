// mini DIRC tagger simulation based on B4b example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManager.hh" // Include the single-threaded RunManager
#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4TScoreNtupleWriter.hh"
#include "G4UIcommand.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "G4UIterminal.hh"
#include <G4UItcsh.hh>

#include "G4OpticalPhysics.hh"
#include <TApplication.h>

#include "FTFP_BERT.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace
{
  void PrintUsage()
  {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " exampleB4b [-m macro ] [-u UIsession] [-t nThreads] [-vDefault]"
           << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{
  // Evaluate arguments
  //
  if (argc > 7)
  {
    PrintUsage();
    return 1;
  }

  G4String macro;
  G4String session;
  G4bool verboseBestUnits = true;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif
  for (G4int i = 1; i < argc; i = i + 2)
  {
    if (G4String(argv[i]) == "-m")
      macro = argv[i + 1];
    else if (G4String(argv[i]) == "-u")
      session = argv[i + 1];
#ifdef G4MULTITHREADED
    else if (G4String(argv[i]) == "-t")
    {
      nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
    }
#endif
    else if (G4String(argv[i]) == "-vDefault")
    {
      verboseBestUnits = false;
      --i; // this option is not followed with a parameter
    }
    else
    {
      PrintUsage();
      return 1;
    }
  }

  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive *ui = nullptr;
  if (!macro.size())
  {
    ui = new G4UIExecutive(argc, argv, session);
  }

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  // Use G4SteppingVerboseWithUnits
  if (verboseBestUnits)
  {
    G4int precision = 8;
    G4SteppingVerbose::UseBestUnit(precision);
  }

  // Construct the default run manager
  /*
  auto runManager =
      G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
  if (nThreads > 0)
  {
    runManager->SetNumberOfThreads(nThreads);
  }
#endif
*/
auto runManager = new G4RunManager(); // Use single-threaded RunManager


  // Set mandatory initialization classes
  //
  auto detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  auto physicsList = new FTFP_BERT(0);
  runManager->SetUserInitialization(physicsList);
  
 // Add optical physics
 auto opticalPhysics = new G4OpticalPhysics();
 physicsList->RegisterPhysics(opticalPhysics);
  
 
 TApplication app("App", nullptr, nullptr);

  auto actionInitialization = new ActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);

  runManager->Initialize();

  // Initialize visualization
  // auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  auto visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  // CHOOSE BETWEEN BATCH MODE AND UI SESSION
  bool GUI = true; // actually true is terminal mode, false is GUI mode
  //bool GUI = false; // actually true is terminal mode, false is GUI mode
  // Process macro or start UI sessionui
  //
  if (macro.size())
  {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + macro);
  }
  else
  {
    if (GUI)
    {
        G4cout << "main: Terminal" << G4endl;
      // G4UIterminal is a (dumb) terminal.
        G4UIsession *session = 0;
      // I use the ini file to select the terminal or GUI
      // no need of the pre-compiler definitions
      // #ifdef G4UI_USE
        session = new G4UIterminal(new G4UItcsh);
      // #else
      // 	  session = new G4UIterminal();
      // #endif
        session->SessionStart();
        delete session;
    }
    else
    {
      // interactive mode : define UI session
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI())
        {
          UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
