#include "AliMpBusPatch.h"
#include "AliMpDDLStore.h"
#include "bp2json.h"
#include "de2json.h"
#include "json.h"
#include <vector>

void frt2json(const AliMpBusPatch& bp, std::ostream& out) {
    out << quote("frt") << ":{";
    out << quote("id") << ":" << bp.GetFrtId();
    out << ",";
    out << quote("pos") << ":" << quote(bp.GetFRTPosition());
    out << "}";
}

void cable2json(const AliMpBusPatch& bp, std::ostream& out) {
    out << quote("cable") << ":{";
    if (bp.GetCableLabel() != "someName") {
        out << quote("label") << ":" << quote(bp.GetCableLabel()) << ",";
    }
    out << quote("length") << ":" << bp.GetCableLength() << ",";
    out << quote("length_unit") << ":" << quote("m");
    out << "}";
}

void patchmodule2json(const AliMpBusPatch& bp, std::ostream& out) {
    std::vector<int> pm;
    for ( int i = 0; i < bp.GetNofPatchModules(); ++i ) 
    {
        pm.push_back(bp.GetNofManusPerModule(i));
    }
    outputArray(out,"nof_manus_per_patch_module",pm);
}

void bp2json(int bpid, std::ostream& out) {
    AliMpBusPatch* bp = AliMpDDLStore::Instance()->GetBusPatch(bpid);
    out << "{";
    out << quote("id") << ":" << bpid;
    out << ",";
    frt2json(*bp,out);
    out << ",";
    cable2json(*bp,out);
    out << ",";
    patchmodule2json(*bp,out);
    out << ",";
    std::vector<int> manus;
    for ( int i = 0; i < bp->GetNofManus(); ++i) {
        manus.push_back(bp->GetManuId(i));
    }
    outputArray(out,"manus",manus);
    out << "}";
}

void all_bp2json(std::ostream& out) {
    std::vector<int> bpids = de2bp(get_deids());
    out << quote("bps") << ":[";
    for ( std::vector<int>::size_type i = 0; i < bpids.size(); ++i) {
        bp2json(bpids[i],out);
        if ( i < bpids.size()-1) out << ",";
    }
    out << "]";
}


