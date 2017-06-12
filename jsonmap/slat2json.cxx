#include "AliMpMotifPosition.h"
#include "AliMpPCB.h"
#include "json.h"
#include "slat2json.h"

void pcb2json(const AliMpPCB& pcb, std::ostream& out) {
    out << "{";
    out << quote("id") << ":" << quote(pcb.GetID());
    out << ",";
    // out << quote("manus") << ":" << quote("zob"); 
    std::vector<int> manus;
    for ( int i = 0 ; i < pcb.GetSize(); ++i ) {
        AliMpMotifPosition* mp = pcb.GetMotifPosition(i);
        manus.push_back(mp->GetID());
        // break;
    }
    outputArray(out,"manus",manus);
    out << "}";
}

void slat2json(const AliMpSlat& slat, std::ostream& out) {
    out << std::endl;
    out << "{";
    out << quote("pcbs") << ":[";
    for ( int i = 0; i < slat.GetSize(); ++i ) {
        AliMpPCB* pcb = slat.GetPCB(i);
        pcb2json(*pcb,out);
        if ( i < slat.GetSize() - 1 ) out << ",";
    }
    out << "]}";
}


