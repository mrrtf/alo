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

template<typename T>
class Node
{
  public:
    Node() = delete;

    explicit Node(Interval <T> i);

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

    bool goLeft(const Interval <T>& i) const
    {
      /// Whether i.begin() strictly < midpoint()
      return isStrictlyBelow(i.begin(), midpoint());
    }

    bool goRight(const Interval <T>& i) const
    {
      /// Whether midpoint() is strictly < i.end()
      return isStrictlyBelow(midpoint(), i.end());
    }

    bool isPotent() const
    { return mIsPotent; }

    Node& potent(bool v)
    {
      mIsPotent = v;
      return *this;
    }

    T midpoint() const
    {
      return (interval().begin() + interval().end()) / 2;
    }

    Interval <T> interval() const
    { return mInterval; }

    void insertInterval(Interval <T> i);

    void deleteInterval(Interval <T> i);

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

    void contribution(Interval <T> i, std::vector<o2::mch::contour::Interval<T>>& edgeStack);

    void update();

    void promote();

    void demote();

  private:

    Node* mLeftChild;
    Node* mRightChild;

    Interval <T> mInterval;
    int mCardinality; // cardinality
    bool mIsPotent; // potent state
};

template<typename T>
Node<T>* buildNode(const std::vector<T>& values, int b, int e)
{
  Interval<T> i{values[b], values[e]};
  Node<T>* node = new Node<T>(i);
  if (e - b == 1) {
    return node;
  }
  int mid((b + e) / 2);
  node->setLeft(buildNode<T>(values, b, mid)).setRight(buildNode<T>(values, mid, e));
  return node;
}

template<typename T>
bool isActive(const Node<T>& node)
{ return node.cardinality() > 0 || node.isPotent(); }

template<typename T>
Node<T>* createSegmentTree(std::vector<T> values)
{
  if (values.size() < 2) { throw std::invalid_argument("must get at least two values"); }

  std::sort(values.begin(), values.end());

  return buildNode(values, 0, values.size() - 1);
}

template<typename T>
Node<T>::Node(Interval <T> i) : mInterval(i),
                                mCardinality(0),
                                mIsPotent(false),
                                mLeftChild(nullptr),
                                mRightChild(nullptr)
{}

template<typename T>
Node<T>::~Node()
{
  delete mLeftChild;
  delete mRightChild;
}

template<typename T>
void Node<T>::contribution(Interval <T> i, std::vector<o2::mch::contour::Interval<T>>& edgeStack)
{
/// Contribution of an edge (b,e) to the final contour
  if (cardinality()) {
    return;
  }
  if (interval().isFullyContainedIn(i) && !isPotent()) {
    if (edgeStack.empty()) {
      edgeStack.push_back(interval());
    } else {
      auto& back = edgeStack.back();
      if (!back.extend(interval())) {
// add a new segment if it can not be merged with current one
        edgeStack.push_back(interval());
      }
    }
  } else {
    if (goLeft(i)) {
      left()->contribution(i, edgeStack);
    }
    if (goRight(i) < i.end()) {
      right()->contribution(i, edgeStack);
    }
  }
}

template<typename T>
void dump(const char* msg, const Node<T>& node, const Interval <T>& i)
{
  std::cout << msg << "(" << i << ") into mInterval=" << node.interval() << " midpoint="
            << node.midpoint();
  std::cout << " isFullyContained=" << node.interval().isFullyContainedIn(i) << " cardinality="
            << node.cardinality()
            << (node.goLeft(i) ? " -L- " : " -R- ");

  if (areEqual(i.begin(), node.midpoint())) {
    std::cout << " WARNING BEGIN=MID";
  }
  if (areEqual(i.end(), node.midpoint())) {
    std::cout << " WARNING END=MID";
  }
}

template<typename T>
void Node<T>::insertInterval(Interval <T> i)
{
  if (interval().isFullyContainedIn(i)) {
    cardinality(1);
  } else {
    if (goLeft(i)) {
      left()->insertInterval(i);
    }
    if (goRight(i)) {
      right()->insertInterval(i);
    }
  }
  update();
}

template<typename T>
void Node<T>::deleteInterval(Interval <T> i)
{
  if (interval().isFullyContainedIn(i)) {
    cardinality(-1);
  } else {
    if (cardinality() > 0) {
      demote();
    }
    if (goLeft(i)) {
      left()->deleteInterval(i);
    }
    if (goRight(i)) {
      right()->deleteInterval(i);
    }
  }
  update();
}

template<typename T>
void Node<T>::update()
{
  if (left() == nullptr) {
    potent(false);
  } else {
    if (left()->cardinality() > 0 && right()->cardinality() > 0) {
      promote();
    }
    potent(!(!isActive(*left()) && !isActive(*right())));
  }
}

template<typename T>
void Node<T>::promote()
{
  left()->cardinality(-1);
  right()->cardinality(-1);
  cardinality(1);
}

template<typename T>
void Node<T>::demote()
{
  left()->cardinality(1);
  right()->cardinality(1);
  cardinality(-1);
  potent(true);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Node<T>& node)
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

#endif
