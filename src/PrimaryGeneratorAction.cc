///////////////////////////////////////////////////////////////////////////////////////
//
// Apr/2015  E. Nacher --> PrimaryGeneratorAction.cc
//
// Based on the /gps method. This simplifies life, but...
// The PrimaryGeneratorAction must be instantiated after initialization of the 
// runManager in the main.cc:  
//                          runManager->Initialize();
//                          runManager->SetUserAction(new PrimaryGeneratorAction);
//
///////////////////////////////////////////////////////////////////////////////////////

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"

#include "G4ios.hh"
#include "fstream"
#include "iomanip"
#include "G4GeneralParticleSource.hh" 
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	
	// Default values  
	
	particleGun = new G4GeneralParticleSource();
	particleGun->SetCurrentSourceIntensity (1);
	particleGun->SetParticlePosition(G4ThreeVector());
	
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	//TFile* efile = new TFile("Co_60.root", "READ");
    //TH1F*  ehist = (TH1F*)efile->Get("hist");
    //create vertex
    double theta, phi;
    theta = 2*(G4UniformRand() -0.5)* pi;
    phi = std::acos(1 - (1-std::cos(1./180.*pi)) * G4UniformRand());
    double x = std::cos(phi);
    double y = std::sin(phi)*std::cos(theta);
    double z = std::sin(phi)*std::sin(theta);
    G4ThreeVector dir= G4ThreeVector(x, y, z);
    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(dir);
	particleGun->GeneratePrimaryVertex(anEvent);
}
