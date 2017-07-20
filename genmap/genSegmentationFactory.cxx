#include "genSegmentationFactory.h"
#include "genSegmentationImpl0.h"

namespace o2
{
namespace mch
{
namespace mapping
{

  std::unique_ptr<SegmentationInterface> getSegmentation(int type, bool isBendingPlane) {
    if (isBendingPlane==true && type==0) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<0,true,226,berg80ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==0) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<0,false,225,berg80ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==1) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<1,true,221,berg80ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==1) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<1,false,222,berg80ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==2) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<2,true,46,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==2) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<2,false,32,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==3) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<3,true,56,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==3) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<3,false,39,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==4) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<4,true,49,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==4) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<4,false,34,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==5) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<5,true,30,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==5) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<5,false,21,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==6) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<6,true,20,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==6) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<6,false,14,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==7) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<7,true,47,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==7) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<7,false,33,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==8) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<8,true,57,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==8) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<8,false,40,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==9) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<9,true,50,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==9) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<9,false,35,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==10) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<10,true,50,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==10) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<10,false,36,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==11) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<11,true,64,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==11) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<11,false,46,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==12) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<12,true,65,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==12) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<12,false,46,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==13) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<13,true,40,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==13) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<13,false,29,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==14) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<14,true,30,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==14) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<14,false,22,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==15) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<15,true,15,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==15) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<15,false,12,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==16) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<16,true,10,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==16) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<16,false,8,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==17) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<17,true,70,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==17) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<17,false,50,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==18) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<18,true,45,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==18) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<18,false,33,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==19) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<19,true,35,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==19) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<19,false,26,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==true && type==20) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<20,true,20,berg100ToManu>{arrayOfMotifTypes}};
    };
    if (isBendingPlane==false && type==20) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<20,false,16,berg100ToManu>{arrayOfMotifTypes}};
    };
  return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<-1,true,0,nullptr>{arrayOfMotifTypes}};
}

} // namespace mapping
} // namespace mch
} // namespace o2
