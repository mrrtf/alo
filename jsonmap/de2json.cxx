#include "AliMpDEStore.h"
#include "AliMpDEIterator.h"
#include <vector>
#include "AliMpDetElement.h"
#include "json.h"
#include "de2json.h"

std::vector<int> get_deids() {
    AliMpDEStore* deStore = AliMpDEStore::Instance();
    AliMpDEIterator it;
    it.First();

    std::vector<int> deids;

    while (!it.IsDone()) {
        if (it.CurrentDEId() < 1100) {
        deids.push_back(it.CurrentDEId());
        }
        it.Next();
    }

    std::sort(deids.begin(),deids.end());
    return deids;
}

std::vector<int> de2bp(const std::vector<int> deids) {
    // create a vector of buspatchid for the given detection elements id
    std::vector<int> buspatches;
    AliMpDEStore* deStore = AliMpDEStore::Instance();
    
    for ( auto d : deids ) {
        AliMpDetElement* de = deStore->GetDetElement(d);
        for ( int ibp = 0; ibp < de->GetNofBusPatches(); ++ibp ) {
            buspatches.push_back(de->GetBusPatchId(ibp));
        }
    }
    return buspatches;
}

void de2json(int deid, std::ostream& out)
{
    AliMpDEStore* deStore = AliMpDEStore::Instance();
    AliMpDetElement* de = deStore->GetDetElement(deid);

    out << "{ \"id\":" << deid << ",";
    out << quote("name") << ":" << quote(de->GetDEName()) << ",";
    out << quote("segtype") << ":" << quote(de->GetSegType()) <<",";
    out << quote("segname") << ":[" << quote(de->GetSegName(AliMp::kCath0)) 
        <<"," << quote(de->GetSegName(AliMp::kCath1)) << "],";
    out << quote("cath0_is_bending_plane") << ":"
        << ( (de->GetPlaneType(AliMp::kCath0)==AliMp::kBendingPlane)?1:0);

    out << ",";

    std::vector<int> buspatches = de2bp(std::vector<int>{deid});

    outputArray(out,"bps",buspatches);
    out << "}";

}

void all_de2json(std::ostream& out)
{
    std::vector<int> deids = get_deids();

    out << quote("des") << ":[";

    for ( std::vector<int>::size_type i = 0; i < deids.size(); ++i ) {
        de2json(deids[i],out); 
        if ( i < deids.size()-1 ) out << ",";

    }
    out << "]" << std::endl;
}

