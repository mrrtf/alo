#ifndef BERG2JSON_H
#define BERG2JSON_H

#include <fstream>
#include <string>

template<typename WRITER>
void berg2json(int npins, const char* filename, WRITER& w) {

    w.Key("id");
    w.Int(npins);
    w.Key("pins");
    w.StartArray();
    std::ifstream in(filename);

    int n;
    std::string s;
    int i = 0;
    while (in>>n>>s) {
        w.StartObject();
        w.Key("id");
        w.Int(n);
        w.Key("manu");
        w.String(s.c_str());
        w.EndObject();
        ++i;
    }
    w.EndArray();
} 

#endif

