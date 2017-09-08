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
#include "contour.h"
#include <limits>
#include <stdint.h>

using std::numeric_limits;

template<typename T, typename U>
bool CanTypeFitValue(const U value)
{
  const intmax_t botT = intmax_t(numeric_limits<T>::min());
  const intmax_t botU = intmax_t(numeric_limits<U>::min());
  const uintmax_t topT = uintmax_t(numeric_limits<T>::max());
  const uintmax_t topU = uintmax_t(numeric_limits<U>::max());
  return !((botT > botU && value < static_cast<U> (botT)) || (topT < topU && value > static_cast<U> (topT)));
}


namespace o2 {
namespace mch {
namespace geometry {

Node::Node(Interval i) : mInterval(i),
                         mCardinality(0),
                         mIsPotent(false),
                         mLeftChild(nullptr),
                         mRightChild(nullptr)
{
}

Node::~Node()
{
  delete mLeftChild;
  delete mRightChild;

}

bool Interval::isFullyContainedIn(Interval i) const
{
  // returns true if this node's interval is fully contained within interval [b,e]
  return i.begin() <= mBegin && mEnd <= i.end();
}

void Node::contribution(Interval i, std::vector<Interval>& edgeStack)
{
  /// Contribution of an edge (b,e) to the final contour
  if (cardinality()) {
    return;
  }
  if (mInterval.isFullyContainedIn(i) && !isPotent()) {
    if (edgeStack.empty()) {
      edgeStack.push_back(mInterval);
    } else {
      Interval& back = edgeStack.back();
      if (!back.extend(mInterval)) {
        // add a new segment if it can not be merged with current one
        edgeStack.push_back(mInterval);
      }
    }
  } else {
    if (i.begin() < mInterval.midpoint()) {
      left()->contribution(i, edgeStack);
    }
    if (mInterval.midpoint() < i.end()) {
      right()->contribution(i, edgeStack);
    }
  }
}

void Node::insertInterval(Interval i)
{
  if (mInterval.isFullyContainedIn(i)) {
    ++mCardinality;
  } else {
    if (i.begin() < mInterval.midpoint()) {
      mLeftChild->insertInterval(i);
    }
    if (mInterval.midpoint() < i.end()) {
      mRightChild->insertInterval(i);
    }
  }
  update();
}

void Node::deleteInterval(Interval i)
{
  if (mInterval.isFullyContainedIn(i)) {
    --mCardinality;
  } else {
    if (mCardinality > 0) {
      demote();
    }
    if (i.begin() < mInterval.midpoint()) {
      mLeftChild->deleteInterval(i);
    }
    if (mInterval.midpoint() < i.end()) {
      mRightChild->deleteInterval(i);
    }
  }
  update();
}

void Node::update()
{
  if (mLeftChild == nullptr) {
    potent(false);
  } else {
    if (mLeftChild->cardinality() > 0 && mRightChild > 0) {
      promote();
    }
    if (!isActive(*mLeftChild) && !isActive(*mRightChild)) {
      potent(false);
    } else {
      potent(true);
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
  potent(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SegmentTree::SegmentTree(const std::vector<double>& values) : mValues(values), mRoot(nullptr)
{
  if (values.size() < 2) { throw std::invalid_argument("must get at least two values"); }

  std::sort(mValues.begin(), mValues.end());

  if (!CanTypeFitValue<int>(mValues.size() - 1)) {
    throw std::runtime_error("too many values");
  }

  mRoot = build(Interval{0, static_cast<int>(mValues.size() - 1)});
}

Node* SegmentTree::build(Interval i)
{
  Node* node = new Node(i);
  if (isElementary(i)) { return node; }
  node->setLeft(build(leftPart(i))).setRight(build(rightPart(i)));
  return node;
}

bool Interval::extend(const Interval& i)
{
  if (i.begin()==end())
  {
    mEnd = i.end();
    return true;
  }
  else
  {
    return false;
  }
}

Interval leftPart(Interval i)
{ return {i.begin(), i.midpoint()}; }

Interval rightPart(Interval i)
{ return {i.midpoint(), i.end()}; }

bool isElementary(Interval i)
{ return i.end() - i.begin() == 1; }

std::ostream& operator<<(std::ostream& os, const Interval& i)
{
  os << "[" << i.begin() << "," << i.end() << "]";
  return os;
}

bool isActive(const Node& node)
{ return node.cardinality() > 0 || node.isPotent(); }

std::ostream& operator<<(std::ostream& os, const Node& node)
{
  auto w = os.width();
  os << node.interval();
  if (node.cardinality()) { os << " C=" << node.cardinality(); }
  if (node.isPotent()) { os << " potent"; }
  os << '\n';
  os.width(w + 6);
  if (node.left()) {
    os << (*node.left());
  }
  if (node.right()) {
    os << (*node.right());
  }
  os.width(w);
  return os;
}

std::ostream& operator<<(std::ostream& os, const SegmentTree& tree)
{
  if (tree.root()) {
    os << (*tree.root());
  } else {
    os << "Empty segment tree";
  }
  return os;
}


}
}
}
