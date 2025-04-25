#include "TaggerDetectorConstruction.hh"
#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "taggerPMTSD.hh"


TaggerDetectorConstruction::TaggerDetectorConstruction()
{
  G4cout << "TaggerDetectorConstruction" << G4endl; 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



TaggerDetectorConstruction::~TaggerDetectorConstruction()
{
  G4cout << "TaggerDetectorConstruction done" << G4endl; 
}

void TaggerDetectorConstruction::DefineMaterials()
{
  // Materials for the tagger
C  = new G4Element("Carbon"  ,"C" , 6., 12.01*g/mole);//name, symbol, Z, A
O  = new G4Element("Oxygen"  ,"O" , 8., 16.00*g/mole);//name, symbol, Z, A

CO2 =
new G4Material("CO2", 
                1.98 *kg/m3,//density 
                2, //number of components
                kStateGas, 
                300.*kelvin,//temperature 
                1.*atmosphere);//pressure
CO2->AddElement(C, 1);//element, number of atoms
CO2->AddElement(O, 2);//element, number of atoms


// This is the energy range of the visible photons
// This is common for all the optical properties
// unless is discretized, as will be in with the PMT
G4double photonEnergy[] = {2.0*eV, 3.5*eV}; // Define the energy range 
const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);



//***********From Copilot****************************** */
// Add optical properties to the CO2

G4double refractiveIndex[] = {1.00045, 1.00045}; // Refractive index for CO2
assert(sizeof(refractiveIndex) == sizeof(photonEnergy));

// I believe the absorption length is this large because the C02 is quite transparent to visible light
G4double absorption[] = {10*m, 10*m}; // Absorption length for CO2
assert(sizeof(absorption) == sizeof(photonEnergy));


G4MaterialPropertiesTable* CO2MPT = new G4MaterialPropertiesTable();
CO2MPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
CO2MPT->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries);

CO2->SetMaterialPropertiesTable(CO2MPT);
//***********From Copilot****************************** */

Al = new G4Element("Aluminum", "Al", 13., 26.98*g/mole);
Aluminum = new G4Material("Aluminum", 2.70*g/cm3, 1);
Aluminum->AddElement(Al, 1); // Add Aluminum element with 100% composition


// the Winston cone material
PLA = new G4Material("PLA", 1.25*g/cm3, 3);
PLA->AddElement(C, 3);
PLA->AddElement(G4Element::GetElement("H"), 4);
PLA->AddElement(O, 2);


// Define Mylar (C10H8O4)
G4Element* H = new G4Element("Hydrogen", "H", 1., 1.01*g/mole);

G4Material* Mylar = new G4Material("Mylar", 1.39*g/cm3, 3);
Mylar->AddElement(C, 10);
Mylar->AddElement(H, 8);
Mylar->AddElement(O, 4);

// Define Aluminized Mylar (assuming a thin layer of aluminum on Mylar)
G4double density = 1.39*g/cm3; // Approximate density
AluminizedMylar = new G4Material("AluminizedMylar", density, 2);
AluminizedMylar->AddMaterial(Mylar, 0.98); // 98% Mylar
AluminizedMylar->AddElement(Al, 0.02); // 2% Aluminum


// Add optical properties to Aluminized Mylar
G4double reflectivityAlMylar[] = {0.9, 0.9}; // Reflectivity for Aluminized Mylar
assert(sizeof(reflectivityAlMylar) == sizeof(photonEnergy));

MPTAluminizedMylar = new G4MaterialPropertiesTable();
MPTAluminizedMylar->AddProperty("REFLECTIVITY", photonEnergy, reflectivityAlMylar, nEntries);



 // PMMA C5H8O2 ( Acrylic )
 // -------------

 Acrylic = new G4Material("Acrylic", 1.19*g/cm3, 3);
 Acrylic->AddElement(C, 5);
 Acrylic->AddElement(H, 8);
 Acrylic->AddElement(O, 2);

//////////////////////////////////////////////////////////////////
//               ACRYLIC Optical properties
// from copilot. I found a more complex parameterization here:
// https://apc.u-paris.fr/~franco/g4doxy4.10/html/_ultra_detector_construction_8cc_source.html
// but there is a problem with the order of the elements
// in the array. The enerygy is not in increasing order.
//////////////////////////////////////////////////////////////////
 
