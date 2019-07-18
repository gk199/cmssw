#include <memory>

// user include files
 #include "FWCore/Framework/interface/Frameworkfwd.h"
 #include "FWCore/Framework/interface/one/EDAnalyzer.h"

 #include "FWCore/Framework/interface/Event.h"
 #include "FWCore/Framework/interface/MakerMacros.h"

 #include "FWCore/Framework/interface/Run.h"
 #include "FWCore/Common/interface/TriggerResultsByName.h"
 #include "FWCore/Common/interface/EventBase.h"

 #include "FWCore/ParameterSet/interface/ParameterSet.h"
 #include "FWCore/ServiceRegistry/interface/Service.h"
 #include "CommonTools/UtilAlgos/interface/TFileService.h"

 //Load here all the dataformat that we will need
 #include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
 #include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
 #include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
 #include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
 #include "DataFormats/VertexReco/interface/VertexFwd.h"
 #include "DataFormats/PatCandidates/interface/Muon.h"
 #include "DataFormats/PatCandidates/interface/Electron.h"
 #include "DataFormats/PatCandidates/interface/Jet.h"
 #include "DataFormats/PatCandidates/interface/Photon.h"
 #include "DataFormats/PatCandidates/interface/MET.h"
 #include "DataFormats/PatCandidates/interface/Tau.h"
 #include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
 #include "DataFormats/PatCandidates/interface/GenericParticle.h"
 #include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
 #include "DataFormats/Common/interface/ValueMap.h"
 #include "DataFormats/HepMCCandidate/interface/GenParticle.h"
 #include "DataFormats/Common/interface/ValueMap.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TNtuple.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "Math/LorentzVector.h" 
#include <Math/VectorUtil.h>
#include <algorithm>

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

using namespace edm;
using namespace reco;
using namespace pat;
using namespace std;

class GenProcessAnalyzer : public edm::one::EDAnalyzer<edm::one::WatchRuns> {
  public:
    explicit GenProcessAnalyzer(const edm::ParameterSet&);
    ~GenProcessAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    const reco::Candidate* findFirstMotherWithDifferentID(const reco::Candidate *particle);

  struct ptsort: public std::binary_function<LorentzVector, LorentzVector, bool> 
  {
    bool operator () (const LorentzVector & x, const LorentzVector & y) 
    { return  ( x.pt() > y.pt() ) ; }
  };

  private:

    edm::EDGetTokenT<edm::View<reco::GenParticle> > GenTag_;

    void beginJob() override;
    void beginRun(edm::Run const& iRun, edm::EventSetup const&) override;
    void analyze(const edm::Event&, const edm::EventSetup&) override;
    void endRun(edm::Run const& iRun, edm::EventSetup const&) override {};
    void endJob() override;

    bool verbose_;

  //TString outfile;
    int num_bs = 4;

  // make histograms for the ideal (full gen info) pt, eta, and phi of the b quarks
    std::map<int, TH1F*> h_pt_b;
    std::map<int, TH1F*> h_eta_b;
    std::map<int, TH1F*> h_phi_b;
    
  // make histograms for the realistic (using eta and pt cuts) pt, eta, and phi of the b quarks
    std::map<int, TH1F*> h_pt_b_realistic;
    std::map<int, TH1F*> h_eta_b_realistic;
    std::map<int, TH1F*> h_phi_b_realistic;

    TH1F *h_momid_lep_allP, *h_momid_lep;
    std::vector<int> MesonIDs = {111, 411, 421, 521, 511, 221, 431, 531, 223, 443, 333, 113, 541};
    std::vector<int> BaryonIDs = {4122, 5122, 3212, 5232, 4232, 5132, 4132};
};


GenProcessAnalyzer::GenProcessAnalyzer(const edm::ParameterSet& iConfig):
  GenTag_( consumes<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("GenParticleTag")) ),
  verbose_(   iConfig.getParameter<bool>("verbose")               )
{
  //  outfile = "./gen.root";
    edm::Service<TFileService> fs;

    printf("Definition of plots\n");
    for(int i=0;i<4;i++){
      // make histograms for the ideal (full gen info) pt, eta, and phi of the b quarks         
      h_pt_b[i] = fs->make< TH1F>(Form("pt_%db",i+1),Form("Pt Distribution of the %d leading pt b quark, hard process",i+1),150,0,1500);
      h_eta_b[i] = fs->make< TH1F>(Form("eta_%db",i+1),Form("Eta Distribution of the %d leading pt b quark, hard process",i+1),22,-5.5,5.5);
      h_phi_b[i] = fs->make< TH1F>(Form("phi_%db",i+1),Form("Phi Distribution of the %d leading pt b quark, hard process",i+1),50,-3.5,3.5);
      // make histograms for the realistic (using eta and pt cuts) pt, eta, and phi of the b quarks                     
      h_pt_b_realistic[i] = fs->make< TH1F>(Form("pt_%db_realistic",i+1),Form("Pt Distribution (with cuts) of the %d leading pt b quark, hard process",i+1),150,0,1500);
      h_eta_b_realistic[i] = fs->make< TH1F>(Form("eta_%db_realistic",i+1),Form("Eta Distribution (with cuts) of the %d leading pt b quark, hard process",i+1),22,-5.5,5.5);
      h_phi_b_realistic[i] = fs->make< TH1F>(Form("phi_%db_realistic",i+1),Form("Phi Distribution (with cuts) of the %d leading pt b quark, hard process",i+1),50,-3.5,3.5); 
   }

}


