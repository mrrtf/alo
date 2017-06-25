#include "AliDAQ.h"
#include "AliMpDDL.h"
#include "AliMpDDLStore.h"
#include <vector>

void ddl_read_mapping(AliMpDDLStore* ddlStore,
        std::vector<AliMpDDL*>& ddls,
        std::vector<std::vector<int>>& desperddl)
{
    for ( int i = 0; i < 20; i++) {
        AliMpDDL* ddl = ddlStore->GetDDL(i);
        ddls.push_back(ddl);
        std::vector<int> deids;
        for ( int ide = 0; ide < ddl->GetNofDEs(); ++ide) {
            deids.push_back(ddl->GetDEId(ide));
        }
        desperddl.push_back(deids);
    }
}

