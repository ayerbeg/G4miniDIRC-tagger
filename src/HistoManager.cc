//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include "G4SystemOfUnits.hh"

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4Threading.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
{
  G4cout<<"HistoManager::HistoManager() STARTED"<<G4endl;
}


HistoManager::~HistoManager()
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Readme(G4int idx)
{
  G4cout << "HistoManager::Readme() :"<<  EnePho[idx] << " "<< idx<< G4endl;
  
}

void HistoManager::Book()
{
  G4cout << " HistoManager::Book"<< G4endl;
  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.
  //
  if (!G4Threading::IsMasterThread()) {
    G4cout << "HistoManager::Book: Not the master thread, skipping ROOT operations." << G4endl;
    return;
}

 
  rootfile = CreateSaveFile("tagger");


  if (!rootfile || rootfile->IsZombie()) 
    {
      G4cout << " HistoManager::Book :"
	     << " problem creating the ROOT TFile "
	     << G4endl;
      return;
    }
  
   
  
  taggerana = new TTree("taggerana","tagger analysis");
  
  // This method works, do we need structure? 
  // hodotree ->Branch("EventID",event.EventID,"EventID/I");
  // hodotree ->Branch("EDepTot",&event.EDepTot,"EDepTot/D");
  // hodotree ->Branch("paddle",&event.paddle,"paddle/I");




  // GEM 
  //local coordinates
  taggerana -> Branch("nPho", &nPho,"nPho/I"); 
  taggerana -> Branch("EnePho", &EnePho );
  /*
  gemana -> Branch("vZloc", &vZloc );
  //global coordinates
  gemana -> Branch("vXglo", &vXglo );
  gemana -> Branch("vYglo", &vYglo );
  gemana -> Branch("vZglo", &vZglo );
  
  gemana -> Branch("vTchamber", &vTchamber );
  gemana -> Branch("vChamber", &vChamber );
  gemana -> Branch("vgPDG",  &vgPDG );
  gemana -> Branch("vgLevel",  &vgLevel );
*/


  
  
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// This method is called by EndOfRunAction
void HistoManager::Save()
{
  G4cout <<"IN SAVE"<<G4endl;

  if (!G4Threading::IsMasterThread()) {
    G4cout << "HistoManager::Save: Not the master thread, skipping ROOT operations." << G4endl;
    return;
}

  if (!rootfile) {
    G4cout << "HistoManager::Save: ROOT file is null, nothing to save." << G4endl;
  return;
}
    

  
   
    rootfile->Write();        // Writing the histograms to the file
    rootfile->Close();  // and closing the tree (and the file)

 

    G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= kMaxHisto) {
    G4cerr << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
  if  (fHisto[ih] != nullptr) { fHisto[ih]->Fill(xbin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple1(G4double energyAbs, G4double energyGap,
                              G4double trackLAbs , G4double trackLGap )
{
  fEabs = energyAbs;
  fEgap = energyGap;
  fLabs = trackLAbs;
  fLgap = trackLGap;

  G4cout<<"***EVentID: "<<fEabs <<" EDepTot: "<<fEgap <<" paddle: "<< fLabs <<G4endl;
  
  // if (fNtuple1 != nullptr) { fNtuple1->Fill(); }
  // if (fNtuple2 != nullptr) { fNtuple2->Fill(); }
}






void HistoManager::Filltagger(G4int ID, G4int inPho, vector<G4double> vEnePho)
{

  
  //G4cout<<"Filling tagger"<<G4endl;
  //  G4cout<<"EVentID: "<< fEventID<<" EDepTot: "<< fEDepTot <<" paddle: "<< fpaddle<<G4endl;

  fEventID = ID;
  
//  G4cout<<"***EVentID: "<<ID<<G4endl;
 
//  G4cout<<"npho "<< inPho <<G4endl;
//  G4cout<<"Enepho "<< vEnePho.size() <<G4endl;

G4double c = 2.99792458e17; // Speed of light in nm/s
G4double h = 4.135667696e-15; // Planck's constant in eV*s

nPho = inPho;

for(int kk = 0; kk<vEnePho.size(); kk++)
  {
    EnePho.push_back((c*h) / (vEnePho[kk]/eV))  ;
  }
  // The energy of the photon in eV

  // The variables are filled in the corresponding detector classes.
  taggerana ->Fill();
   
  
  EnePho.clear();
 
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
  G4cout << "\n ----> print histograms statistic \n" << G4endl;
  for (auto h1 : fHisto) {
    const G4String name = h1->GetName();

    G4String unitCategory;
    if (name[0] == 'E' ) { unitCategory = "Energy"; }
    if (name[0] == 'L' ) { unitCategory = "Length"; }

    G4cout << name
           << ": mean = " << G4BestUnit(h1->GetMean(), unitCategory)
           << " rms = " << G4BestUnit(h1->GetRMS(), unitCategory )
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




TFile *HistoManager::CreateSaveFile(G4String Name)
{

  G4cout<<"IN createsave"<<G4endl;

  // Perhaps later I implement an enviroment variable for this
  //  G4String pfad(getenv("KAOSDATADIR"));
    
  //    G4String pfad("./rootfiles/");
        
  G4String dataname;

  char buffer[100];
  time_t Now = time(0);	
  strftime((char*)&buffer,100,"%Y-%m-%d-%H-%M-%S",localtime(&Now));
  FileNameSuffix = buffer;

  FileNameSuffix = Name+ "_" + FileNameSuffix+".root";

  TFile *rootoutfile = new TFile(FileNameSuffix, "recreate");


  G4cout << "\n----> Output file is open in " << FileNameSuffix << G4endl;
  
  return rootoutfile;
}