const G4int nAcryEntries = 10;
G4double AcryphotonEnergy[nAcryEntries] = {
    1.77*eV, 2.07*eV, 2.48*eV, 2.76*eV, 3.10*eV,
    3.54*eV, 3.88*eV, 4.13*eV, 4.43*eV, 4.96*eV
};

// Refractive index for acrylic
G4double AcryrefractiveIndex[nAcryEntries] = {
    1.489, 1.490, 1.491, 1.492, 1.493,
    1.494, 1.495, 1.496, 1.497, 1.498
};

// Absorption length for acrylic (in meters) //it's that true?
G4double AcryabsorptionLength[nAcryEntries] = {
    10*m, 10*m, 10*m, 10*m, 10*m,
    10*m, 10*m, 10*m, 10*m, 10*m
};

// Create material properties table for acrylic
G4MaterialPropertiesTable* MPT_Acrylic = new G4MaterialPropertiesTable();
MPT_Acrylic->AddProperty("RINDEX", AcryphotonEnergy, AcryrefractiveIndex, nEntries);
MPT_Acrylic->AddProperty("ABSLENGTH", AcryphotonEnergy, AcryabsorptionLength, nEntries);

 
Acrylic->SetMaterialPropertiesTable(MPT_Acrylic);

//////////////////////////////////////////////////////////////////



// Silver
Ag = new G4Element("Silver", "Ag", 47., 107.87*g/mole);

Silver = new G4Material("Silver", 10.49*g/cm3, 1);
Silver->AddElement(Ag, 1);

//*******************From CoPilot****************** */
// Define optical properties for Silver
 
G4double reflectivity[] = {0.95, 0.95}; // Reflectivity for Silver
assert(sizeof(reflectivity) == sizeof(photonEnergy));

MPTSilver = new G4MaterialPropertiesTable();
MPTSilver->AddProperty("REFLECTIVITY", photonEnergy, reflectivity, nEntries);
//*******************From CoPilot****************** */

// Define the material for the PMT (e.g., Aluminum)
G4Material* diskMaterial = G4Material::GetMaterial("Aluminum");



