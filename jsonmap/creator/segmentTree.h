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

class Node
{
// TODO : include Node within SegmentTree as we don't need it elsewhere
  public:
    Node() = delete;

    ~Node();

    Node(int begin, int end);

    friend std::ostream& operator<<(std::ostream& os, const Node& node);

    bool isFullyContainedInInterval(int b, int e) const;

    int midpoint() const
    { return (mBegin + mEnd) / 2; }

    bool isActive()
    { return mCardinality > 0 || mPotent; }

    void insertInterval(int b, int e);

    void deleteInterval(int b, int e);

    void update();

    void promote();

    void demote();

    Node* mLeftChild;
    Node* mRightChild;

    int mBegin;
    int mEnd;
    int mCardinality; // cardinality
    bool mPotent; // potent state

};

class SegmentTree
{

  public:
    SegmentTree() = delete;

    SegmentTree(const std::vector<double>& values);

    Node* build(int b, int e);

    friend std::ostream& operator<<(std::ostream& os, const SegmentTree& tree);

    void insertInterval(int b, int e)
    { mRoot->insertInterval(b, e); }

    void deleteInterval(int b, int e)
    { mRoot->deleteInterval(b, e); }

  private:
    std::vector<double> mValues; // sorted values
    Node* mRoot; // top node
    std::vector<Node> mStack; // stack of nodes
};

}
}
}

#endif
