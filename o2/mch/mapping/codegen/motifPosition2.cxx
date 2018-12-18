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

#include "motifPosition.h"
#include "writer.h"
#include "rapidjson/document.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <boost/format.hpp>

using namespace rapidjson;
namespace jsonmap
{
namespace codegen
{
namespace impl2
{

void generateCodeForMotifPositions(const Value& catsegs,
                                   const Value& motiftypes,
                                   const Value& padsizes,
                                   const Value& bergs)
{
}

} // namespace impl2
} // namespace codegen
} // namespace jsonmap
