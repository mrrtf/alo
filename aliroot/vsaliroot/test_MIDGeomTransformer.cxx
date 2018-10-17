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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <boost/format.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/test_case.hpp>

#include "AliGeomManager.h"
#include "AliMUONGeometryTransformer.h"
#include "AliMpDDLStore.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "MIDBase/GeometryTransformer.h"
#include "MIDBase/LegacyUtility.h"
#include "MathUtils/Cartesian3D.h"
#include "TGeoManager.h"

#include <array>
#include <iostream>
#include <random>
#include <sstream>

struct GEOM {
  GEOM() {
    if (geoTrans.GetNofModuleTransformers() == 0) {
      if (!AliMpDDLStore::Instance()) {
        AliMpDataProcessor mp;
        AliMpDataMap *dataMap = mp.CreateDataMap("data");
        AliMpDataStreams dataStreams(dataMap);
        AliMpDDLStore::ReadData(dataStreams);
      }
      geoTrans.LoadGeometryData("transform.dat");
    }
  }

  static AliMUONGeometryTransformer geoTrans;
  static o2::mid::GeometryTransformer o2geoTrans;
};

AliMUONGeometryTransformer GEOM::geoTrans;
o2::mid::GeometryTransformer GEOM::o2geoTrans;

BOOST_AUTO_TEST_SUITE(o2_mid_geomTransformer)
BOOST_FIXTURE_TEST_SUITE(geo, GEOM)

std::vector<Point3D<double>> generatePoints(int ntimes) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> distX(-127.5, 127.5);
  std::uniform_real_distribution<double> distY(-40., 40.);

  std::vector<Point3D<double>> points;

  for (int itime = 0; itime < ntimes; ++itime) {
    points.emplace_back(Point3D<double>(distX(mt), distY(mt), 0.));
  }

  return points;
}

BOOST_TEST_DECORATOR(*boost::unit_test::tolerance(0.001))
BOOST_DATA_TEST_CASE(IsSameByPosition,
                     boost::unit_test::data::xrange(72) * generatePoints(1000),
                     deId, point) {
  Point3D<double> globalPoint =
      GEOM::o2geoTrans.localToGlobal(deId, point.x(), point.y());

  int detElemId = o2::mid::LegacyUtility::convertToLegacyDeId(deId);
  Double_t xg, yg, zg;
  GEOM::geoTrans.Local2Global(detElemId, point.x(), point.y(), point.z(), xg,
                              yg, zg);

  BOOST_TEST(globalPoint.x() == xg);
  BOOST_TEST(globalPoint.y() == yg);
  BOOST_TEST(globalPoint.z() == zg);

  Point3D<double> localPoint = GEOM::o2geoTrans.globalToLocal(deId, xg, yg, zg);
  BOOST_TEST(localPoint.x() == point.x());
  BOOST_TEST(localPoint.y() == point.y());
  BOOST_TEST(localPoint.z() == point.z());
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
