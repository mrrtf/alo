#ifndef ALO_JSONMAP_READER_CH2JSON_H
#define ALO_JSONMAP_READER_CH2JSON_H

#include "json.h"
#include <vector>

template<typename WRITER>
void all_ch2json(std::string topkey, std::vector<std::vector<int>> deids, WRITER& w)
{
    w.StartObject();
    w.Key(topkey.c_str());
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
