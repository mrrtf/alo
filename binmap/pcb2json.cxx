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
#include <string>
#include <vector>

void getPCBs(const AliMpSlat& slat, std::vector<AliMpPCB*>& pcbs)
{
    for ( int i = 0; i < slat.GetSize(); ++i ) {
        AliMpPCB* pcb = slat.GetPCB(i);
        if ( std::find_if(pcbs.begin(),pcbs.end(),
                    [&pcb](AliMpPCB* p) {
                        return strcmp(pcb->GetID(),p->GetID())==0; 
                        }
                    ) == pcbs.end() ) {
            pcbs.push_back(pcb);
        }
    }
}

std::vector<AliMpPCB*> get_allpcbs(AliMpDataStreams& dataStreams) {

    std::vector<AliMpPCB*> pcbs;

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

    std::vector<AliMpPCB*> pcbs = get_allpcbs(dataStreams);

    out << quote("pcbs") << ":[";
    std::vector<AliMpPCB*>::size_type i = 0;
    for ( auto& s: pcbs ) {
        pcb2json(*s,out);
        if (i<pcbs.size()-1) out << ",";
        i++;
        
    } 
    out << "]";
}
