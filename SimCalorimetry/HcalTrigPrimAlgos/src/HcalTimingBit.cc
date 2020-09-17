#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalTimingBit.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"

#include <cassert>
#include <iostream>

std::bitset<3> HcalTimingBit::compute(int ibin, HcalUpgradeTriggerPrimitiveDigi& digi) const
{
      //std::bitset<3> result;

      std::vector<int> energy_depth = digi.getDepthData();
      int N_cell=0;
      double depth_tdc_rise = -99.;

      HcalTrigTowerDetId id = digi.id(); 
      id = HcalTrigTowerDetId(id.ieta(), id.iphi(), 1, id.version());
      int tp_ieta_ = id.ieta();
      //int tp_iphi_ = id.iphi();

      for (int i = 0; i < static_cast<int>(energy_depth.size()); ++i) { // i=2 to exclude depth layer 0
        int depth_energy = energy_depth[i];
        depth_tdc_rise = digi.SOI_rising_avg(i+1);

	if(abs(tp_ieta_) >= 16 && i<2) continue; // exclude depth 1 for HE, but keep it for HB
        if(abs(tp_ieta_) <= 20 && depth_tdc_rise >= 3 && depth_energy >= 50) N_cell++;
        if(abs(tp_ieta_) >= 21 && depth_tdc_rise >= 3 && depth_energy >= 105) N_cell++;
      }

      return N_cell;
}
