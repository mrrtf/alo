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


#ifndef O2_MCH_MAPPING_PAD_H
#define O2_MCH_MAPPING_PAD_H

#include <ostream>
#include <boost/format.hpp>

namespace o2 {
namespace mch {
namespace mapping {

struct Pad
{
    Pad(double x1 = 0, double y1 = 0, double x2 = 0, double y2 = 0) :
      xBottomLeft{x1}, yBottomLeft{y1},
      xTopRight{x2}, yTopRight{y2}
    {}

    bool isValid() const
    { return (xTopRight - xBottomLeft) > 0.1; }

    friend std::ostream &operator<<(std::ostream &os, const Pad &Pad)
    {
      if (Pad.isValid()) {
        os << boost::format("(%7.2f,%7.2f)->(%7.2f,%7.2f)") % Pad.xBottomLeft % Pad.yBottomLeft %
              Pad.xTopRight % Pad.yTopRight;
      } else {
        os << " ( not existing Pad )";
      }
      return os;
    }

    double positionX() const {
      return (xBottomLeft+xTopRight)/2.0;
    }

    double positionY() const {
      return (yBottomLeft+yTopRight)/2.0;
    }

    double dimensionX() const {
      return (xTopRight - xBottomLeft);
    }

    double dimensionY() const {
      return (yTopRight - yBottomLeft);
    }

    Pad translate(double x, double y)
    {
      return {xBottomLeft + x, yBottomLeft + y, xTopRight + x, yTopRight + y};
    }

    double xBottomLeft;
    double yBottomLeft;
    double xTopRight;
    double yTopRight;
};
}}}

#endif
