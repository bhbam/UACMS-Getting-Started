// -*- C++ -*-
//
// Package:    UACMS-Getting-Started/PhotonAnalyzer
// Class:      PhotonAnalyzer
//
/**\class PhotonAnalyzer PhotonAnalyzer.cc UACMS-Getting-Started/PhotonAnalyzer/plugins/PhotonAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Cilicia Uzziel Perez
//         Created:  Mon, 31 Aug 2020 09:38:51 GMT
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
 #include "FWCore/Utilities/interface/InputTag.h"
 #include "DataFormats/TrackReco/interface/Track.h"
 #include "DataFormats/TrackReco/interface/TrackFwd.h"

 // Misc.
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

//TFileService
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//for the GenParticleCollection and GenParticles
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "TLorentzVector.h"
#include "TH2D.h"
#include "TTree.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using reco::TrackCollection;

class PhotonAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit PhotonAnalyzer(const edm::ParameterSet&);
      ~PhotonAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      // edm::EDGetTokenT<TrackCollection> tracksToken_;  //used to select what tracks to read from configuration file
      edm::Service<TFileService> fs;
      edm::EDGetTokenT<std::vector<reco::GenParticle> > genParticlesToken_;
      edm::InputTag genParticles_;
      edm::InputTag particles_;
      edm::EDGetTokenT<edm::View<pat::Photon> >        photonsMiniAODToken_; //photonCollection


      TTree *fTree;
      //Structs
      struct eventInfo_t {
        Long64_t run;
        Long64_t LS;
        Long64_t evnum;
      };
      eventInfo_t fEventInfo;

      struct genPhotonInfo_t{
        double pt;
        double eta;
        double phi;
        double E;
      };

      genPhotonInfo_t fGenPhoton1;
      genPhotonInfo_t fGenPhoton2;

      struct PhotonInfo_t{
        double pt;
        double eta;
        double phi;
        double E;
        bool isPassLoose;
        bool isPassMedium;
        bool isPassTight;
      };

      PhotonInfo_t fPhoton1;
      PhotonInfo_t fPhoton2;



};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PhotonAnalyzer::PhotonAnalyzer(const edm::ParameterSet& iConfig)
 // :
 //  tracksToken_(consumes<TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tracks")))

{
   //now do what ever initialization is needed
   fTree = fs->make<TTree>("fTree","GENDiphotonTree");

   fTree->Branch("genPhoton1", &fGenPhoton1, "pt/D:eta:phi:E");
   fTree->Branch("genPhoton2", &fGenPhoton2, "pt/D:eta:phi:E");
   fTree->Branch("Photon1", &fPhoton1, "pt/D:eta:phi:E:isPassLoose/B:isPassMedium:isPassTight");
   fTree->Branch("Photon2", &fPhoton2, "pt/D:eta:phi:E:isPassLoose/B:isPassMedium:isPassTight");

   genParticlesToken_   = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genparticles"));
   photonsMiniAODToken_ = consumes<edm::View<pat::Photon> >     (iConfig.getParameter<edm::InputTag>("photonsMiniAOD"));

}


PhotonAnalyzer::~PhotonAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
PhotonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

    // Handle<TrackCollection> tracks;
    // iEvent.getByToken(tracksToken_, tracks);
    // for(TrackCollection::const_iterator itTrack = tracks->begin();
    //     itTrack != tracks->end();
    //     ++itTrack) {
    //   // do something with track parameters, e.g, plot the charge.
    //   // int charge = itTrack->charge();
    // }

    fEventInfo.run = -99999.99;
    fEventInfo.LS = -99999.99;
    fEventInfo.evnum = -99999.99;

    edm::Handle<std::vector<reco::GenParticle> > genParticles;
    edm::Handle<edm::View<pat::Photon> > photons;
    iEvent.getByToken(genParticlesToken_,   genParticles);
    iEvent.getByToken(photonsMiniAODToken_, photons);

    fGenPhoton1.pt = -99999.99;
    fGenPhoton1.eta = -99999.99;
    fGenPhoton1.phi = -99999.99;
    fGenPhoton1.E = -99999.99;

    fGenPhoton2.pt = -99999.99;
    fGenPhoton2.eta = -99999.99;
    fGenPhoton2.phi = -99999.99;
    fGenPhoton2.E = -99999.99;

    fPhoton1.pt = -99999.99;
    fPhoton1.eta = -99999.99;
    fPhoton1.phi = -99999.99;
    fPhoton1.E = -99999.99;

    fPhoton2.pt = -99999.99;
    fPhoton2.eta = -99999.99;
    fPhoton2.phi = -99999.99;
    fPhoton2.E = -99999.99;

    int photoncount = 0;

    for(std::vector<reco::GenParticle>::const_iterator ip = genParticles->begin(); ip != genParticles->end(); ++ip){
     if(ip->status()==1 && ip->pdgId()==22){
        //cout << "Photon end state found" << endl;
       photoncount = photoncount + 1;
       double pt = ip->pt();
       double eta = ip->eta();
       double phi = ip->phi();
       double E = ip->energy();

       //Ordering photons
       if (pt > fGenPhoton1.pt){
           fGenPhoton2.pt = fGenPhoton1.pt;
           fGenPhoton2.eta = fGenPhoton1.eta;
           fGenPhoton2.phi = fGenPhoton1.phi;
           fGenPhoton2.E = fGenPhoton1.E;

           fGenPhoton1.pt = pt;
           fGenPhoton1.eta = eta;
           fGenPhoton1.phi = phi;
           fGenPhoton1.E = E;
       }
       if ((pt < fGenPhoton1.pt) && (pt > fGenPhoton2.pt)){
           fGenPhoton2.pt = pt;
           fGenPhoton2.eta = eta;
           fGenPhoton2.phi = phi;
           fGenPhoton2.E = E;
       }
     }//end photon end state condition
 }//end loop over gen particles
 cout << "Number of photons in event: " << photoncount << endl;

 // Store patPhotons in a vector (for practice)
 std::vector<edm::Ptr<pat::Photon>> patPhotons;
 for (size_t i = 0; i < photons->size(); i++){
   const auto pho = photons->ptrAt(i);
   patPhotons.push_back(pho);
 }

 // Loop over the patPhotons (an example on how to iterate over the elements of a vector)
 const pat::Photon *patPho;
 for (std::vector<int>::size_type j = 0; j != patPhotons.size(); j++)
 {
   patPho = &(*patPhotons.at(j));
   double pt = patPho->pt();
   double eta = patPho->eta();
   double phi = patPho->phi();
   double E = patPho->energy();
   bool isPassLoose  = patPho->photonID("cutBasedPhotonID-Fall17-94X-V2-loose");
   bool isPassMedium = patPho->photonID("cutBasedPhotonID-Fall17-94X-V2-medium");
   bool isPassTight  = patPho->photonID("cutBasedPhotonID-Fall17-94X-V2-tight");
   if (isPassLoose) std::cout << "Cette Photon a passé le Loose Photon ID" << std::endl;
   else if (isPassMedium) std::cout << "Cette Photon a passé le Medium Photon ID" << std::endl;
   else if (isPassTight) std::cout << "Cette Photon a passé le Tight Photon ID" << std::endl;
   else std::cout << "Cett Photon n'a passé aucun des Photon ID catégories " << std::endl;

   // Ordering
   //Ordering photons
   if (pt > fPhoton1.pt){
       fPhoton2.pt = fPhoton1.pt;
       fPhoton2.eta = fPhoton1.eta;
       fPhoton2.phi = fPhoton1.phi;
       fPhoton2.E = fPhoton1.E;
       fPhoton2.isPassLoose = fPhoton1.isPassLoose;
       fPhoton2.isPassMedium = fPhoton1.isPassMedium;
       fPhoton2.isPassTight = fPhoton1.isPassTight;

       fPhoton1.pt = pt;
       fPhoton1.eta = eta;
       fPhoton1.phi = phi;
       fPhoton1.E = E;
       fPhoton1.isPassLoose = isPassLoose;
       fPhoton1.isPassMedium = isPassMedium;
       fPhoton1.isPassTight = isPassTight;
   }
   if ((pt < fPhoton1.pt) && (pt > fPhoton2.pt)){
       fPhoton2.pt = pt;
       fPhoton2.eta = eta;
       fPhoton2.phi = phi;
       fPhoton2.E = E;
       fPhoton2.isPassLoose = isPassLoose;
       fPhoton2.isPassMedium = isPassMedium;
       fPhoton2.isPassTight = isPassTight;
   }
 } // end loop over patPhotons

 /* Alternative way of looping over the photonCollection
 * for (edm::View<pat::Photon>::const_iterator iPho = photons->begin(); iPho != photons->end(); ++iPho) {
 *    double E = iPho->energy(); }
 */
 //Apply cuts offline

 //Fill the tree Branches
  fTree->Fill();




#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
PhotonAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
PhotonAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PhotonAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PhotonAnalyzer);
