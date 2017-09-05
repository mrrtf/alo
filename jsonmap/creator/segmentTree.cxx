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

template <typename T, typename U>
    bool CanTypeFitValue(const U value) {
        const intmax_t botT = intmax_t(numeric_limits<T>::min() );
        const intmax_t botU = intmax_t(numeric_limits<U>::min() );
        const uintmax_t topT = uintmax_t(numeric_limits<T>::max() );
        const uintmax_t topU = uintmax_t(numeric_limits<U>::max() );
        return !( (botT > botU && value < static_cast<U> (botT)) || (topT < topU && value > static_cast<U> (topT)) );
    }


namespace o2 {
namespace mch {
namespace geometry {

Node::Node(Interval i) : mInterval(i),
                           mCardinality(0),
                           mPotent(false),
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

//void Node::contribution(Interval i, std::vector<Interval>& edgeStack)
//{
//  /// Contribution of an edge (b,e) to the final contour
//  if ( mCardinality == 0 )
//  {
//    if ( isFullyContainedInInterval(b,e) && !mPotent )
//    {
//      if ( edgeStack.empty() ) {
//        edgeStack.push_back(std::make_pair<int,int>{mBegin,mEnd});
//      }
//      else
//      {
//
//      }
//      if ( back.second.y() == b )
//      {
//        // merge to existing segment
//        back.first.y()
//        Double_t y(back->StartY());
//        back->Set(0.0,y,0.0,fMax);
//      }
//      else
//      {
//        // add a new segment
//        stack.Add(new AliMUONSegment(0.0,fMin,0.0,fMax));
//      }
//    }
//    else
//    {
//      if ( b < fMidPoint )
//      {
//        fLeftNode->Contribution(b,e,stack);
//      }
//      if ( fMidPoint < e )
//      {
//        fRightNode->Contribution(b,e,stack);
//      }
//    }
//  }
//}

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
  os << node.mInterval;
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

  if (!CanTypeFitValue<int>(mValues.size()-1)) {
    throw std::runtime_error("too many values");
  }

  mRoot = build(Interval{0, static_cast<int>(mValues.size() - 1)});
}

Node* SegmentTree::build(Interval i)
{
  Node* node = new Node(i);
  if (i.isElementary()) { return node; }
  node->mLeftChild = build(i.left());
  node->mRightChild = build(i.right());
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
