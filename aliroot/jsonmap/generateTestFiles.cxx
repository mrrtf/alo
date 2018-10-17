// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// @author  Laurent Aphecetche

#include "generateTestFiles.h"

#include "AliMpDDLStore.h"
#include "AliMpSegmentation.h"
#include "AliMpVSegmentation.h"
#include <vector>
#include <AliMpMotifPosition.h>
#include "de.h"
#include "json.h"
#include "motifPosition.h"
#include "seg.h"
#include "AliMpMotifType.h"
#include "AliMpConnection.h"
#include <algorithm>
#include <random>

std::vector<int> get_channels(const AliMpMotifPosition& mp)
{
  std::vector<int> channels;
  AliMpMotifType *mt = mp.GetMotif()->GetMotifType();
  for (int i = 1; i <= 100; ++i) {
    AliMpConnection *c = mt->FindConnectionByPadNum(i);
    if (!c) continue;
    channels.push_back(c->GetManuChannel());
  }
  std::sort(channels.begin(), channels.end());
  return channels;
}

// generate file simply listing all possible valid (de,manu,channels) combinations
template<typename WRITER>
void generate_channel_list(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, WRITER& w) {

  std::vector<int> deids = get_deids(ddlStore);
  w.StartObject();
  w.Key("detection_elements");
  w.StartArray();
  for ( auto detElemId: deids ) {
    auto seg = mseg->GetMpSegmentation(detElemId,AliMp::kCath0,false);
    std::string segname = get_segtype(*seg);
    std::vector<AliMpMotifPosition*> mps = get_motifpositions(ddlStore,mseg,segname);
    std::sort(mps.begin(), mps.end(),
              [](AliMpMotifPosition* m1, AliMpMotifPosition* m2) { return m1->GetID() < m2->GetID(); });
    w.StartObject();
    w.Key("id");
    w.Int(detElemId);
    w.Key("manus");
    w.StartArray();
    for ( auto mp: mps) {
      w.StartObject();
      w.Key("id");
      w.Int(mp->GetID());
      auto channels = get_channels(*mp);
      if ( channels.size() != 64) {
        WriteArray(w, "channels", channels);
      }
      w.EndObject();
    }
    w.EndArray();
    w.EndObject();
  }
  w.EndArray();
  w.EndObject();
}

struct BBox {
    double xmin, ymin, xmax, ymax;
};

BBox max(const BBox& a, const BBox& b){

  auto xmin = std::min(a.xmin, b.xmin);
  auto ymin = std::min(a.ymin, b.ymin);
  auto xmax= std::min(a.xmax, b.xmax);
  auto ymax = std::min(a.ymax, b.ymax);

  return BBox{xmin,ymin,xmax,ymax};
}

BBox get_bbox(const AliMpVSegmentation& seg)
{
  auto xmin = seg.GetPositionX()-seg.GetDimensionX();
  auto ymin = seg.GetPositionY()-seg.GetDimensionY();
  auto xmax = seg.GetPositionX()+seg.GetDimensionX();
  auto ymax = seg.GetPositionY()+seg.GetDimensionY();
  return BBox{xmin,ymin,xmax,ymax};
}

std::ostream& operator<<(std::ostream& os, const BBox& box)
{
  os << "mTopLeft: " << box.xmin << "," << box.ymax << " mBottomRight: " << box.xmax << "," << box.ymin;
  return os;
}

struct Point {
    double x,y;
};

template<typename WRITER>
void generate_random_channel_positions_de(int detElemId, AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, int N, WRITER& w)
{

  auto bseg = get_segs(mseg, {detElemId}, AliMp::kBendingPlane).front();
  auto nbseg = get_segs(mseg, {detElemId}, AliMp::kNonBendingPlane).front();

  auto bbox = get_bbox(*bseg);
  auto nbbox = get_bbox(*bseg);

  auto box = max(bbox, nbbox);

  std::random_device rd;
  std::mt19937 mt(rd());
  std::vector<Point> testPoints(N);

  double offset{0.1};

  std::uniform_real_distribution<double> distX{box.xmin-offset, box.xmax+offset};
  std::uniform_real_distribution<double> distY{box.ymin-offset, box.ymax+offset};
  std::uniform_int_distribution<int> zeroOrOne{0,1};
  std::generate(testPoints.begin(), testPoints.end(), [&distX, &distY, &mt] { return Point{distX(mt), distY(mt)}; });

  for (auto p : testPoints) {
    w.StartObject();
    w.Key("de");
    w.Int(detElemId);
    w.Key("bending");
    bool isBendingPlane = (zeroOrOne(mt)==0);
    w.String( isBendingPlane ? "true":"false");
    AliMpVSegmentation* seg = isBendingPlane ? bseg : nbseg;
    
    w.Key("x");
    w.Double(p.x);
    w.Key("y");
    w.Double(p.y);
    AliMpPad pad = seg->PadByPosition(p.x,p.y,false);
    if (!pad.IsValid()) {
      w.Key("isoutside");
      w.String("true");
    }
    else {
      w.Key("dsid");
      w.Int(pad.GetManuId());
      w.Key("dsch");
      w.Int(pad.GetManuChannel());
      w.Key("px");
      w.Double(pad.GetPositionX());
      w.Key("py");
      w.Double(pad.GetPositionY());
      w.Key("sx");
      w.Double(pad.GetDimensionX()*2.0);
      w.Key("sy");
      w.Double(pad.GetDimensionY()*2.0);
    }
    w.EndObject();
  }
}

// generate some random (valid or invalid) pad positions
template<typename WRITER>
void generate_random_channel_positions(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, int N, WRITER& w) {
  for (auto detElemId : get_deids(ddlStore)) {
    generate_random_channel_positions_de(detElemId,ddlStore,mseg,N,w);
  }
}

void generate_test_files(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg, int nposperde)
{
  //generate_channel_list(ddlStore, mseg, OF("test_channel_list.json").Writer());
  OF of("test_random_pos.json");
  auto& w = of.Writer();
  w.StartObject();
  w.Key("testpositions");
  w.StartArray();
  generate_random_channel_positions(ddlStore, mseg, nposperde, w);
  w.EndArray();
  w.EndObject();
}
