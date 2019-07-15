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

 #include "CondFormats/JetMETObjects/interface/JetResolution.h"
 #include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
 #include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

 #include "RecoBTag/SecondaryVertex/interface/SecondaryVertex.h"
 #include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"

 #include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

 #include "EgammaAnalysis/ElectronTools/interface/ElectronEnergyCalibratorRun2.h"
 #include "EgammaAnalysis/ElectronTools/interface/PhotonEnergyCalibratorRun2.h"
 #include "EgammaAnalysis/ElectronTools/interface/EnergyScaleCorrection_class.h"

 #include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
 #include "DataFormats/EgammaCandidates/interface/Conversion.h"
 #include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "UserCode/bsmhiggs_fwk/interface/BSMPhysicsEvent.h"

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
typedef std::vector<PhysicsObject>        PhysicsObjectCollection;
using namespace edm;
using namespace reco;
using namespace pat;
using namespace std;

class GenProcessAnalyzer : public edm::one::EDAnalyzer<edm::one::WatchRuns> {
  do you fail test
  public:
    explicit GenProcessAnalyzer(const edm::ParameterSet&);
    ~GenProcessAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    const reco::Candidate* findFirstMotherWithDifferentID(const reco::Candidate *particle);

    static bool ptsort(const reco::GenParticle x, const reco::GenParticle y)
      { return x.pt()>y.pt(); }
  private:

    edm::EDGetTokenT<edm::View<reco::GenParticle> > GenTag_;

    void beginJob() override;
    void beginRun(edm::Run const& iRun, edm::EventSetup const&) override;
    void analyze(const edm::Event&, const edm::EventSetup&) override;
    void endRun(edm::Run const& iRun, edm::EventSetup const&) override {};
    void endJob() override;

    bool verbose_;

    TString outfile;
    int num_bs = 4;

    TH1F  *h_num_lep, *h_num_lep_pt10, *h_pt_lep, *h_energy_lep, *h_eta_lep, *h_phi_lep;
    TH1F  *h_num_b, *h_pt_b, *h_energy_b, *h_eta_b, *h_phi_b;
    
    TH1F  *h_num_lep_allP, *h_num_lep_pt10_allP, *h_pt_lep_allP, *h_energy_lep_allP, *h_eta_lep_allP, *h_phi_lep_allP;
    TH1F  *h_num_b_allP, *h_pt_b_allP, *h_energy_b_allP, *h_eta_b_allP, *h_phi_b_allP;

    std::map<int, TH1F*> h_pt_4bs;
    std::map<int, TH1F*> h_eta_4bs;
    std::map<int, TH1F*> h_pt_4bs_allP;
    std::map<int, TH1F*> h_eta_4bs_allP;

    TH1F *h_momid_lep_allP, *h_momid_lep;
    std::vector<int> MesonIDs = {111, 411, 421, 521, 511, 221, 431, 531, 223, 443, 333, 113, 541};
    std::vector<int> BaryonIDs = {4122, 5122, 3212, 5232, 4232, 5132, 4132};
    TH1F *h_pt_lep_from_meson, *h_pt_lep_from_baryon, *h_pt_lep_from_WTau;
    TH1F *h_eta_lep_from_meson, *h_eta_lep_from_baryon, *h_eta_lep_from_WTau;
};


