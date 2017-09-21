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


#ifndef O2_MCH_CONTOUR_INTERVAL_H
#define O2_MCH_CONTOUR_INTERVAL_H

#include <vector>
#include <ostream>

namespace o2 {
namespace mch {
namespace contour {

class Interval
{
  public:

    Interval(int b = {}, int e = {}) : mBegin(b), mEnd(e)
    {
      if (b >= e) { throw std::invalid_argument("begin should be strictly < end"); }
    }

    bool isFullyContainedIn(Interval i) const
    {
      // returns true if this node's interval is fully contained within interval [b,e]
      return i.begin() <= mBegin && mEnd <= i.end();
    }

    int begin() const
    { return mBegin; }

    int end() const
    { return mEnd; }

    int midpoint() const
    { return (mBegin + mEnd) / 2; }

    bool extend(const Interval& i)
    {
      if (i.begin() == end()) {
        mEnd = i.end();
        return true;
      } else {
        return false;
      }
    }

    bool operator==(const Interval& rhs) const
    {
      return mBegin == rhs.mBegin &&
             mEnd == rhs.mEnd;
    }

    bool operator!=(const Interval& rhs) const
    {
      return !(rhs == *this);
    }

  private:
    int mBegin;
    int mEnd;
};

inline Interval leftPart(Interval i)
{ return {i.begin(), i.midpoint()}; }

inline Interval rightPart(Interval i)
{ return {i.midpoint(), i.end()}; }

inline bool isElementary(Interval i)
{ return i.end() - i.begin() == 1; }

inline std::ostream& operator<<(std::ostream& os, const Interval& i)
{
  os << "[" << i.begin() << "," << i.end() << "]";
  return os;
}

}
}
}

#endif
