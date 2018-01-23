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

#include "helper.h"
#include <stdexcept>
#include <ostream>

namespace o2 {
namespace mch {
namespace contour {

template<typename T>
class BBox
{
  public:
    BBox(T xmin, T xmax, T ymin, T ymax) : mXmin{xmin}, mXmax{xmax}, mYmin{ymin}, mYmax{ymax}
    {
      if (xmin >= xmax || ymin >= ymax) {
        throw std::invalid_argument("BBox should be created valid (xmin<xmax and ymin<ymax)");
      }
    }

    T xmin() const
    { return mXmin; }

    T xmax() const
    { return mXmax; }

    T ymin() const
    { return mYmin; }

    T ymax() const
    { return mYmax; }

    T xcenter() const {
      return (xmin()+xmax())/T{2};
    }

    T ycenter() const {
      return (ymin()+ymax())/T{2};
    }

    T width() const
    { return xmax() - xmin(); }

    T height() const
    { return ymax() - ymin(); }

    friend bool operator==(const BBox &lhs, const BBox &rhs)
    {
      return impl::areEqual(lhs.xmin(),rhs.xmin()) &&
        impl::areEqual(lhs.ymin(),rhs.ymin()) &&
        impl::areEqual(lhs.xmax(),rhs.xmax()) &&
        impl::areEqual(lhs.ymax(),rhs.ymax());
    }

    friend bool operator!=(const BBox &lhs, const BBox &rhs)
    {
      return !(rhs == lhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const BBox &box)
    {
      os << "mTopLeft: " << box.xmin() << "," << box.ymax() << " mBottomRight: " << box.xmax() << "," << box.ymin();
      return os;
    }

  private:
    T mXmin;
    T mXmax;
    T mYmin;
    T mYmax;
};

}
}
}

#endif
