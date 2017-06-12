#include <string>

#include "AliMpDEStore.h"
#include "AliMpDataMap.h"
#include "AliMpDataProcessor.h"
#include "AliMpDataStreams.h"
#include "AliMpDetElement.h"
#include "AliMpPCB.h"
#include "AliMpSector.h"
#include "AliMpSectorReader.h"
#include "AliMpSectorSegmentation.h"
#include "AliMpSlat.h"
#include "AliMpSlatMotifMap.h"
#include "AliMpSlatSegmentation.h"
#include "AliMpSt345Reader.h"
#include "de2json.h"
#include "json.h"
#include "slat2json.h"
#include <set>
#include <vector>

AliMpSlatMotifMap* slatmap() {
    static AliMpSlatMotifMap* m = new AliMpSlatMotifMap;
    return m;
}

bool is_slat(std::string segtype) {
    return (segtype.find("st") == std::string::npos);
}

std::vector<std::string> get_segtypes()
{
    std::set<std::string> st;
    std::vector<int> deids = get_deids();
    AliMpDEStore* deStore = AliMpDEStore::Instance();
    for ( auto d: deids) {
        AliMpDetElement* de = deStore->GetDetElement(d);
        st.insert(std::string(de->GetSegType()));
    }
    std::vector<std::string> stv(st.begin(),st.end());
    return stv;
}

AliMpVSegmentation* get_segmentation(std::string segtype, AliMp::PlaneType planeType, AliMpDataStreams& dataStreams)
{
    AliMpVSegmentation* seg = nullptr;

    if (is_slat(segtype)) {
        AliMpSt345Reader reader(slatmap());
        AliMpSlat* slat = reader.ReadSlat(dataStreams, segtype.c_str(), planeType);
        seg = new AliMpSlatSegmentation(slat,true);
    }
    else {
        if (segtype.find("st1")!=std::string::npos)
        {
            AliMpSectorReader reader(AliMq::kStation1, planeType);
            AliMpSector* sector = reader.BuildSector(dataStreams);
            seg = new AliMpSectorSegmentation(sector, true);
        } else { 
            AliMpSectorReader reader(AliMq::kStation2, planeType);
            AliMpSector* sector = reader.BuildSector(dataStreams);
            seg = new AliMpSectorSegmentation(sector, true);
        }
    }
    return seg;
}

void segplane2json(AliMpVSegmentation* seg, std::ostream& out)
{
    out << std::endl;
    out << (seg->PlaneType()==AliMp::kBendingPlane ? quote("bending") : quote("non-bending")) << ":";

    if (seg->StationType()==AliMp::kStation345) {
        AliMpSlatSegmentation* slatSeg = dynamic_cast<AliMpSlatSegmentation*>(seg);
        slat2json(*(slatSeg->Slat()),out);
    }
    else {
        out << "{";
        out << quote("class") << ":" << quote(seg->ClassName());
        out << "}";
    }
}

void seg2json(std::string segtype, AliMpDataStreams& dataStreams, std::ostream& out)
{
    out << std::endl;
    out << "{";
   out << quote("id") << ":" << quote(segtype.c_str());
   out << ",";
   out << quote("is_slat") << ":" << is_slat(segtype) << ",";
   AliMpVSegmentation* seg = get_segmentation(segtype,AliMp::kBendingPlane,dataStreams);
   segplane2json(seg,out);
   out << ",";
   seg = get_segmentation(segtype,AliMp::kNonBendingPlane,dataStreams);
   segplane2json(seg,out);
    out << "}";
}

void all_seg2json(std::ostream& out) {
    
    std::vector<std::string> segtypes = get_segtypes();

    out << quote("segs") << ":[";

    AliMpDataProcessor mp;
    AliMpDataMap* dataMap = mp.CreateDataMap("data");
    AliMpDataStreams dataStreams(dataMap);


    for ( std::set<std::string>::size_type i = 0; i < segtypes.size(); ++i ) {
        seg2json(segtypes[i],dataStreams,out);
        if ( i < segtypes.size() - 1 ) out << ",";
    }

    out << "]";
}

