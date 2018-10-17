#include "pcb.h"
#include "AliMpSlat.h"
#include "AliMpSlatSegmentation.h"
#include "AliMpPCB.h"
#include <vector>
#include "seg.h"
#include "de.h"
#include "AliMpDDLStore.h"
#include "AliMpSegmentation.h"
#include <algorithm>

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

std::vector<AliMpPCB*> get_allpcbs(AliMpDDLStore* ddlStore, AliMpSegmentation* mseg) {

/// get the list of all the PCBs defined for the slats
/// returned vector is ordered alphabetically by PCB ID

    std::vector<AliMpPCB*> pcbs;
    std::vector<int> deids = get_deids(ddlStore);
    std::vector<AliMpVSegmentation*> b = get_segs(mseg,deids,AliMp::kBendingPlane);
    std::vector<AliMpVSegmentation*> nb = get_segs(mseg,deids,AliMp::kNonBendingPlane);

    std::vector<AliMpVSegmentation*> segs;

    segs.insert(segs.end(),b.begin(),b.end());
    segs.insert(segs.end(),nb.begin(),nb.end());

    for ( auto& seg: segs) {
        const AliMpSlat* slat = slat_from_seg(*seg);
        if (slat) {
            getPCBs(*slat,pcbs);
        }
    }

    std::sort(pcbs.begin(),pcbs.end(),[](AliMpPCB* a, AliMpPCB* b) { return strcmp(a->GetID(),b->GetID())<0; });

    return pcbs;
}

