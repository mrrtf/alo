#include "AliMpConnection.h"
#include "AliMpDataMap.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"
#include "AliMpVMotif.h"
#include "json.h"
#include "motif2json.h"
#include "pcb2json.h"
#include <cassert>
#include <string>


void connection2json(const AliMpConnection& c, std::ostream& out) {
    out << "{";
    out << quote("manuchannel") << ":" << c.GetManuChannel(); // TODO: to be removed
    out << ",";
    out << quote("berg") << ":" << c.GetBergNum();
    out << ",";
    out << quote("ix") << ":" << c.GetLocalIx();
    out << ",";
    out << quote("iy") << ":" << c.GetLocalIy();
    out << "}";
}

void motif2json(const AliMpMotifType& mt, std::ostream& out) {
    out << "{";
    out << quote("id") << ":" << quote(mt.GetID());
    out << ",";
    out << quote("is_full") << ":" << (mt.IsFull() ? "true":"false");
    out << ",";
    out << quote("nof_pads") << ":" << mt.GetNofPads();
    out << ",";
    out << quote("pads") << ":[";
    int lastpad;
    for ( int i = 0; i < 64; ++i ) {
        if (mt.FindConnectionByPadNum(i)) {
            lastpad=i;
        }
    }
    for ( int i = 0; i < 64; ++i ) {
        AliMpConnection* c = mt.FindConnectionByPadNum(i);
        if (!c) continue;
        connection2json(*c,out);
        if ( i != lastpad )  out << ",";
    }
    out << "]";
    out << "}";
}

void all_motif2json(std::ostream& out) {

    AliMpDataProcessor mp;
    AliMpDataMap* dataMap = mp.CreateDataMap("data");
    AliMpDataStreams dataStreams(dataMap);

    std::vector<AliMpPCB*> pcbs = get_allpcbs(dataStreams);

    std::vector<AliMpMotifType*> motifTypes = get_allmotiftypes(pcbs);

    std::sort(motifTypes.begin(),motifTypes.end(),
            [](AliMpMotifType* a, AliMpMotifType* b) {
            return strcmp(a->GetID().Data(),b->GetID().Data())<0; }
            );

    out << quote("motifs") << ":[";

    std::vector<AliMpMotifType*>::size_type i = 0;

    for (auto& mt: motifTypes) {
        motif2json(*mt,out);
        if ( i < motifTypes.size() - 1 ) out << ",";
        ++i;
    }

    out << "]";
}
