#include "AliDAQ.h"
#include "AliMpDDL.h"
#include "AliMpDDLStore.h"
#include "json.h"
#include <vector>

std::vector<int> ddl2des(AliMpDDL* ddl) {
    std::vector<int> deids;
    for ( int ide = 0; ide < ddl->GetNofDEs(); ++ide) {
        deids.push_back(ddl->GetDEId(ide));
    }
    return deids;
}

void all_ddl2json(std::ostream& out) {
    out << quote("ddls") << ":[";
    AliMpDDLStore* ddlStore = AliMpDDLStore::Instance();
    for ( int i = 0; i < 20; i++) {
        AliMpDDL* ddl = ddlStore->GetDDL(i);
        out << "{"; 
        out << quote("id") << ":" << AliDAQ::DdlID("MUONTRK",ddl->GetId());
        out << ",";
        outputArray(out,"des",ddl2des(ddl));
        out << "}";
        if (i<19) out << ",";
    }
    out << "]" << std::endl;
}


