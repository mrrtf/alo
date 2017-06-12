#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"
#include <cstdio>
#include <iostream>
#include <rapidjson/ostreamwrapper.h>

using namespace rapidjson;

int main(int argc, char** argv) {
    FILE* fp = fopen(argv[1], "r");
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);
    // OStreamWrapper osw(std::cout);
    // PrettyWriter<OStreamWrapper> writer(osw);
    // d.Accept(writer);
    if (d.HasMember("chs")) {
        std::cout << "chs is an array:" << d["chs"].IsArray() << std::endl;
        for (auto& v: d["chs"].GetArray()) {
            std::cout << v["id"].GetInt() << std::endl;
        }
    }
    fclose(fp);
    return 0;
}
