#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalTimingBit.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"

#include <cassert>

// Compute takes 1 TP digi, and reports if the tower is delayed (has a delayed cell and no prompt cells)

std::bitset<6> HcalTimingBit::compute(int ibin, HcalUpgradeTriggerPrimitiveDigi& digi) const
{
  int Ndelayed = 0;
  int NveryDelayed = 0;
  int Nprompt = 0;
  const int MinE_ieta20 = 64;
  const int MinE_ieta21 = 128;

  int DeepEnergy = 0;
  int EarlyEnergy = 0;
  const int deepLayerMinE_ieta20 = 80;
  const int earlyLayerMaxE_ieta20 = 16;
  const int deepLayerMinE_ieta21 = 160;
  const int earlyLayerMaxE_ieta21 = 32;

  HcalTrigTowerDetId id = digi.id(); 
  int tp_ieta = id.ieta();

  // for each TP, determine how many cells are delayed. digi.SOI_rising_avg(i+1) gives the TDC with LUT conversion (defined here https://cmssdt.cern.ch/lxr/source/EventFilter/HcalRawToDigi/plugins/HcalDigiToRawuHTR.cc)
  std::vector<int> energy_depth = digi.getDepthData();  

  for (int i = 1; i < static_cast<int>(energy_depth.size()); ++i) {
    int TDCvalue;
    if (abs(tp_ieta) > 16) TDCvalue = 2*digi.SOI_rising_avg(i+1); // TDC value in HE (0-50), in half ns steps
    else TDCvalue = digi.SOI_rising_avg(i+1); // compressed TDC, 0-3 in HB
    int energy = energy_depth[i];

    // timing trigger
    // in HB, TDC values are compressed
    if (abs(tp_ieta) <= 16) {
      if ( (TDCvalue == 1) && energy >= MinE_ieta20) Ndelayed += 1;
      if ( (TDCvalue == 2) && energy >= MinE_ieta20) NveryDelayed += 1;
      if (TDCvalue == 0 && energy >= MinE_ieta20) Nprompt += 1;
    }

    // in HE, TDC values are uncompressed 
    if (abs(tp_ieta) > 16 && i >= 2) { // in HE exclude depth layer 1 due to backgrounds
      if (abs(tp_ieta) <= 20) {
	if ( TDCvalue > tdc_HE[abs(tp_ieta)-1][i-1] && TDCvalue <= tdc_HE[abs(tp_ieta)-1][i-1]+2 && energy >= MinE_ieta20) Ndelayed += 1;
        if ( TDCvalue > tdc_HE[abs(tp_ieta)-1][i-1]+2 && energy >= MinE_ieta20) NveryDelayed += 1;
        if ( TDCvalue <= tdc_HE[abs(tp_ieta)-1][i-1] && TDCvalue >= 0 && energy >= MinE_ieta20) Nprompt += 1;
      } 
      if (abs(tp_ieta) >= 21) {
	if ( TDCvalue > tdc_HE[abs(tp_ieta)-1][i-1] && TDCvalue <= tdc_HE[abs(tp_ieta)-1][i-1]+2 && energy >= MinE_ieta21) Ndelayed += 1;
        if ( TDCvalue > tdc_HE[abs(tp_ieta)-1][i-1]+2 && energy >= MinE_ieta21) NveryDelayed += 1;
        if ( TDCvalue <= tdc_HE[abs(tp_ieta)-1][i-1] && TDCvalue >= 0 && energy >= MinE_ieta21) Nprompt += 1;
      }
    }

    // depth trigger
    if (i <= 2) { // early layers
      if (abs(tp_ieta) <= 20 && energy > earlyLayerMaxE_ieta20) EarlyEnergy += 1;
      if (abs(tp_ieta) >= 21 && energy > earlyLayerMaxE_ieta21) EarlyEnergy += 1;
    }
    if (i >= 3) { // deep layers
      if (abs(tp_ieta) <= 20 && energy > deepLayerMinE_ieta20) DeepEnergy += 1;
      if (abs(tp_ieta) >= 21 && energy > deepLayerMinE_ieta21) DeepEnergy += 1;
    }
  }

  int Depth1_Timing5 = 0; // depth 10 10 01 01 veto
  if (DeepEnergy > 0 && EarlyEnergy == 0) Depth1_Timing5 += 32; // 100000
  Depth1_Timing5 += std::min(2*Ndelayed, 6); // 000110
  Depth1_Timing5 += std::min(8*NveryDelayed, 24); // 011000
  if (Nprompt > 0) Depth1_Timing5 += 1; // 000001

  return Depth1_Timing5;
}
