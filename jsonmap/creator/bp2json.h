#ifndef BP2JSON_H
#define BP2JSON_H

#include "AliMpBusPatch.h"
#include "AliMpDDLStore.h"
#include "bp2json.h"
#include "de2json.h"
#include "json.h"
#include <vector>

template<typename WRITER>
void frt2json(const AliMpBusPatch& bp, WRITER& w) 
{
    w.Key("frt");
    w.StartObject();
    w.Key("id");
    w.Int(bp.GetFrtId());
    w.Key("pos");
    w.String(bp.GetFRTPosition());
    w.EndObject();
}

template<typename WRITER>
void cable2json(const AliMpBusPatch& bp, WRITER& w)
{
    w.Key("cable");
    w.StartObject();
    if (bp.GetCableLabel() != "someName") {
        w.Key("label");
        w.String(bp.GetCableLabel());
    }
    w.Key("length");
    w.Double(bp.GetCableLength());
    w.Key("length_unit");
    w.String("m");
    w.EndObject();
}

template<typename WRITER>
void patchmodule2json(const AliMpBusPatch& bp, WRITER& w)
{
    std::vector<int> pm;
    for ( int i = 0; i < bp.GetNofPatchModules(); ++i ) 
    {
        pm.push_back(bp.GetNofManusPerModule(i));
    }
    WriteArray(w,"nof_manus_per_patch_module",pm);
}

template<typename WRITER>
void bp2json(const AliMpBusPatch& bp, WRITER& w) {
    w.StartObject();
    w.Key("id");
    w.Int(bp.GetId());
    frt2json(bp,w);
    cable2json(bp,w);
    patchmodule2json(bp,w);
    std::vector<int> manus;
    for ( int i = 0; i < bp.GetNofManus(); ++i) {
        manus.push_back(bp.GetManuId(i));
    }
    WriteArray(w,"manus",manus);
    w.EndObject();
}

template<typename WRITER>
void all_bp2json(std::vector<AliMpBusPatch*>& bps, WRITER& w) 
{
    w.StartObject();
    w.Key("bps");
    w.StartArray();
    for ( auto& b: bps )
    {
        bp2json(*b,w);
    }
    w.EndArray();
    w.EndObject();
}

#endif