GenProcessAnalyzer::~GenProcessAnalyzer(){
  /*
  TFile *ofile=TFile::Open(outfile, "recreate");
  for(int i=0;i<4;i++){
    h_pt_b[i]->Write();
  }
//    h_pt_b->Write();
//    h_eta_b->Write();
//    h_phi_b->Write();
    ofile->Close();
  */
}

void
GenProcessAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& iSetup){

  //gen particles
  Handle<edm::View<reco::GenParticle> > genParticles;
  event.getByToken(GenTag_, genParticles);

  std::vector<LorentzVector > genbs;

  int num_b = 0;
  int index = 0;
  // loop over the gen particles
  for(auto & it: *genParticles){
    index ++;
    // require hard process - this is what LLP will come from 
    if(!it.isHardProcess()) continue;

    const reco::Candidate* mom = findFirstMotherWithDifferentID(&it);
    /*
    if (mom){
      momId = mom->pdgId();
    }
    */

    int pdgId = abs(it.pdgId());
    int status = it.status();
    //    float px = it.px();
    //    float py = it.py();
    //    float pz = it.pz();
    float pt = it.pt();
    float eta = it.eta();
    float phi = it.phi();
    float mass = it.mass();
    //    float energy = it.energy();
    //    event.put(std::move(pdgId)); // appends PDIG to the step 1 root file

    if ( verbose_ ) {
      printf("  %3d : ID=%6d, m=%5.1f, momID=%6d : pt=%6.1f, eta=%7.3f, phi=%7.3f, status=%d\n",
	     index,
	     pdgId,
	     mass,
	     mom->pdgId(),
	     pt,
	     eta,
	     phi,
	     status
	     );
    }

    LorentzVector p4( it.px(), it.py(), it.pz(), it.energy() );

    if( abs(pdgId)==5 ){ // b quarks - make (good) assumption that any b quark is from the LLP
      genbs.push_back(p4); 
      num_b ++;
    }
  }

  sort(genbs.begin(), genbs.end(), ptsort()); // want to sort b-jets by pt, this is what will make TPs

  if (verbose_) printf("\n\n B quarks Gen Particles: \n");

  for(int i=0;i<4;i++){

    // fill the ideal version of the histogram                                                                         
    h_pt_b[i]->Fill(genbs[i].pt());
    h_eta_b[i]->Fill(genbs[i].eta());
    h_phi_b[i]->Fill(genbs[i].phi());

    // make cuts for the realistic version of the histogram     
    // these are the detector cuts for the hard scattering process 
    if (genbs[i].pt() > 10 && abs(genbs[i].eta()) < 2.5){
      h_pt_b_realistic[i]->Fill(genbs[i].pt());
      h_eta_b_realistic[i]->Fill(genbs[i].eta());
      h_phi_b_realistic[i]->Fill(genbs[i].phi());
    }
  }

  /* // this is filling pt, energy, eta, phi for all pt, not for b quarks sorted by pt
  for (auto & it : genbs){
    h_pt_b->Fill(it.pt());
    h_energy_b->Fill(it.energy());
    h_eta_b->Fill(it.eta());
    h_phi_b->Fill(it.phi());
    h_pt_4bs[ib]->Fill(it.pt());
    h_eta_4bs[ib]->Fill(it.eta());
    
    ib ++;
    int _momId = 0;
    auto _mom = findFirstMotherWithDifferentID(&it);
    if (_mom) _momId = _mom->pdgId();
    if (verbose_) printf("Gen Particles: ID=%d, momID=%d,mass=%5.1f, pt=%6.1f,energy=%6.1f,status=%d\n",it.pdgId(),_momId,it.mass(),it.pt(),it.energy(),it.status());
    if (ib >= num_bs) break;
  }
  */

  //  h_num_b->Fill(num_b);

}

const reco::Candidate* GenProcessAnalyzer::findFirstMotherWithDifferentID(const reco::Candidate *particle){
  if( particle == 0 ) {
    printf("ERROR! null candidate pointer, this should never happen\n");
    return 0;
  }
  else if (particle->numberOfMothers() > 0 && particle->pdgId() != 0) {
    if (particle->pdgId() == particle->mother(0)->pdgId()) {
      return findFirstMotherWithDifferentID(particle->mother(0));
    }else{
      return particle->mother(0);
    }
  }
  return 0;
}

void 
GenProcessAnalyzer::beginJob() 
{
}

void 
GenProcessAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const&){
}

void 
GenProcessAnalyzer::endJob() 
{
}

void
GenProcessAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenProcessAnalyzer);
