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


#ifndef ALO_JSONMAP_CREATOR_MOTIFPOSITION_H
#define ALO_JSONMAP_CREATOR_MOTIFPOSITION_H

#include <string>
#include <vector>
#include <utility>

class AliMpMotifPosition;

class AliMpVSegmentation;

class AliMpDDLStore;

class AliMpSegmentation;

std::vector<AliMpMotifPosition*> get_motifpositions(const AliMpVSegmentation& seg);

std::vector<AliMpMotifPosition*> get_motifpositions(const AliMpVSegmentation& b, const AliMpVSegmentation& nb);

std::vector<AliMpMotifPosition*> get_motifpositions(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg,
                                                    std::string segname);

std::vector<std::pair<std::vector<AliMpMotifPosition*>, std::vector<AliMpMotifPosition*>>> get_motifpositions(
  AliMpDDLStore* ddlStore, AliMpSegmentation* mseg,
  const std::vector<std::string>& segnames);

#endif
