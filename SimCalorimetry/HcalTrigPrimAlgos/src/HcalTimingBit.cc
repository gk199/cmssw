#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalTimingBit.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"

#include <cassert>

std::bitset<4> HcalTimingBit::compute(int ibin, HcalUpgradeTriggerPrimitiveDigi& digi_1, HcalUpgradeTriggerPrimitiveDigi& digi_2, HcalUpgradeTriggerPrimitiveDigi& digi_3, HcalUpgradeTriggerPrimitiveDigi& digi_4) const //, HcalTrigPrimDigiCollection& digi_collection) const
{
  int Ndelayed_2x2 = 0;
  // group towers in 2x2 ieta, iphi regions
  // with 1 bit per tower, this gives a group of 4 bits
  // 3 of these bits are used for the timing bit (counting number of cells above time and energy values)
  // the 4th bit is used for the depth information

  HcalTrigTowerDetId id_1 = digi_1.id();
  id_1 = HcalTrigTowerDetId(id_1.ieta(), id_1.iphi(), 1, id_1.version());
  int tp_ieta_1 = id_1.ieta();
  int tp_iphi_1 = id_1.ieta();

  HcalTrigTowerDetId id_2 = digi_2.id();
  id_2 = HcalTrigTowerDetId(id_2.ieta(), id_2.iphi(), 1, id_2.version());
  int tp_ieta_2 = id_2.ieta();
  int tp_iphi_2 = id_2.ieta();

  HcalTrigTowerDetId id_3 = digi_3.id();
  id_3 = HcalTrigTowerDetId(id_3.ieta(), id_3.iphi(), 1, id_3.version());
  int tp_ieta_3 = id_3.ieta();
  int tp_iphi_3 = id_3.ieta();

  HcalTrigTowerDetId id_4 = digi_4.id();
  id_4 = HcalTrigTowerDetId(id_4.ieta(), id_4.iphi(), 1, id_4.version());
  int tp_ieta_4 = id_4.ieta();
  int tp_iphi_4 = id_4.ieta();

  // for each TP, determine how many cells are delayed. In HB, the TPs have the same size (4 cells). digi.SOI_rising_avg(i+1) gives the TDC with LUT conversion (defined here https://cmssdt.cern.ch/lxr/source/EventFilter/HcalRawToDigi/plugins/HcalDigiToRawuHTR.cc)
  std::vector<int> energy_depth_1 = digi_1.getDepthData();
  std::vector<int> energy_depth_2 = digi_2.getDepthData();
  std::vector<int> energy_depth_3 = digi_3.getDepthData();
  std::vector<int> energy_depth_4 = digi_4.getDepthData();
  if (tp_ieta_1 <= 16) {
    for (int i=0; i<static_cast<int>(energy_depth_1.size());++i) 
      {
	int TDCvalue = digi_1.SOI_rising_avg(i+1);
	int energy = energy_depth_1[i];
	if ( (TDCvalue == 1 || TDCvalue == 2)  && energy > 50) Ndelayed_2x2 += 1;
	int TDCvalue_2 = digi_2.SOI_rising_avg(i+1);
	int energy_2 = energy_depth_2[i];
	if ( (TDCvalue_2 == 1 || TDCvalue_2 == 2)  && energy_2 > 50) Ndelayed_2x2 += 1;
	int TDCvalue_3 = digi_3.SOI_rising_avg(i+1);
	int energy_3 = energy_depth_3[i];
	if ( (TDCvalue_3 == 1 || TDCvalue_3 == 2)  && energy_3 > 50) Ndelayed_2x2 += 1;
	int TDCvalue_4 = digi_4.SOI_rising_avg(i+1);
	int energy_4 = energy_depth_4[i];
	if ( (TDCvalue_4 == 1 || TDCvalue_4 == 2)  && energy_4 > 50) Ndelayed_2x2 += 1;
      }
  }

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



  return Ndelayed_2x2;
}
