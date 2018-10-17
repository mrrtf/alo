#include "bp.h"
#include "de.h"
#include "AliMpDetElement.h"
#include "AliMpBusPatch.h"
#include "AliMpDDLStore.h"

std::vector<int> get_bpids(AliMpDDLStore* ddlStore, const std::vector<int> deids) 
{
    std::vector<int> bps;
    for ( auto d : deids ) {
        AliMpDetElement* de = ddlStore->GetDetElement(d);
        for ( int ibp = 0; ibp < de->GetNofBusPatches(); ++ibp ) {
            bps.push_back(de->GetBusPatchId(ibp));
        }
    }
    return bps;
}

std::vector<AliMpBusPatch*> get_bps(AliMpDDLStore* ddlStore, const std::vector<int> deids) 
{
    std::vector<AliMpBusPatch*> bps;
    std::vector<int> bpids = get_bpids(ddlStore,deids);

    for ( auto& b : bpids) {
        bps.push_back(ddlStore->GetBusPatch(b));
    }
    return bps;
}

