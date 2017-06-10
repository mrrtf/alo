#include "AliMpDataMap.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"
#include "AliMpSlat.h"
#include "AliMpSlatSegmentation.h"
#include "AliMpVMotif.h"
#include "json.h"
#include "pcb2json.h"
#include "seg2json.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <string>
#include <vector>

void getPCBs(const AliMpSlat& slat, std::map<std::string,AliMpPCB*>& pcbs)
{
    for ( int i = 0; i < slat.GetSize(); ++i ) {
        AliMpPCB* pcb = slat.GetPCB(i);
        if ( !pcbs.count(pcb->GetID())) {
            pcbs[std::string(pcb->GetID())]=pcb;
        }
    }
}

std::map<std::string, AliMpPCB*> get_allpcbs(AliMpDataStreams& dataStreams) {

    std::map<std::string,AliMpPCB*> pcbs;

    std::vector<std::string> segtypes = get_segtypes();

    for ( auto seg: segtypes ) {
        if (!is_slat(seg)) continue;

        std::vector<AliMpVSegmentation*> mseg = { 
            get_segmentation(seg,AliMp::kNonBendingPlane,dataStreams),
            get_segmentation(seg,AliMp::kBendingPlane,dataStreams) };

        std::for_each(mseg.begin(),mseg.end(),[&](AliMpVSegmentation* seg){
                AliMpSlatSegmentation* slatSeg = dynamic_cast<AliMpSlatSegmentation*>(seg);
                const AliMpSlat* slat = slatSeg->Slat();
                getPCBs(*slat,pcbs);
                });
    }

    return pcbs;
}

void pcb2json(AliMpPCB& pcb, std::ostream& out)
{
    out << "{";
    out << quote("id") << ":" << quote(pcb.GetID());
    out << ",";
    out << quote("padsize") << ":{";
    out << quote("x") << ":" << pcb.PadSizeX() << ",";
    out << quote("y") << ":" << pcb.PadSizeY() << ",";
    out << quote("unit") << ":" << quote("cm");
    out << "}";
    out << ",";
    out << quote("motifs") << ":[";
    for ( int i = 0; i < pcb.GetSize(); ++i ) {
        out << "{";
        AliMpMotifPosition* mp = pcb.GetMotifPosition(i);
        AliMpVMotif* m = mp->GetMotif();
        AliMpMotifType* mt = m->GetMotifType();
        assert(m->GetNofPadDimensions()==1);
        int ix = std::round((mp->GetPositionX()-m->DimensionX()-pcb.Xmin()) / (2*m->GetPadDimensionX(0)));
        int iy = (mp->GetPositionY()-m->DimensionY()) / (2*m->GetPadDimensionY(0));
        out << quote("id") << ":" << quote(mt->GetID());
        out << ",";
        out << quote("pos") << ":{";
        out << quote("ix") << ":" << ix;
        out << ",";
        out << quote("iy") << ":" << iy;
        out << "}";
        out << "}";
        if ( i < pcb.GetSize()-1 ) out << ",";
    }
    out << "]";
    out << "}";
}

void all_pcb2json(std::ostream& out) {

    AliMpDataProcessor mp;
    AliMpDataMap* dataMap = mp.CreateDataMap("data");
    AliMpDataStreams dataStreams(dataMap);

    std::map<std::string,AliMpPCB*> pcbs = get_allpcbs(dataStreams);

    out << quote("pcbs") << ":[";
    std::map<std::string,AliMpPCB*>::size_type i = 0;
    for ( auto& s: pcbs ) {
        pcb2json(*(s.second),out);
        if (i<pcbs.size()-1) out << ",";
        i++;
        
    } 
    out << "]";
}
