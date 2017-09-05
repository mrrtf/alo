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


#ifndef O2_MCH_GEOMETRY_NODE_H
#define O2_MCH_GEOMETRY_NODE_H

#include <vector>
#include <ostream>
#include <boost/format.hpp>

namespace o2 {
namespace mch {
namespace geometry {

class Interval
{
  public:
    Interval(int b, int e) : mBegin(b), mEnd(e)
    {
      if (b > e) { throw std::invalid_argument("begin should be < end"); }
    }

    bool isFullyContainedIn(Interval i) const;

    int begin() const
    { return mBegin; }

    int end() const
    { return mEnd; }

    int midpoint() const
    { return (mBegin + mEnd) / 2; }

    //void merge(int b, int e )

    friend std::ostream& operator<<(std::ostream& os, const Interval& i)
    {
      os << "[" << i.mBegin << "," << i.mEnd << "]";
      return os;
    }

    bool isElementary() const
    { return mEnd - mBegin == 1; }

    Interval left() const
    { return Interval{begin(), midpoint()}; }

    Interval right() const
    { return Interval{midpoint(), end()}; }

  private:
    int mBegin;
    int mEnd;
};

class Node
{
// TODO : include Node within SegmentTree as we don't need it elsewhere
  public:
    Node() = delete;

    Node(Interval i);

    ~Node();

    friend std::ostream& operator<<(std::ostream& os, const Node& node);

    bool isActive()
    { return mCardinality > 0 || mPotent; }

    void insertInterval(Interval i);

    void deleteInterval(Interval i);

    //void contribution(Interval i, std::vector<Interval>& edgeStack);

    void update();

    void promote();

    void demote();

    Node* mLeftChild;
    Node* mRightChild;

    Interval mInterval;
    int mCardinality; // cardinality
    bool mPotent; // potent state

};

class SegmentTree
{

  public:
    SegmentTree() = delete;

    SegmentTree(const std::vector<double>& values);

    Node* build(Interval i);

    friend std::ostream& operator<<(std::ostream& os, const SegmentTree& tree);

    void insertInterval(Interval i)
    { mRoot->insertInterval(i); }

    void deleteInterval(Interval i)
    { mRoot->deleteInterval(i); }

  private:
    std::vector<double> mValues; // sorted values
    Node* mRoot; // top node
    std::vector<Node> mStack; // stack of nodes
};

}
}
}

#endif
