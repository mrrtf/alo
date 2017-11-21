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


#ifndef O2_MCH_SVG_CONTOUR_H
#define O2_MCH_SVG_CONTOUR_H

#include "contour.h"
#include "bbox.h"
#include <fstream>

namespace o2 {
namespace mch {
namespace svg {

void writeContour(std::ofstream &out, int scale, const o2::mch::contour::Contour<double> &contour,
                  o2::mch::contour::BBox<double> &box, const char *style = "fill:none;stroke:black;stroke-width:0.5px");

void writeHeader(std::ofstream &out, o2::mch::contour::BBox<double> &box, int scale);

void writeContour(std::string filename, const o2::mch::contour::Contour<double> &contour, int scale);

}}}

#endif
