//
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See https://alice-o2.web.cern.ch/ for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// @author  Laurent Aphecetche


#ifndef O2_MCH_MAPPING_ZONE_H
#define O2_MCH_MAPPING_ZONE_H

#include "contour.h"
#include <vector>
#include <ostream>

namespace o2 {
namespace mch {
namespace mapping {

template<typename MOTIFPOSITION>
struct Zone
{
    friend std::ostream &operator<<(std::ostream &os, const Zone &zone)
    {
      os << "envelop: " << zone.envelop << " # contours: " << zone.contours.size()
         << " # motifPositions: " << zone.motifPositions.size();
      return os;
    }

    bool contains(double x, double y) const {
      if (!envelop.contains(x,y))
      {
        return false;
      }
      for ( const auto& c: contours ) {
        if (c.contains(x,y)) {
          return true;
        }
      }
      return false;
    }

    o2::mch::contour::Contour<double> envelop;
    std::vector<o2::mch::contour::Contour<double>> contours;
    std::vector<MOTIFPOSITION> motifPositions;
};


}
}
}

#endif
