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
    std::vector<double> ypos{0, 1, 2, 3, 4, 5, 6, 7, 8};
    Node node{0, 8};
};

BOOST_AUTO_TEST_SUITE(o2_mch_geometry)

BOOST_FIXTURE_TEST_SUITE(segment_tree, YPOS)

BOOST_AUTO_TEST_CASE(NodeCtorThrowsIfBottomEndPointOfIntervalIsAboveEndPoint)
{
  BOOST_CHECK_THROW(Node a(24, 3), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(NeedAtLeastTwoValuesToBuildASegmentTree)
{
  BOOST_CHECK_THROW(SegmentTree t({1}), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(NodeIsFullyContainedInInterval)
{
  BOOST_CHECK_EQUAL(Node(0, 4).isFullyContainedInInterval(1, 5), false);
  BOOST_CHECK_EQUAL(Node(1, 2).isFullyContainedInInterval(1, 5), true);
}

//BOOST_AUTO_TEST_CASE(NodeInsert)
//{
//  SegmentTree t(ypos);
//
//  t.insertInterval(1, 5);
//  t.insertInterval(5, 8);
//
//  std::cout << t << '\n';
//}

BOOST_AUTO_TEST_CASE(JustCreatedNodeIsNotPotent)
{
  BOOST_CHECK_EQUAL(node.mPotent, false);
}

BOOST_AUTO_TEST_CASE(JustCreatedNodeHasCardinalityEqualsZero)
{
  BOOST_CHECK_EQUAL(node.mCardinality, 0);
}

BOOST_AUTO_TEST_CASE(PromoteNode)
{
  Node node{0, 8};
  int dummyCardinality{3};

  node.mLeftChild = new Node(0, 4);
  node.mRightChild = new Node(4, 8);

  node.mLeftChild->mCardinality = dummyCardinality;
  node.mLeftChild->mCardinality = dummyCardinality;

  node.promote();

  BOOST_CHECK_EQUAL(node.mCardinality, 1);
  BOOST_CHECK_EQUAL(node.mLeftChild->mCardinality, dummyCardinality - 1);
}

//BOOST_AUTO_TEST_CASE(BuildSegmentTree)
//{
//  SegmentTree t{ypos};
//  std::cout << t << std::endl;
//  BOOST_CHECK(true);
//}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


