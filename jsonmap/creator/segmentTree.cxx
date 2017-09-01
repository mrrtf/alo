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


#include <stdexcept>
#include <algorithm>
#include "segmentTree.h"

namespace o2 {
namespace mch {
namespace geometry {

Node::Node(int b, int e) : mBegin(b), mEnd(e), mCardinality(0), mPotent(false), mLeftChild(nullptr), mRightChild(nullptr)
{
  if (b > e) { throw std::invalid_argument("begin should be < end"); }
}

Node::~Node()
{
  delete mLeftChild;
  delete mRightChild;

}
bool Node::isFullyContainedInInterval(int b, int e) const
{
  // returns true if this node's interval is fully contained within interval [b,e]
  return b <= mBegin && mEnd <= e;
}

void Node::insertInterval(int b, int e)
{
  if (isFullyContainedInInterval(b, e)) {
    ++mCardinality;
  } else {
    if (b < midpoint()) {
      mLeftChild->insertInterval(b, e);
    }
    if (midpoint() < e) {
      mRightChild->insertInterval(b, e);
    }
  }
  update();
}

void Node::deleteInterval(int b, int e)
{
  throw;
}

void Node::update()
{
  if (mLeftChild==nullptr)
  {
    mPotent = false;
  }
  else
  {
    if (mLeftChild->mCardinality > 0 && mRightChild > 0 ) {
      promote();
    }
    if ( !mLeftChild->isActive() && !mRightChild->isActive() )
    {
      mPotent = false;
    }
    else
    {
      mPotent = true;
    }
  }
}

void Node::promote()
{
  --mLeftChild->mCardinality;
  --mRightChild->mCardinality;
  ++mCardinality;
}

void Node::demote()
{
  ++mLeftChild->mCardinality;
  ++mRightChild->mCardinality;
  --mCardinality;
  mPotent = true;
}

std::ostream& operator<<(std::ostream& os, const Node& node)
{
  auto w = os.width();
  os << "[" << node.mBegin << "," << node.mEnd << "]";
  if (node.mCardinality) { os << " C=" << node.mCardinality; }
  if (node.mPotent) { os << " P=" << node.mPotent; }
  os << '\n';
  os.width(w + 6);
  if (node.mLeftChild) {
    os << (*node.mLeftChild);
  }
  if (node.mRightChild) {
    os << (*node.mRightChild);
  }
  os.width(w);
  return os;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SegmentTree::SegmentTree(const std::vector<double>& values) : mValues(values), mRoot(nullptr)
{
  if (values.size() < 2) { throw std::invalid_argument("must get at least two values"); }

  std::sort(mValues.begin(), mValues.end());

  mRoot = build(0, mValues.size() - 1);
}

Node* SegmentTree::build(int b, int e)
{
  Node* node = new Node(b, e);
  if (e - b == 1) { return node; }
  node->mLeftChild = build(b, (b + e) / 2);
  node->mRightChild = build((b + e) / 2, e);
  return node;
}

std::ostream& operator<<(std::ostream& os, const SegmentTree& tree)
{
  if (tree.mRoot) {
    os << (*tree.mRoot);
  } else {
    os << "Empty segment tree";
  }
  return os;
}

}
}
}
