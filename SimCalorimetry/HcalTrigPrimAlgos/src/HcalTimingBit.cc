#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalTimingBit.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"

#include <cassert>

// Two compute methods are defined. One takes 4 digis, and counts delayed cells, along with an option to set a depth bit (1 bit per tower, in 2x2 encoding). One takes 1 digi, and counts delayed cells (3 bits per tower).

std::bitset<4> HcalTimingBit::compute(int ibin, std::vector<HcalUpgradeTriggerPrimitiveDigi> digis) const
{
  int Ndelayed_2x2 = 0;

  // group towers in 2x2 ieta, iphi regions. With 1 bit per tower, this gives a group of 4 bits
  // 3 of these bits are used for the timing bit (counting number of cells above time and energy values). The 4th bit is used for the depth information

  assert (digis.size() == 4); // check that we have 4 digis in 2x2 grouping
  for (auto& digi: digis) { // loop over 4 TP digis in grouping
    HcalTrigTowerDetId id = digi.id();
    int tp_ieta = id.ieta();
    //    int tp_iphi = id.iphi();

    // for each TP, determine how many cells are delayed. digi.SOI_rising_avg(i+1) gives the TDC with LUT conversion (defined here https://cmssdt.cern.ch/lxr/source/EventFilter/HcalRawToDigi/plugins/HcalDigiToRawuHTR.cc)
    std::vector<int> energy_depth = digi.getDepthData();
    if (abs(tp_ieta) <= 16) {
      for (int i=1; i<static_cast<int>(energy_depth.size());++i) 
	{
	  int TDCvalue = digi.SOI_rising_avg(i+1);
	  int energy = energy_depth[i];
	  if ( (TDCvalue == 1 || TDCvalue == 2)  && energy > 50) Ndelayed_2x2 += 1;
	}
    }
    
    if (abs(tp_ieta) > 16) {
      for (int i=2; i<static_cast<int>(energy_depth.size());++i) { // start loop at i = 2 to exclude depth 1 HE
	int TDCvalue = digi.SOI_rising_avg(i+1);
	int energy = energy_depth[i];
	if (abs(tp_ieta) <= 20) {
	  if ( (TDCvalue > tdc_HE[abs(tp_ieta)-1][i]) && energy > 50) Ndelayed_2x2 += 1;
	}
	if (abs(tp_ieta) >= 21) {
	  if ( (TDCvalue > tdc_HE[abs(tp_ieta)-1][i]) && energy > 105) Ndelayed_2x2 += 1;
	}
      }
    }
  } // closing loop over the digis, have computed number of delayed cells in 2x2 grouping

  if (version_timing_ == 1) {
    if (Ndelayed_2x2 >= 16) Ndelayed_2x2 = 15; // map 16 delayed cells onto 15 delayed cells to use 4 bits to report
  }
  if (version_timing_ == 2) {
    if (Ndelayed_2x2 >= 8) Ndelayed_2x2 = 7; // map 8-16 delayed cells onto 7 delayed cells to use 3 bits to report
    int depth_bit = 0;
    Ndelayed_2x2 += depth_bit * 8; // 4th bit may be used for depth / H/E based trigger
  }

  return Ndelayed_2x2;
}



std::bitset<3> HcalTimingBit::compute(int ibin, HcalUpgradeTriggerPrimitiveDigi& digi) const
{
  int Ndelayed = 0;

  HcalTrigTowerDetId id = digi.id(); 
  int tp_ieta = id.ieta();
  //  int tp_iphi = id.iphi();

  std::vector<int> energy_depth = digi.getDepthData();  

  for (int i = 1; i < static_cast<int>(energy_depth.size()); ++i) {
    int TDCvalue = digi.SOI_rising_avg(i+1);
    int energy = energy_depth[i];

    if (abs(tp_ieta) <= 16) {
      if ( (TDCvalue == 1 || TDCvalue == 2) && energy > 50) Ndelayed += 1;
    }

    if (abs(tp_ieta) > 16 && i >= 2) { // in HE exclude depth layer 1 due to backgrounds
      if (abs(tp_ieta) <= 20) {
	if ( (TDCvalue > tdc_HE[abs(tp_ieta)-1][i]) && energy > 50) Ndelayed += 1;
      } 
      if (abs(tp_ieta) >= 21) {
	if ( (TDCvalue > tdc_HE[abs(tp_ieta)-1][i]) && energy > 105) Ndelayed += 1;
      }
    }
  }

  return Ndelayed;
}