//*******************From CoPilot****************** */
// This looks quite similar to the borosilicate glass 
// defintion in the wikipedia page
// https://en.wikipedia.org/wiki/Borosilicate_glass


  // Define elements
  Si = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
  B = new G4Element("Boron", "B", 5., 10.81*g/mole);
  Na = new G4Element("Sodium", "Na", 11., 22.99*g/mole);
  //Al = new G4Element("Aluminum", "Al", 13., 26.98*g/mole);
  //O = new G4Element("Oxygen", "O", 8., 16.00*g/mole);

  // Define borosilicate glass material
  BorosilicateGlass = new G4Material("BorosilicateGlass", 2.23*g/cm3, 5);
  BorosilicateGlass->AddElement(Si, 80.6*perCent);  // Silicon dioxide (SiO₂)
  BorosilicateGlass->AddElement(B, 13.0*perCent);   // Boron trioxide (B₂O₃)
  BorosilicateGlass->AddElement(Na, 4.0*perCent);   // Sodium oxide (Na₂O)
  BorosilicateGlass->AddElement(Al, 2.0*perCent);   // Aluminum oxide (Al₂O₃)
  BorosilicateGlass->AddElement(O, 0.4*perCent);    // Remaining oxygen

  // Define optical properties for borosilicate glass
  //G4double refractiveIndex[] = {1.47, 1.47}; // Refractive index for borosilicate glass
  //assert(sizeof(refractiveIndex) == sizeof(photonEnergy));


  // THIS IS AN ARTIFICIAL VALUE TO KILL THE PHOTON IN THE MATERIAL
  G4double absorptionLength[] = {10*um, 10*um}; // Absorption length for borosilicate glass
  assert(sizeof(absorptionLength) == sizeof(photonEnergy));

  G4MaterialPropertiesTable* MPTBorosilicateGlass = new G4MaterialPropertiesTable();
  MPTBorosilicateGlass->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
  MPTBorosilicateGlass->AddProperty("ABSLENGTH", photonEnergy, absorptionLength, nEntries);

 



  // These numbers are taken from a plot showing a characteristic curve for bialkaly photocathodes
  // I didn't find the original for the ET 4312KB. 40 points

  // For the moment I add the QE propeties of the photocathode to the borosilicate glass
  // From Roman code, MAYBE reflectivity should be added to the MPTBorosilicateGlass 

  const int num = 40;

  G4double en_photonCathode[num]={ 
  1.78*eV, 1.8*eV,  1.83*eV, 1.86*eV, 1.89*eV, 1.91*eV, 1.95*eV, 1.97*eV, 2.01*eV, 2.04*eV, 2.07*eV, 2.11*eV, 2.14*eV, 2.18*eV,
  2.22*eV, 2.26*eV, 2.3*eV,  2.34*eV, 2.39*eV, 2.44*eV, 2.49*eV, 2.54*eV, 2.59*eV, 2.65*eV, 2.7*eV,  2.76*eV, 2.83*eV, 2.89*eV,
  2.96*eV, 3.03*eV, 3.1*eV,  3.18*eV, 3.27*eV, 3.35*eV, 3.44*eV, 3.53*eV, 3.64*eV, 3.76*eV, 3.87*eV, 3.99*eV};
  

  G4double qe_photonCathode[num]={
   0.632,   0.402,   0.46,    0.46,    0.402,   0.402,   0.402,   0.632,   0.805,   1.207,   1.667,   2.299,  2.931,  3.621,  4.483,
   5.345,   6.379,   8.046,  10.747,  13.103,  14.368,  15.057,  16.207,  17.069,  18.563,  19.885,  21.149,  21.954,
  22.701,  23.391,  23.908,  24.023,  23.448,  23.103,  21.954,  18.563,  12.471,   3.563,   0.92,    0.517};
  





  
  MPTBorosilicateGlass->AddProperty("EFFICIENCY", en_photonCathode, qe_photonCathode, num);


  BorosilicateGlass->SetMaterialPropertiesTable(MPTBorosilicateGlass);
  //*******************From CoPilot****************** */





  // Print materials
