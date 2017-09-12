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

template<typename T, typename U>
bool CanTypeFitValue(const U value)
{
  const intmax_t botT = intmax_t(std::numeric_limits<T>::min());
  const intmax_t botU = intmax_t(std::numeric_limits<U>::min());
  const uintmax_t topT = uintmax_t(std::numeric_limits<T>::max());
  const uintmax_t topU = uintmax_t(std::numeric_limits<U>::max());
  return !((botT > botU && value < static_cast<U> (botT)) || (topT < topU && value > static_cast<U> (topT)));
}

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

    bool extend(const Interval& i);

    bool operator==(const Interval& rhs) const
    {
      return mBegin == rhs.mBegin &&
             mEnd == rhs.mEnd;
    }

    bool operator!=(const Interval& rhs) const
    {
      return !(rhs == *this);
    }

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

    void contribution(Interval i, std::vector<Interval>& edgeStack);

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

Node* createSegmentTree(std::vector<double> values);

Node* buildNode(Interval i);

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
