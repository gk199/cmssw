#include "SimG4Core/Geometry/interface/DDG4ProductionCuts.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "G4ProductionCuts.hh"
#include "G4RegionStore.hh"

#include <algorithm>

DDG4ProductionCuts::DDG4ProductionCuts(const G4LogicalVolumeToDDLogicalPartMap& map,
                                       int verb,
                                       const edm::ParameterSet& p)
    : map_(map), verbosity_(verb) {
  keywordRegion_ = "CMSCutsRegion";
  // Legacy flag
  protonCut_ = p.getUntrackedParameter<bool>("CutsOnProton", true);
  initialize();
}

DDG4ProductionCuts::~DDG4ProductionCuts() {}

/** helper function to compare parts through their name instead of comparing them
    by their pointers. 
    It's guaranteed to produce the same order in subsequent application runs,
    while pointers usually can't guarantee this
*/
bool dd_is_greater(const std::pair<G4LogicalVolume*, DDLogicalPart>& p1,
                   const std::pair<G4LogicalVolume*, DDLogicalPart>& p2) {
  bool result = false;
  if (p1.second.name().ns() > p2.second.name().ns()) {
    result = true;
  }
  if (p1.second.name().ns() == p2.second.name().ns()) {
    if (p1.second.name().name() > p2.second.name().name()) {
      result = true;
    }
    if (p1.second.name().name() == p2.second.name().name()) {
      if (p1.first->GetName() > p2.first->GetName()) {
        result = true;
      }
    }
  }
  return result;
}

void DDG4ProductionCuts::update() {
  //
  // Loop over all DDLP and provide the cuts for each region
  //
  for (auto const& tit : vec_) {
    setProdCuts(tit.second, tit.first);
  }
}

void DDG4ProductionCuts::initialize() {
  vec_ = map_.all(keywordRegion_);
  // sort all root volumes - to get the same sequence at every run of the application.
  // (otherwise, the sequence will depend on the pointer (memory address) of the
  // involved objects, because 'new' does no guarantee that you allways get a
  // higher (or lower) address when allocating an object of the same type ...
  sort(vec_.begin(), vec_.end(), &dd_is_greater);
  if (verbosity_ > 0) {
    edm::LogVerbatim("Physics") << " DDG4ProductionCuts (New) : starting\n"
                                << " DDG4ProductionCuts : Got " << vec_.size() << " region roots.\n"
                                << " DDG4ProductionCuts : List of all roots:";
    for (size_t jj = 0; jj < vec_.size(); ++jj)
      edm::LogVerbatim("Physics") << "   DDG4ProductionCuts : root=" << vec_[jj].second.name();
  }

  // Now generate all the regions
  for (G4LogicalVolumeToDDLogicalPartMap::Vector::iterator tit = vec_.begin(); tit != vec_.end(); tit++) {
    std::string regionName;
    unsigned int num = map_.toString(keywordRegion_, (*tit).second, regionName);

    if (num != 1) {
      throw cms::Exception("SimG4CorePhysics", " DDG4ProductionCuts::initialize: Problem with Region tags.");
    }
    G4Region* region = getRegion(regionName);
    region->AddRootLogicalVolume((*tit).first);

    if (verbosity_ > 0)
      edm::LogVerbatim("Physics") << " MakeRegions: added " << ((*tit).first)->GetName() << " to region "
                                  << region->GetName();
  }
}

void DDG4ProductionCuts::setProdCuts(const DDLogicalPart lpart, G4LogicalVolume* lvol) {
  if (verbosity_ > 0)
    edm::LogVerbatim("Physics") << " DDG4ProductionCuts: inside setProdCuts";

  G4Region* region = nullptr;

  std::string regionName;
  unsigned int num = map_.toString(keywordRegion_, lpart, regionName);

  if (num != 1) {
    throw cms::Exception("SimG4CorePhysics", " DDG4ProductionCuts::setProdCuts: Problem with Region tags.");
  }
  if (verbosity_ > 0)
    edm::LogVerbatim("Physics") << "Using region " << regionName;

  region = getRegion(regionName);

  //
  // search for production cuts
  // you must have four of them: e+ e- gamma proton
  //
  double gammacut;
  double electroncut;
  double positroncut;
  double protoncut;
  int temp = map_.toDouble("ProdCutsForGamma", lpart, gammacut);
  if (temp != 1) {
    throw cms::Exception(
        "SimG4CorePhysics",
        " DDG4ProductionCuts::setProdCuts: Problem with Region tags - no/more than one ProdCutsForGamma.");
  }
  temp = map_.toDouble("ProdCutsForElectrons", lpart, electroncut);
  if (temp != 1) {
    throw cms::Exception(
        "SimG4CorePhysics",
        " DDG4ProductionCuts::setProdCuts: Problem with Region tags - no/more than one ProdCutsForElectrons.");
  }
  temp = map_.toDouble("ProdCutsForPositrons", lpart, positroncut);
  if (temp != 1) {
    throw cms::Exception(
        "SimG4CorePhysics",
        " DDG4ProductionCuts::setProdCuts: Problem with Region tags - no/more than one ProdCutsForPositrons.");
  }
  temp = map_.toDouble("ProdCutsForProtons", lpart, protoncut);
  if (temp == 0) {
    // There is no ProdCutsForProtons set in XML,
    // check if it's a legacy geometry scenario without it
    if (protonCut_) {
      protoncut = electroncut;
    } else {
      protoncut = 0.;
    }
  } else if (temp != 1) {
    throw cms::Exception(
        "SimG4CorePhysics",
        " DDG4ProductionCuts::setProdCuts: Problem with Region tags - more than one ProdCutsForProtons.");
  }

  //
  // For the moment I assume all of the four are set
  //
  G4ProductionCuts* prodCuts = getProductionCuts(region);
  prodCuts->SetProductionCut(gammacut, idxG4GammaCut);
  prodCuts->SetProductionCut(electroncut, idxG4ElectronCut);
  prodCuts->SetProductionCut(positroncut, idxG4PositronCut);
  prodCuts->SetProductionCut(protoncut, idxG4ProtonCut);
  if (verbosity_ > 0) {
    edm::LogVerbatim("Physics") << "DDG4ProductionCuts : Setting cuts for " << regionName
                                << "\n    Electrons: " << electroncut << "\n    Positrons: " << positroncut
                                << "\n    Gamma    : " << gammacut;
  }
}

G4Region* DDG4ProductionCuts::getRegion(const std::string& regName) {
  G4Region* reg = G4RegionStore::GetInstance()->FindOrCreateRegion(regName);
  return reg;
}

G4ProductionCuts* DDG4ProductionCuts::getProductionCuts(G4Region* reg) {
  G4ProductionCuts* prodCuts = reg->GetProductionCuts();
  if (!prodCuts) {
    prodCuts = new G4ProductionCuts();
    reg->SetProductionCuts(prodCuts);
  }
  return prodCuts;
}