//  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TaggerDetectorConstruction::BuildTagger(G4LogicalVolume *worldLV)
{
  DefineMaterials();

  // Geometry parameters

  //
  //  cosmic          z |  /y
  //    |      --->     | /
  //    |     MIRROR    |/_____x   PMT
  //    V              
  //                 
  //

G4double x_box = 901.1 *mm; 
G4double y_box = 440.0 *mm;
G4double z_box = 1612.1 *mm;

// We make a box with the above dimensions and place it at the origin
// We will then place the tagger components inside this box

MainBox = new G4Box("Main_box", // its name
			       x_box/2, 
             y_box/2,  
             z_box/2); // its size


G4double x_extbox = 703. *mm;
G4double y_extbox = 433. *mm; 
G4double z_extbox = 451.3 *mm;

ExtBox = new G4Box("Ext_box", // its name
			       x_extbox/2, 
             y_extbox/2,  
             z_extbox/2); // its size
      


 
Tagger = new G4UnionSolid("Main_box+Ext_box", MainBox, ExtBox,0, 
                        G4ThreeVector (x_box/2+x_extbox/2,
                                      0,
                                      -z_box/2+z_extbox/2));//, nullptr, G4ThreeVector(0., 0., 0.));

TaggerLV = new G4LogicalVolume(Tagger,       // its solid
                              Aluminum,  // its material
					                    "TaggerLV");         // its name
	      
new G4PVPlacement(nullptr,                                                    // no rotation
				G4ThreeVector (0,0, 0),   // its position
				TaggerLV,                                                      // its logical volume
				"Tagger_phy",                                                  // its name
				worldLV,                                           // its mother volume
				false,                                                      // no boolean operation
				0,                                     // copy number
				fCheckOverlaps);                                            // checking overlaps


G4double AlBoxThick = 3.*mm; // the thickness of the Al box

// Main: main box, Ext: extension box
MainGas = new G4Box("Main_Gas", // its name
			       x_box/2 - AlBoxThick, 
             y_box/2 - AlBoxThick,  
             z_box/2 - AlBoxThick ); // its size


ExtGas = new G4Box("Ext_Gas", // its name
			       x_extbox/2, 
             y_extbox/2 - AlBoxThick,  
             z_extbox/2 - AlBoxThick); // its size


Gas = new G4UnionSolid("Main_Gas+Ext_Gas", MainGas, ExtGas,0, 
                      G4ThreeVector (x_box/2+x_extbox/2 - AlBoxThick,
                                    0, 
                                    -z_box/2+z_extbox/2));//, nullptr, G4ThreeVector(0., 0., 0.));


GasLV = new G4LogicalVolume(Gas,       // its solid
					                    CO2,  // its material
					                    "GasLV");         // its name
	      
new G4PVPlacement(nullptr,                                                    // no rotation
				G4ThreeVector (0,0, 0),   // its position
				GasLV,                                                      // its logical volume
				"Gas_phy",                                                  // its name
				TaggerLV,                                           // its mother volume
				false,                                                      // no boolean operation
				0,                                     // copy number
				fCheckOverlaps);                                            // checking overlaps




//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// Winston cone construction
// it consists of a mother volume (WCMother) and two shells (WCShell_1 and WCShell_2)
// The mother volume is a bit larger than the Winston cone
// and will containg the PLA skin layer and the GDML-loaded Winston cone



G4double length = 315.47*mm;
G4double OutRadius = 88.91*mm;
G4double InRadius = 76.11*mm;


// The mother volume is a bit larger than the Winston cone
// and will containg the PLA skin layer and the GDML-loaded Winston cone
// I don't need to be very larger than the Winston cone, but I will make it 5um larger

WCMother = new G4Tubs("WinstonConeMother", // name
              0., 
              OutRadius +5*um,
              length/2 + 52*um, // Z half length
              0.*deg, // starting Phi
              360. *deg); // segment angle

WCMotherLV = new G4LogicalVolume(WCMother,       // its solid
    					                  CO2,  // its material
		    			                  "WCMotherLV");         // its name

G4RotationMatrix* yRot = new G4RotationMatrix();
yRot->rotateY(90.0 *deg); 

// the 20*cm offset of the WC should be revisited
G4ThreeVector WCposition (x_box/2 + length/2 - 5*cm, 0, -z_box/2 + 20*cm); // the origin of the solid is NOT at the longitudinal center

new G4PVPlacement(yRot,                                                    // no rotation
				WCposition + G4ThreeVector (0, 0, 0*cm),   // its position
				WCMotherLV,                                                      // its logical volume
				"WCMotherphy",                                                  // its name
				GasLV,                                           // its mother volume
				false,                                                      // no boolean operation
				0,                                     // copy number
				fCheckOverlaps); 


WCMotherLV->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));


WCShell_1 = new G4Tubs("WCS_1", // name
                OutRadius , // inner radius
                OutRadius + 4*um, // outer radius +4um to have the real cone inside
                length/2 + 50*um, // Z half length. I made it 2mm larger to have the real cone inside
                0.*deg, // starting Phi
                360. *deg); // segment angle

WCShell_2 = new G4Tubs("WC_2", // name
                InRadius , // inner radius
                OutRadius +4*um, // outer radius +4um to have the real cone inside
                2*um, // Z half length
                0.*deg, // starting Phi
                360. *deg); // segment angle


WCS1LV = new G4LogicalVolume(WCShell_1,       // its solid
                           PLA,  // its material
                           "WCS1LV");         // its name


WCS2LV = new G4LogicalVolume(WCShell_2,       // its solid
                            PLA,  // its material
                            "WCS2LV");         // its name

 new G4PVPlacement(nullptr,
                    G4ThreeVector (0, 0, 0*cm),   // its position
                    WCS1LV,                                                    // its logical volume
                    "WCS1phy",                                                  // its name
                    WCMotherLV,                                           // its mother volume
                    false,                                                      // no boolean operation
                    0,                                     // copy number
                    fCheckOverlaps); 


