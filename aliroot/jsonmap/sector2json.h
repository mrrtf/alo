#ifndef SECTOR2JSON_H
#define SECTOR2JSON_H

#include "AliMpMotifType.h"
#include "AliMpRow.h"
#include "AliMpSector.h"
#include "AliMpSubZone.h"
#include "AliMpVMotif.h"
#include "AliMpRowSegment.h"
#include "AliMpVRowSegment.h"
#include "AliMpZone.h"
#include "json.h"

bool isleftmost(const AliMpVRowSegment& rowsegment) 
{
    // tells if the row segment is non-special and is the left most in its row
    if (rowsegment.IsA() != AliMpRowSegment::Class()) return false;
    AliMpRow* row = rowsegment.GetRow();
    for ( int i = 0 ; i < row->GetNofRowSegments(); ++i ) 
    {
        AliMpVRowSegment* rs = row->GetRowSegment(i);
        if (rs->IsA() != AliMpRowSegment::Class() ) continue;
        if ( rs->GetPositionX() < rowsegment.GetPositionX() )
        {
            return false;
        }
    }
    return true;
}

template<typename WRITER>
void rowsegment2json(const AliMpVRowSegment& rowsegment, WRITER& w) 
{
    w.StartObject();
    int ix = rowsegment.GetLowLimitIx()-1;
    if (ix && isleftmost(rowsegment)) 
    {
        w.Key("ix");
        w.Int(ix);
    }
    w.Key("row");
    w.Int(rowsegment.GetRow()->GetID());

    // w.Key("nof_motifs(TBR)");
    // w.Int(rowsegment.GetNofMotifs());
    //
    // w.Key("first_motif_id(TBR)");
    // w.Int(rowsegment.GetMotifPositionId(0) & 0x1FF);

    std::vector<int> manuids;

    for ( int i = 0 ; i < rowsegment.GetNofMotifs(); ++i ) 
    {
        manuids.push_back(rowsegment.GetMotifPositionId(i) & 0x1FF);
    }

    // w.Key("increasing_manuids(TBR)");
    // w.Bool(manuids.size()>1 && manuids[0]<manuids[1]);

    WriteArray(w,"manuids",manuids);

    w.EndObject();
}

template<typename WRITER>
void subzone2json(const AliMpSubZone& subzone, WRITER& w) 
{
    {
        w.StartObject();
        w.Key("motif");
        {
            w.StartObject();
            w.Key("id");
            w.String(subzone.GetMotif()->GetID());
            w.Key("type");
            w.String(subzone.GetMotif()->GetMotifType()->GetID());
            w.EndObject();
        }
        w.Key("row_segments");
        {
            w.StartArray();
            for ( int i = 0; i < subzone.GetNofRowSegments(); ++i )
            {
                rowsegment2json(*(subzone.GetRowSegment(i)),w);
            }
            w.EndArray();
        }
        w.EndObject();
    }
}

template<typename WRITER>
void zone2json(const AliMpZone& zone, WRITER& w) 
{
    {
        w.StartObject();
        w.Key("id");
        w.Int(zone.GetID());
        w.Key("pad_sizes");
        {
            w.StartObject();
            w.Key("x");
            w.Double(zone.GetPadDimensionX()*2.0);
            w.Key("y");
            w.Double(zone.GetPadDimensionY()*2.0);
            w.Key("unit");
            w.String("cm");
            w.EndObject();
        }
        w.Key("subzones");
        {
            w.StartArray();
            for ( int i = 0; i < zone.GetNofSubZones(); ++i )
            {
                subzone2json(*(zone.GetSubZone(i)),w);
            }
            w.EndArray();
        }
        w.EndObject();
    }
}

template<typename WRITER>
void sector2json(const AliMpSector& sector, WRITER& w) 
{
    {
        w.StartObject();
        w.Key("sector_data");
        {
            w.StartObject();
            w.Key("nof_zones");
            w.Int(sector.GetNofZones());
            w.Key("nof_rows");
            w.Int(sector.GetNofRows());
            w.Key("direction");
            w.String( (sector.GetDirection()==AliMp::kX)  ? "X":"Y");

            w.Key("position");
            {
                w.StartObject();
                w.Key("x");
                w.Double(sector.GetPositionX());
                w.Key("y");
                w.Double(sector.GetPositionY());
                w.Key("unit");
                w.String("cm");
                w.EndObject();
            }

            w.Key("zones");
            {
                w.StartArray();
                for ( int i = 1; i <= sector.GetNofZones(); ++i )
                {
                    zone2json(*(sector.GetZone(i)),w);
                }
                w.EndArray();
            }
            w.EndObject();
        }
        w.EndObject();
    }
}

#endif
