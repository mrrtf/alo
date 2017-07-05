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


#ifndef ALO_JSONMAP_READER_MOTIFTYPE_H
#define ALO_JSONMAP_READER_MOTIFTYPE_H

#include <string>
#include <utility>

std::pair<std::string,std::string> readMotifTypes(const std::string& inputFileName);

#endif // ALO_JSONMAP_READER_MOTIFTYPE_H