GenProcessAnalyzer::GenProcessAnalyzer(const edm::ParameterSet& iConfig):
  GenTag_( consumes<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("GenParticleTag")) ),
  verbose_(   iConfig.getParameter<bool>("verbose")               )
{
//    outfile = "./GENSIM.root";
    edm::Service<TFileService> fs;

    h_num_lep_allP = fs->make< TH1F>("n_lep_allP","Multiplicity of lepton",50,0,50);
    h_num_lep_pt10_allP = fs->make< TH1F>("n_lep_pt10_allP","Multiplicity of lepton, Pt>10GeV |#eta|<2.5",50,0,50);
    h_num_b_allP = fs->make< TH1F>("n_b_allP","Multiplicity of B quarks",50,0,50);
    h_pt_lep_allP = fs->make< TH1F>("pt_lep_allP","Pt Distribution of leptons",150,0,1500);
    h_pt_b_allP = fs->make< TH1F>("pt_b_allP","Pt Distribution of four leading pt b quarks",150,0,1500);
    h_energy_lep_allP = fs->make< TH1F>("energy_lep_allP","Energy Distribution of lepton",50,0,1000);
    h_energy_b_allP = fs->make< TH1F>("energy_b_allP","Energy Distribution of four leading pt b quarks",50,0,1000);
    h_eta_lep_allP = fs->make< TH1F>("eta_lep_allP", "Eta Distribution of lepton",22,-5.5,5.5);
    h_eta_b_allP = fs->make< TH1F>("eta_b_allP", "Eta Distribution of four leading pt b quarks",22,-5.5,5.5);
    h_phi_lep_allP = fs->make< TH1F>("phi_lep_allP","Phi Distribution of lepton",16,-4,4);
    h_phi_b_allP = fs->make< TH1F>("phi_b_allP","Phi Distribution of four leading pt b quarks",16,-4,4);
    
    h_num_lep = fs->make< TH1F>("n_lep","Multiplicity of lepton, hard process",50,0,50);
    h_num_lep_pt10 = fs->make< TH1F>("n_lep_pt10","Multiplicity of lepton, Pt>10GeV |#eta|<2.5, hard process",50,0,50);
    h_num_b = fs->make< TH1F>("n_b","Multiplicity of B quarks, hard process",50,0,50);
    h_pt_lep = fs->make< TH1F>("pt_lep","Pt Distribution of leptons, hard process",150,0,1500);
    h_pt_b = fs->make< TH1F>("pt_b","Pt Distribution of four leading pt b quarks, hard process",150,0,1500);
    h_energy_lep = fs->make< TH1F>("energy_lep","Energy Distribution of lepton, hard process",50,0,1000);
    h_energy_b = fs->make< TH1F>("energy_b","Energy Distribution of four leading pt b quarks, hard process",50,0,1000);
    h_eta_lep = fs->make< TH1F>("eta_lep", "Eta Distribution of lepton, hard process",22,-5.5,5.5);
    h_eta_b = fs->make< TH1F>("eta_b", "Eta Distribution of four leading pt b quarks, hard process",22,-5.5,5.5);
    h_phi_lep = fs->make< TH1F>("phi_lep","Phi Distribution of lepton, hard process",16,-4,4);
    h_phi_b = fs->make< TH1F>("phi_b","Phi Distribution of four leading pt b quarks,hard process",16,-4,4);

    for(int i=0;i<num_bs;i++){
      h_pt_4bs[i] = fs->make< TH1F>(Form("pt_%db",i+1),Form("Pt Distribution of the %d leading pt b quark, hard process",i+1),150,0,1500);
      h_eta_4bs[i] = fs->make< TH1F>(Form("eta_%db",i+1),Form("Eta Distribution of the %d leading pt b quark, hard process",i+1),22,-5.5,5.5);
      h_pt_4bs_allP[i] = fs->make< TH1F>(Form("pt_%db_allP",i+1),Form("Pt Distribution of the %d leading pt b quark",i+1),150,0,1500);
      h_eta_4bs_allP[i] = fs->make< TH1F>(Form("eta_%db_allP",i+1),Form("Eta Distribution of the %d leading pt b quark",i+1),22,-5.5,5.5);
    }

    h_momid_lep_allP = fs->make< TH1F>("momid_lep_allP","mother id of leptons",6000,0,6000);
    h_momid_lep = fs->make< TH1F>("momid_lep","mother id of leptons, hard process",6000,0,6000);
    h_pt_lep_from_meson = fs->make< TH1F>("pt_lep_from_meson","Pt Distribution of leptons from Mesons",150,0,1500);
    h_pt_lep_from_baryon = fs->make< TH1F>("pt_lep_from_baryon","Pt Distribution of leptons from Baryons",150,0,1500);
    h_pt_lep_from_WTau = fs->make< TH1F>("pt_lep_from_WTau","Pt Distribution of leptons from Ws, Taus",150,0,1500);
    h_eta_lep_from_meson = fs->make< TH1F>("eta_lep_from_meson","Eta Distribution of leptons from Mesons",22,-5.5,5.5);
    h_eta_lep_from_baryon = fs->make< TH1F>("eta_lep_from_baryon","Eta Distribution of leptons from Baryons",22,-5.5,5.5);
    h_eta_lep_from_WTau = fs->make< TH1F>("eta_lep_from_WTau","Eta Distribution of leptons from Ws, Taus",22,-5.5,5.5);

}


