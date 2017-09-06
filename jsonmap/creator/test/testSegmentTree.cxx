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

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <iostream>
#include "segmentTree.h"

using namespace o2::mch::geometry;

struct YPOS
{

    YPOS()
    {

      Node* left = new Node{Interval{0,4}};
      Node* right = new Node{Interval{4,8}};

      testNode.setLeft(left).setRight(right);

      left->cardinality(dummyCardinality);
      right->cardinality(dummyCardinality);

    }

    std::vector<double> ypos{0, 1, 2, 3, 4, 5, 6, 7, 8};
    Node node{Interval{0, 8}};
    Node testNode{Interval{0, 8}};
    int dummyCardinality{3};

};

BOOST_AUTO_TEST_SUITE(o2_mch_geometry)

BOOST_FIXTURE_TEST_SUITE(segment_tree, YPOS)

BOOST_AUTO_TEST_CASE(IntervalCtorThrowsIfBeginIsAfterEnd)
{
  BOOST_CHECK_THROW(Interval a(24, 3), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(IntervalCtorThrowsIfBeginEqualsEnd)
{
  BOOST_CHECK_THROW(Interval a(24, 24), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(NeedAtLeastTwoValuesToBuildASegmentTree)
{
  BOOST_CHECK_THROW(SegmentTree t({1}), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(IntervalIsFullyContainedInInterval)
{
  Interval i{1, 5};
  BOOST_CHECK_EQUAL(Interval(0, 4).isFullyContainedIn(i), false);
  BOOST_CHECK_EQUAL(Interval(1, 2).isFullyContainedIn(i), true);
}

BOOST_AUTO_TEST_CASE(NodeInsertAndDelete)
{
  SegmentTree t(ypos);

  t.insertInterval(Interval{1, 5});
  t.insertInterval(Interval{5, 8});

  std::cout << t << '\n';

  t.deleteInterval(Interval{6, 7});

  std::cout << t << '\n';
}

BOOST_AUTO_TEST_CASE(JustCreatedNodeIsNotPotent)
{
  BOOST_CHECK_EQUAL(node.isPotent(), false);
}

BOOST_AUTO_TEST_CASE(JustCreatedNodeHasCardinalityEqualsZero)
{
  BOOST_CHECK_EQUAL(node.cardinality(), 0);
}

BOOST_AUTO_TEST_CASE(PromoteNode)
{
  testNode.promote();

  BOOST_CHECK_EQUAL(testNode.cardinality(), 1);
  BOOST_CHECK_EQUAL(testNode.left()->cardinality(), dummyCardinality - 1);
  BOOST_CHECK_EQUAL(testNode.right()->cardinality(), dummyCardinality - 1);
}

BOOST_AUTO_TEST_CASE(DemoteNode)
{
  testNode.promote();
  testNode.demote();
  BOOST_CHECK_EQUAL(testNode.cardinality(), 0);
  BOOST_CHECK_EQUAL(testNode.left()->cardinality(), dummyCardinality);
  BOOST_CHECK_EQUAL(testNode.right()->cardinality(), dummyCardinality);
  BOOST_CHECK_EQUAL(testNode.isPotent(), true);
}

//BOOST_AUTO_TEST_CASE(BuildSegmentTree)
//{
//  SegmentTree t{ypos};
//  std::cout << t << std::endl;
//  BOOST_CHECK(true);
//}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


