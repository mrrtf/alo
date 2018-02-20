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


#ifndef ALO_VSALIROOT_COMMONCONTOUR_H
#define ALO_VSALIROOT_COMMONCONTOUR_H

#include "Contour.h"
#include <utility>
#include <vector>

class TObjArray;

class AliMUONPolygon;

class AliMpSegmentation;

o2::mch::contour::Polygon<double> convert(const AliMUONPolygon &polygon);

o2::mch::contour::Contour<double> convert(const TObjArray &polygons);

o2::mch::contour::Contour<double> createAliRootContour(int detElemId, int manuId);

double area1(const o2::mch::contour::Polygon<double> &p);

double area1(const o2::mch::contour::Contour<double> &c);

double area2(const o2::mch::contour::Polygon<double> &p);

double area2(const o2::mch::contour::Contour<double> &c);

o2::mch::contour::Contour<double> createO2Contour(AliMpSegmentation *mseg, int detElemId, int manuId);

std::vector<std::vector<o2::mch::contour::Polygon<double>>> getPads(AliMpSegmentation *mseg);

std::pair<o2::mch::contour::Contour<double>, o2::mch::contour::Contour<double>>
createContours(AliMpSegmentation *mseg, int detElemId, int manuId);

std::vector<o2::mch::contour::Polygon<double>> createManuPads(AliMpSegmentation *mseg, int detElemId, int manuId);

std::vector<o2::mch::contour::Contour<double>> getContours(const std::vector<std::vector<o2::mch::contour::Polygon<double>>> &pads);

#endif

