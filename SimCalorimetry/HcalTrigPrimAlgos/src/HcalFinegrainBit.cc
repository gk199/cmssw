#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalFinegrainBit.h"

#include <cassert>

std::bitset<4> HcalFinegrainBit::compute(const HcalFinegrainBit::Tower& tower) const {
  if (version_ == 0) {
    std::bitset<4> result;

    // First layer consistent with a MIP
    result[0] = tower[is_mip][0];

    // First layer consistent with a MIP, at least one layer with more
    // than MIP energy deposition
    result[1] = result[0] & (tower[is_above_mip].count() > 0);

    // There layers consistent with a MIP
    result[2] = tower[is_mip].count() >= 3;

    // Unset
    result[3] = false;

    return result;
  }
  if (version_ == 1) {
    std::bitset<4> result;

    // All algorithms the same for testing purposes
    result[0] = result[1] = result[2] = result[3] = tower[is_mip][0];

    return result;
  }
  if (version_ == 2) {
    std::bitset<4> result;

    // All algorithms the same for testing purposes
    result[0] = result[1] = result[2] = result[3] = true;

    return result;
  }
  return 0;
}

// A very simple illustrative timing/depth bit computation
std::bitset<2> HcalFinegrainBit::compute(const HcalFinegrainBit::TowerTDC& tower) const {
   std::bitset<2> result;
  int timeflag = 0, depthflag = 0;
  for(size_t i=0; i<7; i++){
    if(tower[i].first>50 && tower[i].second>20) // simple algo for timing flag setting
      timeflag += 1;
    if(tower[i].first>50) // simple algo for depth flag setting
      depthflag +=1;
  }

  if(timeflag>4) 
    result[0]=true;
  else 
    result[0]=false;
  if(depthflag>0)
    result[1]=true;
  else
    result[1]=false;

  return result;
}

