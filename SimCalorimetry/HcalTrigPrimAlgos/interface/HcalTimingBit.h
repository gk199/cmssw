#ifndef HcalSimAlgos_HcalTimingBit_h
#define HcalSimAlgos_HcalTimingBit_h

#include <bitset>
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalUpgradeTriggerPrimitiveDigi.h"

class HcalTimingBit {
 public:
  
  HcalTimingBit() {};

  std::bitset<4> compute(int ibin, HcalUpgradeTriggerPrimitiveDigi& TPDigi_1, HcalUpgradeTriggerPrimitiveDigi& TPDigi_2, HcalUpgradeTriggerPrimitiveDigi& TPDigi_3, HcalUpgradeTriggerPrimitiveDigi& TPDigi_4) const; //, HcalTrigPrimDigiCollection& Digi_collection) const;
  
 private:
  
};

#endif
