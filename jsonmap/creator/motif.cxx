#include "motif.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"
#include "AliMpMotifPosition.h"
#include <algorithm>

std::vector<AliMpMotifType*> get_allmotiftypes(const std::vector<AliMpPCB*>& pcbs) {

    std::vector<AliMpMotifType*> motifTypes;

    for (auto& pcb: pcbs) {
        for ( int i = 0; i < pcb->GetSize(); ++i ) {
            AliMpMotifType* mt = pcb->GetMotifPosition(i)->GetMotif()->GetMotifType();
            if ( std::find_if(motifTypes.begin(),motifTypes.end(),
                        [&mt](AliMpMotifType* a) { 
                        return strcmp(a->GetID(),mt->GetID())==0; }
                        ) == motifTypes.end()) {
                motifTypes.push_back(mt);
            }
        }
    }

    return motifTypes;
}
