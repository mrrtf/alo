#include "ch2json.h"

#include "AliMpDEIterator.h"
#include "json.h"

void all_ch2json(std::ostream& out)
{
    AliMpDEIterator it;
    out << quote("chs") << ":[";
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

        outputArray(out,"des",deids);

        out << "}";
        if ( ch < 9 ) out << ",";
    }
    out << "]" << std::endl;
}
