#include "genSegmentationFactory.h"
#include "genSegmentationImpl0.h"
#include "genSegmentationImpl0Type0.h"
#include "genSegmentationImpl0Type1.h"
#include "genSegmentationImpl0Type2.h"
#include "genSegmentationImpl0Type3.h"
#include "genSegmentationImpl0Type4.h"
#include "genSegmentationImpl0Type5.h"
#include "genSegmentationImpl0Type6.h"
#include "genSegmentationImpl0Type7.h"
#include "genSegmentationImpl0Type8.h"
#include "genSegmentationImpl0Type9.h"
#include "genSegmentationImpl0Type10.h"
#include "genSegmentationImpl0Type11.h"
#include "genSegmentationImpl0Type12.h"
#include "genSegmentationImpl0Type13.h"
#include "genSegmentationImpl0Type14.h"
#include "genSegmentationImpl0Type15.h"
#include "genSegmentationImpl0Type16.h"
#include "genSegmentationImpl0Type17.h"
#include "genSegmentationImpl0Type18.h"
#include "genSegmentationImpl0Type19.h"
#include "genSegmentationImpl0Type20.h"

namespace o2
{
namespace mch
{
namespace mapping
{

  std::unique_ptr<SegmentationInterface> getSegmentation(int type, bool isBendingPlane) {
    if (isBendingPlane==true && type==0) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<0,true>{}};
    };
    if (isBendingPlane==false && type==0) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<0,false>{}};
    };
    if (isBendingPlane==true && type==1) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<1,true>{}};
    };
    if (isBendingPlane==false && type==1) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<1,false>{}};
    };
    if (isBendingPlane==true && type==2) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<2,true>{}};
    };
    if (isBendingPlane==false && type==2) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<2,false>{}};
    };
    if (isBendingPlane==true && type==3) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<3,true>{}};
    };
    if (isBendingPlane==false && type==3) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<3,false>{}};
    };
    if (isBendingPlane==true && type==4) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<4,true>{}};
    };
    if (isBendingPlane==false && type==4) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<4,false>{}};
    };
    if (isBendingPlane==true && type==5) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<5,true>{}};
    };
    if (isBendingPlane==false && type==5) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<5,false>{}};
    };
    if (isBendingPlane==true && type==6) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<6,true>{}};
    };
    if (isBendingPlane==false && type==6) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<6,false>{}};
    };
    if (isBendingPlane==true && type==7) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<7,true>{}};
    };
    if (isBendingPlane==false && type==7) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<7,false>{}};
    };
    if (isBendingPlane==true && type==8) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<8,true>{}};
    };
    if (isBendingPlane==false && type==8) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<8,false>{}};
    };
    if (isBendingPlane==true && type==9) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<9,true>{}};
    };
    if (isBendingPlane==false && type==9) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<9,false>{}};
    };
    if (isBendingPlane==true && type==10) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<10,true>{}};
    };
    if (isBendingPlane==false && type==10) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<10,false>{}};
    };
    if (isBendingPlane==true && type==11) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<11,true>{}};
    };
    if (isBendingPlane==false && type==11) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<11,false>{}};
    };
    if (isBendingPlane==true && type==12) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<12,true>{}};
    };
    if (isBendingPlane==false && type==12) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<12,false>{}};
    };
    if (isBendingPlane==true && type==13) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<13,true>{}};
    };
    if (isBendingPlane==false && type==13) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<13,false>{}};
    };
    if (isBendingPlane==true && type==14) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<14,true>{}};
    };
    if (isBendingPlane==false && type==14) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<14,false>{}};
    };
    if (isBendingPlane==true && type==15) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<15,true>{}};
    };
    if (isBendingPlane==false && type==15) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<15,false>{}};
    };
    if (isBendingPlane==true && type==16) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<16,true>{}};
    };
    if (isBendingPlane==false && type==16) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<16,false>{}};
    };
    if (isBendingPlane==true && type==17) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<17,true>{}};
    };
    if (isBendingPlane==false && type==17) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<17,false>{}};
    };
    if (isBendingPlane==true && type==18) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<18,true>{}};
    };
    if (isBendingPlane==false && type==18) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<18,false>{}};
    };
    if (isBendingPlane==true && type==19) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<19,true>{}};
    };
    if (isBendingPlane==false && type==19) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<19,false>{}};
    };
    if (isBendingPlane==true && type==20) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<20,true>{}};
    };
    if (isBendingPlane==false && type==20) {
      return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<20,false>{}};
    };
  return std::unique_ptr<SegmentationInterface>{new SegmentationImpl0<-1,true>{}};
}

} // namespace mapping
} // namespace mch
} // namespace o2