new G4PVPlacement(nullptr,
                    G4ThreeVector (0, 0, length/2 + 50*um), //i ts position
                    WCS2LV,                                                      // its logical volume
                    "WCS2phy",                                                  // its name
                    WCMotherLV,                                           // its mother volume
                    false,                                                      // no boolean operation
                    0,                                     // copy number
                    fCheckOverlaps); 



//***************************************************************************** */
// The Winston Cone Solid from a GDML file

fParser.Read("WinstonCone.gdml", false); // reads and stores
// in memory

 // Get the top volume from the GDML file
G4LogicalVolume* gdmlTopVolume = fParser.GetVolume("test_mesh"); 

gdmlTopVolume -> SetMaterial(AluminizedMylar); // The winston cone is made of Aluminized Mylar only in the inner surface
                                              // This is why I covered with a G4Tubs made of PLA  

G4OpticalSurface* aluminizedMylarSurface = new G4OpticalSurface("AluminizedMylarSurface");
aluminizedMylarSurface->SetType(dielectric_metal);
aluminizedMylarSurface->SetFinish(polished);
aluminizedMylarSurface->SetModel(unified);
aluminizedMylarSurface->SetMaterialPropertiesTable(MPTAluminizedMylar);
                                           
new G4LogicalSkinSurface("AluminizedMylarSurface", gdmlTopVolume, aluminizedMylarSurface);


                                              
if (!gdmlTopVolume) {
  G4Exception("DetectorConstruction::Construct()", "InvalidSetup", FatalException, 
               "Failed to load GDML top volume. Check GDML file.");
    }


G4RotationMatrix* antiyRot = new G4RotationMatrix();
antiyRot->rotateY(-90.0 *deg); 



G4double x_offset = 12.7*mm; //this is the distance from the origin of the gdml volume to the near end of the Winston cone
    
new G4PVPlacement(antiyRot,               // the lonfitudinal axis is on Z-direction
                     G4ThreeVector(0, 0, -(length/2 -x_offset)), // the origin of the solid is NOT at the longitudinal center
                     gdmlTopVolume,         // GDML-loaded volume
                     "GDML_Imported",       // Name
                      WCMotherLV,                  // Place inside world
                      false,                 // No boolean operation
                      0,                      // Copy number
                      true);                  // Check overlaps
//***************************************************************************** */




//*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*/
// Mirror

// This is the height of the center of the mirror according Alexander design
G4double dist_ori = 18.73*cm;

G4RotationMatrix* MirrorRot = new G4RotationMatrix();
MirrorRot->rotateY(135.0 *deg); 


G4double kMirrorRad = 40.5/2. *cm;
G4double kFocalLength = 1 *m;
G4double sphererad = 2*kFocalLength;

G4double kMirrorSag = kMirrorRad * kMirrorRad / 4. / kFocalLength; 

// Mirror Surface
G4Sphere* MirrorSurfPhys = 
  new G4Sphere("MirrorSurfPhys", sphererad - 5*mm, sphererad, 0.*deg, 360.*deg, 0.*deg, 5.81*deg);

MirrorSurfLV = new G4LogicalVolume(MirrorSurfPhys,       // its solid 
                              Acrylic,  // its material
                              "MirrorSurfLV");         

                              
new G4PVPlacement(MirrorRot,               // the lonfitudinal axis is on Z-direction
                  G4ThreeVector(-3*cm+ sphererad*cos(45*deg), 0, -z_box/2 + dist_ori+ sphererad*cos(45*deg)), // the origin of the solid is NOT at the longitudinal center
                  MirrorSurfLV,
                  "MirrorSurf",       // Name
                  GasLV,                  // Place inside world
                  false,                 // No boolean operation
                  0,                      // Copy number
                  true);                  // Check overlaps


// I calculated the 5.81deg angle by hand, it would be nice to check it
G4Sphere* MirrorPhys = 
  new G4Sphere("MirrorPhys", sphererad, sphererad+1*um, 0.*deg, 360.*deg, 0.*deg, 5.81*deg);


MirrorLV = new G4LogicalVolume(MirrorPhys,       // its solid
					                   Silver,  // its material
					                  "MirrorLV");    


