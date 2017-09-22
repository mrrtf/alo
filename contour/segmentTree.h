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


#ifndef O2_MCH_CONTOUR_SEGMENTTREE_H
#define O2_MCH_CONTOUR_SEGMENTTREE_H

#include <vector>
#include <ostream>

#include "interval.h"

namespace o2 {
namespace mch {
namespace contour {


class Node
{
  public:
    Node() = delete;

    explicit Node(Interval<int> i);

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

    Interval<int> interval() const
    { return mInterval; }

    void insertInterval(Interval<int> i);

    void deleteInterval(Interval<int> i);

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

    void contribution(Interval<int> i, std::vector<Interval<int>>& edgeStack);

    void update();

    void promote();

    void demote();

  private:

    Node* mLeftChild;
    Node* mRightChild;

    Interval<int> mInterval;
    int mCardinality; // cardinality
    bool mIsPotent; // potent state
};

Node* createSegmentTree(std::vector<double> values);

Node* buildNode(Interval<int> i);

std::ostream& operator<<(std::ostream& os, const Node& node);

bool isActive(const Node& node);

}
}
}

#endif
