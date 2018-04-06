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
#include <vector>
#include <AliMpMotifPosition.h>
#include "de.h"
#include "json.h"
#include "motifPosition.h"
#include "seg.h"
#include "AliMpMotifType.h"
#include "AliMpConnection.h"
#include <algorithm>

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

void generate_test_files(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg)
{
  generate_channel_list(ddlStore, mseg, OF("test_de.json").Writer());
}
