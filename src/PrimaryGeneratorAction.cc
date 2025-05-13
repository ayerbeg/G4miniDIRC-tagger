#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //
  auto particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(particleDefinition);


  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-0.2,0.,-1.));
 
  fParticleGun->SetParticleEnergy(2*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore
  //
  G4double worldZHalfLength = 0.;
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

  // Check that the world volume has box shape
  G4Box* worldBox = nullptr;
  if (  worldLV ) {
    worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  }

  if ( worldBox ) {
    worldZHalfLength = worldBox->GetZHalfLength();
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
  }

  // Set gun position
//  fParticleGun
//    ->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));
 // Generate random position within a uniform range

 G4double rad = G4UniformRand() * 40* cm; // 
 G4double phi = 2 * CLHEP::pi * G4UniformRand();      // Azimuthal angle [0, 2π]
 

 G4double x = rad*cos(phi); // Range: [-40 cm, 40 cm]
 G4double y = rad*sin(phi); // Range: [-40 cm, 40 cm]
 G4double z = 83.135 *cm ; // Fixed value 2m over the focal point

 // Set the particle gun position
 fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));
// fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -z));

 G4cout << "\nEvent #: "<<anEvent->GetEventID()<<" Gun position: " << fParticleGun->GetParticlePosition()/m << G4endl;

  // Set the particle energy. 4GeV muon energy at sea level, according to the PDG 
  fParticleGun->SetParticleEnergy(4*GeV);

  // Set the particle momentum direction

// Generate random momentum direction
//G4double theta = std::acos(1 - 2 * G4UniformRand()); // Polar angle [0, π]

G4double dist = 200*cm; // Distance from the origin to the cone
G4double angle = atan(rad / dist); // Angle of the cone
//G4cout << "phi: " << phi/deg <<" ANGLE: "<< angle/deg<< G4endl;


G4double px =  -(cos(phi) * sin(angle)) ;
G4double py =  -(sin(phi) * sin(angle)) ;
G4double pz =  -cos(angle) ;

//phi =45*deg; // Fixed value for the angle of the cone

//G4double px = - cos(phi) * sin(45*deg) ;
//G4double py = - sin(phi) * sin(45*deg) ;
//G4double pz =  -cos(45*deg) ;

//fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));

G4cout << "Gun momentum direction: " << fParticleGun->GetParticleMomentumDirection() << G4endl;
G4cout << "Gun energy: "<< fParticleGun->GetParticleEnergy()/GeV << G4endl;
// fParticleGun->SetParticlePosition(G4ThreeVector(-30*cm,0.,0.));  
//  fParticleGun->SetParticlePosition(G4ThreeVector(40*cm,0.,100.*cm));  

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
