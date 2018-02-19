//
// This file has been generated. Do not modify it by hand or your changes might be lost.
//
// This implementation file cannot be used standalone, i.e. it is intended to be included
// into another implementation file.
//
#include <array>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace o2 {
namespace mch {
namespace mapping {
namespace impl1 {
std::array<int,156> detElemIndexToDetElemId{ 
100,101,102,103,200,201,202,203,300,301,302,303,400,401,402,403,500,501,502,503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,900,901,902,903,904,905,906,907,908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,1000,1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1023,1024,1025};

  std::array<int,156> detElemIndexToSegType{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,3,4,5,6,6,5,4,3,2,3,4,5,6,6,5,4,3,7,8,9,5,6,6,5,9,8,7,8,9,5,6,6,5,9,8,10,11,12,13,14,15,16,16,15,14,13,12,11,10,11,12,13,14,15,16,16,15,14,13,12,11,10,11,12,13,14,15,16,16,15,14,13,12,11,10,11,12,13,14,15,16,16,15,14,13,12,11,10,11,17,18,19,20,15,15,20,19,18,17,11,10,11,17,18,19,20,15,15,20,19,18,17,11,10,11,17,18,19,20,15,15,20,19,18,17,11,10,11,17,18,19,20,15,15,20,19,18,17,11};

  std::unique_ptr<SegmentationInterface> getSegmentationByType(int type, bool isBendingPlane) {
    if (isBendingPlane==true && type==0) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<0,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==0) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<0,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==1) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<1,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==1) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<1,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==2) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<2,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==2) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<2,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==3) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<3,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==3) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<3,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==4) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<4,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==4) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<4,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==5) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<5,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==5) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<5,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==6) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<6,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==6) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<6,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==7) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<7,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==7) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<7,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==8) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<8,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==8) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<8,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==9) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<9,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==9) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<9,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==10) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<10,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==10) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<10,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==11) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<11,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==11) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<11,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==12) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<12,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==12) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<12,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==13) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<13,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==13) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<13,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==14) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<14,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==14) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<14,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==15) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<15,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==15) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<15,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==16) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<16,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==16) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<16,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==17) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<17,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==17) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<17,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==18) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<18,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==18) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<18,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==19) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<19,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==19) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<19,false>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==20) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<20,true>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==20) {
      return std::unique_ptr<SegmentationInterface>{new Segmentation<20,false>{arrayOfMotifTypes}};
    };
  throw std::out_of_range(std::to_string(type) + " is not a valid segtype");
}

} // namespace impl1
} // namespace mapping
} // namespace mch
} // namespace o2
