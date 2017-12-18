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

#include "commonContour.h"

#include "AliMUONContour.h"
#include "AliMUONManuContourMaker.h"
#include "AliMUONPolygon.h"
#include "AliMpConnection.h"
#include "AliMpManuIterator.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpSegmentation.h"
#include "AliMpVMotif.h"
#include "AliMpVSegmentation.h"
#include "TObjArray.h"
#include "contourCreator.h"

using namespace o2::mch::contour;

o2::mch::contour::Polygon<double> convert(const AliMUONPolygon &polygon)
{
  std::vector<Vertex<double>> vertices;

  for (int i = 0; i < polygon.NumberOfVertices(); ++i) {
    vertices.push_back({polygon.X(i), polygon.Y(i)});
  }

  return
    o2::mch::contour::Polygon<double>{vertices.begin(), vertices.end()};
}


o2::mch::contour::Contour<double> convert(const TObjArray &polygons)
{
  TIter next(&polygons);
  AliMUONPolygon *polygon;

  o2::mch::contour::Contour<double> pc;

  while ((polygon = static_cast<AliMUONPolygon *>(next()))) {
    if (polygon->NumberOfVertices() > 2) {
      pc.addPolygon(convert(*polygon));
    };
  }
  return pc;
}


Contour<double> createO2Contour(AliMpSegmentation *mseg, int detElemId, int manuId)
{
  auto pads = createManuPads(mseg, detElemId, manuId);
  return createContour(pads);
}

std::pair<Contour<double>, Contour<double>> createContours(AliMpSegmentation *mseg, int detElemId, int manuId)
{
  Contour<double> fromAliRoot = createAliRootContour(detElemId, manuId);
  Contour<double> fromO2 = createO2Contour(mseg, detElemId, manuId);
  return {fromAliRoot, fromO2};
}

o2::mch::contour::Contour<double> createAliRootContour(int detElemId, int manuId)
{
  AliMUONManuContourMaker contourMaker(nullptr);
  std::unique_ptr<AliMUONContour> contour(contourMaker.CreateManuContour(detElemId, manuId));
  return convert(*(contour->Polygons()));
}

std::vector<std::vector<Polygon<double>>> getPads(AliMpSegmentation *mseg)
{
  AliMpManuIterator it;

  int detElemId, manuId;

  std::vector<std::vector<Polygon<double>>> pads;

  while (it.Next(detElemId, manuId)) {
    pads.push_back(createManuPads(mseg, detElemId, manuId));
  }
  return pads;

}

std::vector<Contour<double>> getContours(const std::vector<std::vector<Polygon<double>>> &pads)
{
  std::vector<Contour<double>> contours(pads.size());

  for (auto i = 0; i < pads.size(); ++i) {
    contours[i] = createContour(pads[i]);
  }
  return contours;
}

std::vector<Polygon<double>> createManuPads(AliMpSegmentation *mseg, int detElemId, int manuId)
{
  const AliMpVSegmentation *seg = mseg->GetMpSegmentationByElectronics(detElemId, manuId);
  AliMpMotifPosition *pos = seg->MotifPosition(manuId);
  AliMpVMotif *motif = pos->GetMotif();
  AliMpMotifType *motifType = motif->GetMotifType();

  std::vector<Polygon<double>> pads;

  auto xoffset = pos->GetPositionX() - seg->GetPositionX();
  auto yoffset = pos->GetPositionY() - seg->GetPositionY();

  if (motifType->IsFull()) {
    auto dx = motif->DimensionX();
    auto dy = motif->DimensionY();
    pads.push_back({{xoffset - dx, yoffset - dy},
                    {xoffset + dx, yoffset - dy},
                    {xoffset + dx, yoffset + dy},
                    {xoffset - dx, yoffset + dy},
                    {xoffset - dx, yoffset - dy}});

    return pads;
  }

  for (Int_t i = 0; i <= 64; ++i) {
    AliMpConnection *connection = motifType->FindConnectionByGassiNum(i);

    if (connection) {
      Int_t ix = connection->GetLocalIx();
      Int_t iy = connection->GetLocalIy();

      Double_t x, y, dx, dy;

      motif->GetPadDimensionsByIndices(ix, iy, dx, dy);
      motif->PadPositionLocal(ix, iy, x, y);

      x += xoffset;
      y += yoffset;

      Polygon<double> pad{
        {x - dx, y - dy},
        {x + dx, y - dy},
        {x + dx, y + dy},
        {x - dx, y + dy},
        {x - dx, y - dy}
      };

      pads.push_back(pad);
    }
  }
  return pads;
}

double area1(const Polygon<double> &p)
{
  double a{0.0};
  for (auto i = 0; i < p.size() - 1; ++i) {
    const auto &current = p[i];
    const auto &next = p[i + 1];
    a += current.x * next.y - current.y * next.x;
  }
  return a / 2.0;
}

double area1(const Contour<double> &c)
{
  double a{0.0};
  for (auto i = 0; i < c.size(); ++i) {
    a += area1(c[i]);
  }
  return a;
}

double area2(const Polygon<double> &p)
{
  const auto& vertices = getVertices(p);
  double a{0.0};
  for (auto i = 0; i < vertices.size() - 1; ++i) {
    const auto &current = vertices[i];
    const auto &next = vertices[i + 1];
    a += (current.x + next.x) * (next.y - current.y);
  }
  return a / 2.0;
}

double area2(const Contour<double> &c)
{
  double a{0.0};
  for (auto i = 0; i < c.size(); ++i) {
    a += area2(c[i]);
  }
  return a;
}
