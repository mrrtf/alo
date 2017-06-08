// macro to convert mapping txt files into json format

#include "AliMpDataProcessor.h"
#include "AliMpDataMap.h"
#include "AliMpDataStreams.h"
#include "AliMpDEStore.h"
#include "AliMpDEIterator.h"
#include "AliMpDetElement.h"
#include <iostream>
#include <vector>
#include <string>

std::string quote(const char* s)
{
    std::string r = "\"";
    r += s;
    r += "\"";
    return r;
}

void ch2json(ostream& out)
{
    AliMpDEIterator it;
    out << quote("ch") << ":[";
    for ( int ch = 0; ch < 10; ++ch) {
        it.First(ch);
        std::vector<int> deids;

        out << "{" << quote("id") << ":" << ch << ",";

        while (!it.IsDone()) {
            if (it.CurrentDEId() < 1100) {
                deids.push_back(it.CurrentDEId());
            }
            it.Next();
        }

        out << quote("de") << ":[";

        for ( std::vector<int>::size_type i = 0; i < deids.size(); ++i ) {
            out << deids[i];
            if ( i < deids.size()-1 ) out << ",";
        }

        out << "] }";
        if ( ch < 9 ) out << ",";
    }
    out << "]" << std::endl;
}

void de2json(AliMpDEStore* deStore, ostream& out)
{
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

    out << quote("de") << ":[";

    for ( std::vector<int>::size_type i = 0; i < deids.size(); ++i ) {
        
        AliMpDetElement* de = deStore->GetDetElement(deids[i]);

        out << "{ \"id\":" << deids[i] << ",";
        out << quote("name") << ":" << quote(de->GetDEName()) << ",";
        out << quote("type") << ":" << quote(de->GetSegType()) <<",";
        out << quote("cath0_is_bending_plane") << ":"
            << ( (de->GetPlaneType(AliMp::kCath0)==AliMp::kBendingPlane)?1:0);
        out << "}";
        if ( i < deids.size()-1 ) out << ",";
    }
    out << "]" << std::endl;
}

void txt2json() {

    AliMpDataProcessor mp;
    AliMpDataMap* dataMap = mp.CreateDataMap("data");
    AliMpDataStreams dataStreams(dataMap);

    AliMpDEStore* deStore = AliMpDEStore::ReadData(dataStreams,kFALSE);

    std::ofstream out("txt.json");

    out << "{";
    ch2json(out);
    out << "," << std::endl;
    de2json(deStore,out);
    out << "}" << std::endl;
}