// Attach the optical surface to the logical volume
G4OpticalSurface* opticalSurface = new G4OpticalSurface("SilverSurface");
opticalSurface->SetType(dielectric_metal);
opticalSurface->SetFinish(polished);
opticalSurface->SetModel(unified);
opticalSurface->SetMaterialPropertiesTable(MPTSilver);

new G4LogicalSkinSurface("SilverSurface", MirrorLV, opticalSurface);








// the terms sphererad*cos... are due to the way the Sphere is built
// 3*cm is the correction to have the mirror at the right place
new G4PVPlacement(MirrorRot,               // the lonfitudinal axis is on Z-direction
                     G4ThreeVector(-3*cm+ sphererad*cos(45*deg), 0, -z_box/2 + dist_ori+ sphererad*cos(45*deg)), // the origin of the solid is NOT at the longitudinal center
                     MirrorLV,         // GDML-loaded volume
                     "Mirror",       // Name
                     GasLV,                  // Place inside world
                      false,                 // No boolean operation
                      0,                      // Copy number
                      true);                  // Check overlaps
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*/


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*/
// A DISK as sensitive detector to emulate the PMT

// Define the dimensions of the disk
G4double innerRadius = 0.0 * mm;  // Inner radius of the disk
G4double outerRadius = 76.2 * mm /2.; // Outer radius of the disk, 3 inches diameter
//G4double halfThickness = 7.0 * mm; // Half-thickness (Z half-length) 14mm thickness of the window

G4double halfThickness = 12.7 * mm /2; // Half-thickness (Z half-length), the thickness of the WC groove where the PMT is placed 14mm thickness of the window
G4double startAngle = 0.0 * deg;   // Starting angle of the disk
G4double spanningAngle = 360.0 * deg; // Spanning angle of the disk

// Create the disk solid
PMT = new G4Tubs("Disk", 
                               innerRadius, 
                               outerRadius, 
                               halfThickness, 
                               startAngle, 
                               spanningAngle);


// Create the logical volume for the disk
PMTLV = new G4LogicalVolume(PMT, 
                                            BorosilicateGlass, 
                                            "PMTLV");

// Place the disk in the world volume or another mother volume
G4ThreeVector diskPosition(0, 0, 10.0 * mm); // Position of the disk
new G4PVPlacement(nullptr, // No rotation
                G4ThreeVector(0, 0, -(length/2 - 12.7/2)), //remember the WCmother is rotated wrt the world
                PMTLV, 
                "Disk", 
                WCMotherLV, //this is TEMPORARY, it should be, maybe, in the winston cone
                false, 
                0, 
                true);



  // Visualization attributes
  
TaggerLV->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));
GasLV->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
WCS1LV->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
WCS2LV->SetVisAttributes(G4VisAttributes(G4Colour::White()));
gdmlTopVolume->SetVisAttributes(G4VisAttributes(G4Colour::Red()));
MirrorLV ->SetVisAttributes(G4VisAttributes(G4Colour::Green()));
PMTLV->SetVisAttributes(G4VisAttributes(G4Colour::Cyan()));
MirrorSurfLV->SetVisAttributes(G4VisAttributes(G4Colour::Magenta()));
//TaggerLV->SetVisAttributes(G4VisAttributes(G4VisAttributes::GetInvisible() ));
//GasLV->SetVisAttributes(G4VisAttributes(G4VisAttributes::GetInvisible() ));
//WCLV->SetVisAttributes(G4VisAttributes::GetInvisible());
//gdmlTopVolume->SetVisAttributes(G4VisAttributes::GetInvisible());


  //
  // Always return the physical World
  //
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TaggerDetectorConstruction::ConstructSDandField()
{
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  // G4ThreeVector fieldValue;
  // fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  // fMagFieldMessenger->SetVerboseLevel(1);

  // // Register the field messenger for deleting
  // G4AutoDelete::Register(fMagFieldMessenger);

  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  G4SDManager* SDManager = G4SDManager::GetSDMpointer();
  G4String SDname;
  taggerPMTSD* taggerPMT = new taggerPMTSD("/taggerPMT");
  SDManager->AddNewDetector(taggerPMT);

  PMTLV-> SetSensitiveDetector(taggerPMT);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
