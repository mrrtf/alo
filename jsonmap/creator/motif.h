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


#ifndef ALO_JSONMAP_CREATOR_MOTIF_H
#define ALO_JSONMAP_CREATOR_MOTIF_H

#include <vector>
#include <string>

class PadSize;

class AliMpPCB;

class AliMpSector;

class AliMpVMotif;

std::vector<AliMpVMotif*> get_allslatmotifs(const std::vector<AliMpPCB*>& pcbs,
                                            const std::vector<PadSize>& padsizes);

std::vector<AliMpVMotif*>
get_allsectormotifs(const std::vector<const AliMpSector*>& sectors, const std::vector<PadSize>& padsizes);

std::vector<AliMpVMotif*>
get_allmotifs(const std::vector<AliMpPCB*>& pcbs, const std::vector<const AliMpSector*>& sectors,
              const std::vector<PadSize>& padsizes);

int get_padsize_index(float px, float py, const std::vector<PadSize>& padsizes);

std::string get_motif_id(const AliMpVMotif& motif, const std::vector<PadSize> & padsizes);

#endif
