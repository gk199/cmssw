#ifndef HcalSimAlgos_HcalTimingBit_h
#define HcalSimAlgos_HcalTimingBit_h

#include <bitset>
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalUpgradeTriggerPrimitiveDigi.h"

class HcalTimingBit {
 public:
  
 HcalTimingBit(int version_timing) : version_timing_(version_timing) {};

  std::bitset<4> compute(int ibin, std::vector<HcalUpgradeTriggerPrimitiveDigi> digis) const;
  
 private:
  int version_timing_;
};

#endif
