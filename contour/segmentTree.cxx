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

#include "segmentTree.h"

#include "cantypefitvalue.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace o2 {
namespace mch {
namespace contour {

Node::Node(Interval<int> i) : mInterval(i),
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


void Node::contribution(Interval<int> i, std::vector<Interval<int>>& edgeStack)
{
  /// Contribution of an edge (b,e) to the final contour
  if (cardinality()) {
    return;
  }
  if (mInterval.isFullyContainedIn(i) && !isPotent()) {
    if (edgeStack.empty()) {
      edgeStack.push_back(mInterval);
    } else {
      auto& back = edgeStack.back();
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

void Node::insertInterval(Interval<int> i)
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

void Node::deleteInterval(Interval<int> i)
{
  if (mInterval.isFullyContainedIn(i)) {
    --mCardinality;
  } else {
    if (cardinality() > 0) {
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
    if (mLeftChild->cardinality() > 0 && mRightChild->cardinality() > 0) {
      promote();
    }
    potent(!(!isActive(*mLeftChild) && !isActive(*mRightChild)));
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

Node* createSegmentTree(std::vector<double> values)
{
  if (values.size() < 2) { throw std::invalid_argument("must get at least two values"); }

  std::sort(values.begin(), values.end());

  if (!CanTypeFitValue<int>(values.size() - 1)) {
    throw std::runtime_error("too many values");
  }

  return buildNode(Interval<int>{0, static_cast<int>(values.size() - 1)});
}

Node* buildNode(Interval<int> i)
{
  auto* node = new Node(i);
  if (isElementary(i)) { return node; }
  node->setLeft(buildNode(leftPart(i))).setRight(buildNode(rightPart(i)));
  return node;
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

}
}
}