GenProcessAnalyzer::~GenProcessAnalyzer(){
//    TFile *ofile=TFile::Open(outfile, "recreate");
//    h_num_lep->Write();
//    h_pt_lep->Write();
//    h_energy_lep->Write();
//    h_eta_lep->Write();
//    h_phi_lep->Write();
//    h_num_b->Write();
//    h_pt_b->Write();
//    h_energy_b->Write();
//    h_eta_b->Write();
//    h_phi_b->Write();
//    ofile->Close();
}

void
GenProcessAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& iSetup){

  //gen particles
  //Handle<GenParticleCollection> genParticles;
  Handle<edm::View<reco::GenParticle> > genParticles;
  event.getByToken(GenTag_, genParticles);
  //event.getByLabel("genParticles", genParticles);

  std::vector<reco::GenParticle>  genbs;
  std::vector<reco::GenParticle>  genbs_allP;
  int num_lep_allP = 0;
  int num_lep_pt10_allP = 0;
  int num_b_allP = 0;
  int num_lepton = 0;
  int num_lepton_pt10 = 0;
  int num_b = 0;
  int index = 0;
  for(auto & it: *genParticles){
    index ++;
//  for(size_t i=0; i<genParticles->size(); ++i){
//    auto it = (*genParticles)[i];

    int pdgId = it.pdgId();
    int status = it.status();
    float px = it.px();
    float py = it.py();
    float pz = it.pz();
    float pt = it.pt();
    float eta = it.eta();
    float phi = it.phi();
    float mass = it.mass();
    float energy = it.energy();
    event.put(std::move(pdgid)); // appends PDIG to the step 1 root file

    int momId = 0;
    int momInd = -1;
    const reco::Candidate* mom = findFirstMotherWithDifferentID(&it);
    if (mom){
      momId = mom->pdgId();
    }
 
    // require hard process - this is what LLP will come from //
    if(!it.isHardProcess()) continue;

    if( abs(pdgId)==5 ){ // b quarks - make (good) assumption that any b quark is from the LLP
      genbs.emplace_back(it); 
      num_b ++;
    }
  }
  sort(genbs_allP.begin(), genbs_allP.end(), ptsort); // want to sort b-jets by pt, this is what will make TPs
  int ib_allP = 0;
  for (auto & it : genbs_allP){
    h_pt_b_allP->Fill(it.pt());
    h_energy_b_allP->Fill(it.energy());
    h_eta_b_allP->Fill(it.eta());
    h_phi_b_allP->Fill(it.phi());
    h_pt_4bs_allP[ib_allP]->Fill(it.pt());
    h_eta_4bs_allP[ib_allP]->Fill(it.eta());

    ib_allP ++;
    if (ib_allP >= num_bs) break;     
  }
  
  sort(genbs.begin(), genbs.end(), ptsort);
  int ib = 0;
  if (verbose_) printf("\n\n B quarks Gen Particles: \n");
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
  h_num_lep->Fill(num_lepton);
  h_num_lep_pt10->Fill(num_lepton_pt10);
  h_num_b->Fill(num_b);
  h_num_lep_allP->Fill(num_lep_allP);
  h_num_lep_pt10_allP->Fill(num_lep_pt10_allP);
  h_num_b_allP->Fill(num_b_allP);
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
