#include "taggerPMTSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


taggerPMTSD::taggerPMTSD(G4String name)
: G4VSensitiveDetector(name)
{
  G4cout << "<taggerPMTSD> Info" << G4endl; 
  collectionName.insert("taggerPMTHitsCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void taggerPMTSD::Initialize(G4HCofThisEvent* hce)
{
 // G4cout << "<taggerPMTSD> Initialize" << G4endl;


  fHitsCollection
    = new taggerPMTHitsCollection(SensitiveDetectorName,collectionName[0]);

  if (fHCID<0) {
     fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  hce->AddHitsCollection(fHCID,fHitsCollection);

}




G4bool taggerPMTSD::ProcessHits(G4Step* step,
                                     G4TouchableHistory*)
{
 
 // G4cout << "<taggerPMTSD> ProcessHits" << G4endl;

  // From Example B5
  // This is a driftchamber SD
  auto charge = step->GetTrack()->GetDefinition()->GetPDGCharge();
 // if (charge==0.) return true;

  auto preStepPoint = step->GetPreStepPoint();

  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto motherPhysical = touchable->GetVolume(1); // mother
  auto copyNo = motherPhysical->GetCopyNo();

  
  auto worldPos = preStepPoint->GetPosition();
  auto localPos
    = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);

  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();


  G4int ParticleLevel = step->GetTrack()-> GetTrackID();
  G4int PDGID = step->GetTrack()->GetDefinition()-> GetPDGEncoding();
  
  auto hit = new taggerPMTHit(copyNo);

  G4int iParentID = step->GetTrack()-> GetParentID();  

  // this is similar to the SteppingAction analysis
  // But I don't know how this behave with multiple
  // interactions in a given event
  
  hit->SetWorldPos(worldPos);
  hit->SetLocalPos(localPos);
  hit->SetTime(preStepPoint->GetGlobalTime());

  hit->Add(edep);

  hit->SetPDG(PDGID);
  hit->SetLevel(ParticleLevel);
  hit->SetHitParentID(iParentID);
  fHitsCollection->insert(hit);

  return true;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void taggerPMTSD::EndOfEvent(G4HCofThisEvent*)
{
 // G4cout << "<taggerPMTSD> EndOfEvent" << G4endl; 

  
  //  if ( verboseLevel>1 )
    {
     auto nofHits = fHitsCollection->entries();
     G4cout
       << G4endl
       << "-------->Hits Collection: in this event they are " << nofHits
       << " hits in the PMT. " << G4endl;
     for ( std::size_t i=0; i<nofHits; ++i ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


