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


#ifndef O2_MCH_CONTOUR_BBOX_H
#define O2_MCH_CONTOUR_BBOX_H

#include "vertex.h"
#include <stdexcept>
#include <ostream>
#include <limits>
#include <vector>

namespace o2 {
namespace mch {
namespace contour {

template<typename T>
class BBox
{
  public:
    BBox(T xmin, T xmax, T ymin, T ymax) : mTopLeft{xmin, ymax}, mBottomRight{xmax, ymin}
    {
      if (xmin >= xmax || ymin >= ymax) {
        throw std::invalid_argument("BBox should be created valid (xmin<xmax and ymin<ymax)");
      }
    }


    T xmin() const
    { return mTopLeft.x; }

    T xmax() const
    { return mBottomRight.x; }

    T ymin() const
    { return mBottomRight.y; }

    T ymax() const
    { return mTopLeft.y; }

    T width() const { return xmax() - xmin(); }

    T height() const { return ymax() - ymin(); }

    friend bool operator==(const BBox &lhs, const BBox &rhs)
    {
      return lhs.mTopLeft == rhs.mTopLeft &&
             lhs.mBottomRight == rhs.mBottomRight;
    }

    friend bool operator!=(const BBox &lhs, const BBox &rhs)
    {
      return !(rhs == lhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const BBox &box)
    {
      os << "mTopLeft: " << box.mTopLeft << " mBottomRight: " << box.mBottomRight;
      return os;
    }

  private:
    Vertex <T> mTopLeft;
    Vertex <T> mBottomRight;
};

template<typename T>
BBox<T> getBBox(const std::vector<Vertex < T>>

& vertices) {

T xmin{std::numeric_limits<T>::max()};
T xmax{std::numeric_limits<T>::min()};
T ymin{std::numeric_limits<T>::max()};
T ymax{std::numeric_limits<T>::min()};

for (
const auto &v :
vertices ) {
xmin = std::min(xmin, v.x);
xmax = std::max(xmax, v.x);
ymin = std::min(ymin, v.y);
ymax = std::max(ymax, v.y);
}
return {
xmin,xmax,ymin,ymax
};
}

template<typename T>
Vertex<T> getCenter(const BBox <T> &bbox)
{
  return {
    (bbox.xmax() + bbox.xmin()) / T{2},
    (bbox.ymax() + bbox.ymin()) / T{2}
  };
}

}
}
}

#endif
