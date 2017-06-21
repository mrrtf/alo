#ifndef CH2JSON_H
#define CH2JSON_H

#include "json.h"
#include <vector>

template<typename WRITER>
void all_ch2json(std::vector<std::vector<int>> deids, WRITER& w)
{
    w.StartObject();
    w.Key("chs");
    w.StartArray();
    for ( int ch = 0; ch < 10; ++ch) {
        w.StartObject();
        w.Key("id");
        w.Int(ch);
        WriteArray(w,"des",deids[ch]);
        w.EndObject();
    }
    w.EndArray();
    w.EndObject();
}

#endif
