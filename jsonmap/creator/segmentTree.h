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

    Interval(int b = {}, int e = {}) : mBegin(b), mEnd(e)
    {
      if (b >= e) { throw std::invalid_argument("begin should be strictly < end"); }
    }

    bool isFullyContainedIn(Interval i) const;

    int begin() const
    { return mBegin; }

    int end() const
    { return mEnd; }

    int midpoint() const
    { return (mBegin + mEnd) / 2; }

    //void merge(int b, int e )

  private:
    int mBegin;
    int mEnd;
};

class Node
{
  public:
    Node() = delete;

    explicit Node(Interval i);

    ~Node();

    Node* left() const
    { return mLeftChild; }

    Node* right() const
    { return mRightChild; }

    int cardinality() const
    { return mCardinality; }

    Node& cardinality(int c)
    {
      mCardinality += c;
      return *this;
    }

    bool isPotent() const
    { return mIsPotent; }

    Node& potent(bool v)
    {
      mIsPotent = v;
      return *this;
    }

    Interval interval() const
    { return mInterval; }

    void insertInterval(Interval i);

    void deleteInterval(Interval i);

    Node& setLeft(Node* left)
    {
      mLeftChild = left;
      return *this;
    }

    Node& setRight(Node* right)
    {
      mRightChild = right;
      return *this;
    }

    //void contribution(Interval i, std::vector<Interval>& edgeStack);

    void update();

    void promote();

    void demote();

  private:
    Node* mLeftChild;
    Node* mRightChild;

    Interval mInterval;
    int mCardinality; // cardinality
    bool mIsPotent; // potent state
};

class SegmentTree
{

  public:
    SegmentTree() = delete;

    SegmentTree(const std::vector<double>& values);

    Node* root() const
    { return mRoot; }

    void insertInterval(Interval i)
    { mRoot->insertInterval(i); }

    void deleteInterval(Interval i)
    { mRoot->deleteInterval(i); }

  private:
    Node* build(Interval i);

    std::vector<double> mValues; // sorted values
    Node* mRoot; // top node
    std::vector<Node> mStack; // stack of nodes
};

// helper functions for Interval, Node, and SegmentTree

std::ostream& operator<<(std::ostream& os, const SegmentTree& tree);

std::ostream& operator<<(std::ostream& os, const Node& node);

std::ostream& operator<<(std::ostream& os, const Interval& i);

Interval leftPart(Interval i);

Interval rightPart(Interval i);

bool isElementary(Interval i);

bool isActive(const Node& node);

}
}
}

#endif
