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


#ifndef O2_MCH_CONTOUR_SEGMENT_H
#define O2_MCH_CONTOUR_SEGMENT_H

#include <ostream>
#include "vertex.h"
#include "edge.h"

namespace o2 {
namespace mch {
namespace contour {

template<bool vertical>
Vertex<int> beginVertex(const ManhattanEdge<vertical>& edge)
{
  return vertical ? Vertex<int> {edge.coordinate(), begin(edge)} : Vertex<int> {begin(edge), edge.coordinate()};
}

template<bool vertical>
Vertex<int> endVertex(const ManhattanEdge<vertical>& edge)
{
  return vertical ? Vertex<int> {edge.coordinate(), end(edge)} : Vertex<int> {end(edge), edge.coordinate()};
}

class Segment
{
  public:
    template<bool vertical>
    Segment(const ManhattanEdge<vertical>& edge) : mBegin{beginVertex(edge)}, mEnd{endVertex(edge)}
    {}

    Vertex<int> begin() const
    { return mBegin; }

    Vertex<int> end() const
    { return mEnd; }

  private:
    Vertex<int> mBegin;
    Vertex<int> mEnd;
};

inline std::ostream& operator<<(std::ostream& os, const Segment& segment)
{
  os << "[" << segment.begin() << "," << segment.end() << "]";
  return os;
}


}
}
}

#endif
