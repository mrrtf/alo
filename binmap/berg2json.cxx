#include "AliMpFiles.h"
#include "AliMpStation12Type.h"
#include "AliMpStationType.h"
#include "berg2json.h"
#include "json.h"
#include <iostream>
#include <sstream>
#include <fstream>

void berg2json(int npins, const char* filename, std::ostream& out) {

    out << "{";

    out << quote("id") << ":" << npins;
    out << ",";
    out << quote("pins") << ":[";

    std::ifstream in(filename);

    int n;
    std::string s;
    int i = 0;
    while (in>>n>>s) {
        out << "{";
        out << quote("id") << ":" <<n;
        out << ",";
        out << quote("manu") << ":"<< quote(s.c_str());
        out << "}";
        if (i<npins-1) out <<",";
        ++i;
    }
    out << "]";

    out << "}";
} 

void all_berg2json(std::ostream& out) {

    out << quote("bergs") << ":" << "[";

    berg2json(80,AliMpFiles::BergToGCFilePath(AliMp::kStation12,AliMq::kStation1),out);

     out << ",";
     berg2json(100,AliMpFiles::BergToGCFilePath(AliMp::kStation345,AliMq::kStation1),out);
    out << "]";
}
