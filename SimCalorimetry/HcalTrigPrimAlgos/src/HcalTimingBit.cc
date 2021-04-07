#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalTimingBit.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"

#include <cassert>

std::bitset<4> HcalTimingBit::compute(int ibin, std::vector<HcalUpgradeTriggerPrimitiveDigi> digis) const
{
  int Ndelayed_2x2 = 0;

  // define prompt-delayed TDC range. Note this is offset from depth and ieta by 1
  const int tdc_HE[29][7] = {{8, 14, 15, 17, 0, 0, 0},
                             {8, 14, 15, 17, 0, 0, 0},
                             {8, 14, 14, 17, 0, 0, 0},
                             {8, 14, 14, 17, 0, 0, 0},
                             {8, 13, 14, 16, 0, 0, 0},
                             {8, 13, 14, 16, 0, 0, 0},
                             {8, 12, 14, 15, 0, 0, 0},
                             {8, 12, 14, 15, 0, 0, 0},
                             {7, 12, 13, 15, 0, 0, 0},
                             {7, 12, 13, 15, 0, 0, 0},
                             {7, 12, 13, 15, 0, 0, 0},
                             {7, 12, 13, 15, 0, 0, 0},
                             {7, 11, 12, 14, 0, 0, 0},
                             {7, 11, 12, 14, 0, 0, 0},
                             {7, 11, 12, 14, 0, 0, 0},
                             {7, 11, 12, 7, 0, 0, 0},
                             {0, 12, 10, 0, 0, 0, 0},
                             {0, 9, 10, 9, 10, 0, 0},
                             {16, 9, 9, 9, 11, 10, 0},
                             {17, 8, 9, 8, 9, 10, 0},
                             {9, 7, 7, 7, 9, 6, 0},
                             {8, 7, 7, 6, 6, 6, 0},
                             {8, 6, 6, 6, 7, 7, 0},
                             {7, 6, 6, 6, 7, 6, 0},
                             {7, 6, 6, 6, 6, 6, 0},
                             {6, 6, 6, 6, 6, 6, 0},
                             {6, 5, 6, 6, 6, 7, 10},
                             {9, 9, 9, 5, 5, 6, 6},
                             {0, 0, 0, 0, 0, 0, 0}};

  // group towers in 2x2 ieta, iphi regions
  // with 1 bit per tower, this gives a group of 4 bits
  // 3 of these bits are used for the timing bit (counting number of cells above time and energy values)
  // the 4th bit is used for the depth information

  assert (digis.size() == 4); // check that we have 4 digis in 2x2 grouping
  for (auto& digi: digis) { // loop over 4 TP digis in grouping
    HcalTrigTowerDetId id = digi.id();
    //  id = HcalTrigTowerDetId(id.ieta(), id.iphi(), 1, id.version());
    int tp_ieta = id.ieta();
    int tp_iphi = id.iphi();

    // for each TP, determine how many cells are delayed. In HB, the TPs have the same size (4 cells). digi.SOI_rising_avg(i+1) gives the TDC with LUT conversion (defined here https://cmssdt.cern.ch/lxr/source/EventFilter/HcalRawToDigi/plugins/HcalDigiToRawuHTR.cc)
    std::vector<int> energy_depth = digi.getDepthData();
    if (abs(tp_ieta) <= 16) {
      for (int i=0; i<static_cast<int>(energy_depth.size());++i) 
	{
	  int TDCvalue = digi.SOI_rising_avg(i+1);
	  int energy = energy_depth[i];
	  if ( (TDCvalue == 1 || TDCvalue == 2)  && energy > 50) Ndelayed_2x2 += 1;
	}
    }
    
    // in HE, neighboring towers in 2x2 may not have same depth. Need to refernce above array for prompt-delayed TDC range
    if (abs(tp_ieta) > 16) {
      for (int i=0; i<static_cast<int>(energy_depth.size());++i) {
	int TDCvalue = digi.SOI_rising_avg(i+1);
	int energy = energy_depth[i];
	if ( (TDCvalue > tdc_HE[abs(tp_ieta)-1][i]) && energy > 50) Ndelayed_2x2 += 1;
      }
    }
  } // closing loop over the digis, have computed number of delayed cells in 2x2 grouping

  if (version_timing_ == 1) {
    if (Ndelayed_2x2 == 16) Ndelayed_2x2 = 15; // map 16 delayed cells onto 15 delayed cells to use 4 bits to report
  }
  if (version_timing_ == 2) {
    if (Ndelayed_2x2 >= 8) Ndelayed_2x2 = 7; // map 8-16 delayed cells onto 7 delayed cells to use 3 bits to report
    int depth_bit = 0;
    Ndelayed_2x2 += depth_bit * 8; // 4th bit may be used for depth / H/E based trigger
  }

  return Ndelayed_2x2;
}
