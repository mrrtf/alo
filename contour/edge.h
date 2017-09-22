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


#ifndef O2_MCH_CONTOUR_EDGE_H
#define O2_MCH_CONTOUR_EDGE_H

#include <utility>
#include <vector>
#include <iostream>
#include <iomanip>

#include "interval.h"

namespace o2 {
namespace mch {
namespace contour {

template<bool vertical>
class ManhattanEdge
{
  public:

    ManhattanEdge(int coordinate = {}, int b = {}, int e = 1) : mCoordinate(coordinate),
                                                                mInterval{b > e ? e : b, b > e ? b : e},
                                                                mIsDirect(b > e)
    {
      if (b < 0 || e < 0) { throw std::out_of_range("b,e are supposed to be indices, i.e. >=0"); }
    }

    int coordinate() const
    { return mCoordinate; }

    Interval<int> interval() const
    { return mInterval; }

    bool isDirect() const
    { return mIsDirect; }

    bool isVertical() const
    { return vertical; }

    bool isHorizontal() const
    { return !vertical; }

  private:
    int mCoordinate;
    Interval<int> mInterval;
    bool mIsDirect;

};

using VerticalEdge= ManhattanEdge<true>;
using HorizontalEdge = ManhattanEdge<false>;

inline bool isLeftEdge(const VerticalEdge& edge)
{ return edge.isDirect(); }

inline bool isRightEdge(const VerticalEdge& edge)
{ return !isLeftEdge(edge); }

inline bool isTopToBottom(const VerticalEdge& edge)
{
  return isLeftEdge(edge);
}

inline bool isBottomToTop(const VerticalEdge& edge)
{
  return !isTopToBottom(edge);
}

inline bool isLeftToRight(const HorizontalEdge& edge)
{ return !edge.isDirect(); }

inline bool isRightToLeft(const HorizontalEdge& edge)
{ return !isLeftToRight(edge); }

template<bool vertical>
int begin(const ManhattanEdge<vertical>& edge)
{
  return edge.isDirect() ? edge.interval().end() : edge.interval().begin();
}

template<bool vertical>
int end(const ManhattanEdge<vertical>& edge)
{
  return edge.isDirect() ? edge.interval().begin() : edge.interval().end();
}

inline int top(const VerticalEdge& edge)
{ return std::max(edge.interval().begin(), edge.interval().end()); }

inline int bottom(const VerticalEdge& edge)
{ return std::min(edge.interval().begin(), edge.interval().end()); }

template<bool vertical>
void outputXY(std::ostream& os, const ManhattanEdge<vertical>& edge) {
  os << std::setw(3) << edge.coordinate() << " interval: [";
  os << std::setw(3) << begin(edge) << "," << std::setw(3) << end(edge);
  os << "] ";
}

inline std::ostream& operator<<(std::ostream& os, const VerticalEdge& edge)
{
  os << "abscissa: ";
  outputXY(os,edge);
  os << (isTopToBottom(edge) ? 'v' : '^');
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const HorizontalEdge& edge)
{
  os << "ordinate: ";
  outputXY(os,edge);
  os << (isLeftToRight(edge) ? "->-" : "-<-");
  return os;
}

template<bool vertical>
bool operator==(const ManhattanEdge<vertical>& lhs, const ManhattanEdge<vertical>& rhs)
{
  return lhs.coordinate() == rhs.coordinate() &&
         lhs.interval() == rhs.interval() &&
         lhs.isDirect() == rhs.isDirect();
}

template<bool vertical>
bool operator!=(const ManhattanEdge<vertical>&& lhs, const ManhattanEdge<vertical>& rhs)
{
  return !(rhs == lhs);
}

}
}
}

#endif
