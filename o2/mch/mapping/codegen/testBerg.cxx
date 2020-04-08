#define BOOST_TEST_MODULE mch berg test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <array>
#include <string>
#include <iostream>
#include "berg.h"
#include <iomanip>

BOOST_AUTO_TEST_SUITE(berg)

std::array<int, 64> refManu2ds_from_andrea = { 62, 61, 63, 60, 59, 55, 58, 57, 56, 54, 50, 46, 42, 39, 37, 41,
                                               35, 36, 33, 34, 32, 38, 43, 40, 45, 44, 47, 48, 49, 52, 51, 53,
                                               7, 6, 5, 4, 2, 3, 1, 0, 9, 11, 13, 15, 17, 19, 21, 23,
                                               31, 30, 29, 28, 27, 26, 25, 24, 22, 20, 18, 16, 14, 12, 10, 8 };

std::array<int, 64> refManu2ds_st345 = {
  63, 62, 61, 60, 59, 57, 56, 53, 51, 50, 47, 45, 44, 41, 38, 35,
  36, 33, 34, 37, 32, 39, 40, 42, 43, 46, 48, 49, 52, 54, 55, 58,
  7, 8, 5, 2, 6, 1, 3, 0, 4, 9, 10, 15, 17, 18, 22, 25,
  31, 30, 29, 28, 27, 26, 24, 23, 20, 21, 16, 19, 12, 14, 11, 13
};

std::array<int, 64> refManu2ds_st12 = {
  36, 35, 34, 33, 32, 37, 38, 43, 45, 47, 49, 50, 53, 41, 39, 40,
  63, 62, 61, 60, 59, 58, 56, 57, 54, 55, 52, 51, 48, 46, 44, 42,
  31, 30, 29, 28, 27, 26, 25, 24, 22, 23, 20, 18, 17, 15, 13, 11,
  4, 3, 2, 1, 0, 5, 6, 10, 12, 14, 16, 19, 21, 8, 7, 9
};

int countSameConversions(bool isSt345, std::array<int, 64> ref)
{
  std::array<int, 64> manu2ds = getManu2Ds("bergs-run2.json", "bergs-run3.json", isSt345);
  int nsame{ 0 };
  for (auto i = 0; i < manu2ds.size(); ++i) {
    bool same = (manu2ds[i] == ref[i]);
    std::cout << "MANU " << std::setw(2) << i << " got DS " << std::setw(2) << manu2ds[i]
              << " was expecting " << std::setw(2) << ref[i] << " " << (same ? "ok" : "** NOT OK **") << "\n";
    if (same) {
      ++nsame;
    }
  }
  return nsame;
}

BOOST_AUTO_TEST_CASE(checkBergConversionSt12)
{
  BOOST_CHECK_EQUAL(countSameConversions(false, refManu2ds_st12), 64);
}

BOOST_AUTO_TEST_CASE(checkBergConversionSt345)
{
  BOOST_CHECK_EQUAL(countSameConversions(true, refManu2ds_st345), 64);
}

BOOST_AUTO_TEST_CASE(checkBergConversionSt345_andrea)
{
  BOOST_CHECK_EQUAL(countSameConversions(true, refManu2ds_from_andrea), 64);
}

BOOST_AUTO_TEST_SUITE_END()
