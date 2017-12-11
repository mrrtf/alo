#include "segmentationImpl0.h"

#include <array>
#include <stdexcept>
#include <iterator>

namespace o2 {
namespace mch {
namespace mapping {

std::vector<int> getDetElemIds()
{
  return {
    100, 101, 102, 103, 200, 201, 202, 203, 300, 301, 302, 303, 400, 401, 402, 403, 500, 501, 502, 503, 504, 505, 506,
    507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611,
    612, 613, 614, 615, 616, 617, 700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 712, 713, 714, 715, 716,
    717, 718, 719, 720, 721, 722, 723, 724, 725, 800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813,
    814, 815, 816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 900, 901, 902, 903, 904, 905, 906, 907, 908, 909, 910,
    911, 912, 913, 914, 915, 916, 917, 918, 919, 920, 921, 922, 923, 924, 925, 1000, 1001, 1002, 1003, 1004, 1005, 1006,
    1007, 1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1024, 1025
  };
}

namespace {
std::array<int, 156> segtype{
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 3, 4, 5, 6, 6, 5, 4, 3, 7, 8, 9, 5, 6,
  6, 5, 9, 8, 7, 8, 9, 5, 6, 6, 5, 9, 8, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15,
  16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 16, 15,
  14, 13, 12, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17,
  11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11, 10, 11, 17, 18, 19, 20, 15, 15, 20, 19, 18, 17, 11
};

}

std::unique_ptr<SegmentationInterface> getSegmentationByType(int type, bool isBendingPlane)
{
  if (isBendingPlane == true && type == 0) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<0, true, 226, berg80ToManu, MotifPositionTwoPadSizes>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 0) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<0, false, 225, berg80ToManu, MotifPositionTwoPadSizes>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 1) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<1, true, 221, berg80ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 1) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<1, false, 222, berg80ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 2) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<2, true, 46, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 2) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<2, false, 32, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 3) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<3, true, 56, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 3) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<3, false, 39, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 4) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<4, true, 49, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 4) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<4, false, 34, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 5) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<5, true, 30, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 5) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<5, false, 21, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 6) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<6, true, 20, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 6) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<6, false, 14, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 7) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<7, true, 47, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 7) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<7, false, 33, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 8) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<8, true, 57, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 8) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<8, false, 40, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 9) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<9, true, 50, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 9) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<9, false, 35, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 10) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<10, true, 50, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 10) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<10, false, 36, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 11) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<11, true, 64, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 11) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<11, false, 46, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 12) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<12, true, 65, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 12) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<12, false, 46, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 13) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<13, true, 40, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 13) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<13, false, 29, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 14) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<14, true, 30, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 14) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<14, false, 22, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 15) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<15, true, 15, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 15) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<15, false, 12, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 16) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<16, true, 10, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 16) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<16, false, 8, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 17) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<17, true, 70, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 17) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<17, false, 50, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 18) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<18, true, 45, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 18) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<18, false, 33, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 19) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<19, true, 35, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 19) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<19, false, 26, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == true && type == 20) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<20, true, 20, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  if (isBendingPlane == false && type == 20) {
    return std::unique_ptr<SegmentationInterface>{
      new SegmentationImpl0<20, false, 16, berg100ToManu, MotifPosition>{arrayOfMotifTypes}};
  };
  return std::unique_ptr<SegmentationInterface>{
    new SegmentationImpl0<-1, true, 0, nullptr, MotifPosition>{arrayOfMotifTypes}};
}

} // namespace mapping
} // namespace mch
} // namespace o2
