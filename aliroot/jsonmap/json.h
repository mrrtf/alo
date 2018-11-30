#ifndef ALO_JSONMAP_READER_JSON_H
#define ALO_JSONMAP_READER_JSON_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"

template<class StreamType, class WriterType>
class OutputFile {
    public:
    OutputFile(std::string filename) : mFile(fopen(filename.c_str(), "wb")),
    mWriteBuffer(new char[65536]),
    mStream(mFile,mWriteBuffer,sizeof(mWriteBuffer)),
    mWriter(mStream) {}

    virtual ~OutputFile() {
        delete[] mWriteBuffer;
        fclose(mFile);
    }
    
    WriterType& Writer() { return mWriter;}

    private:
    FILE* mFile;
    char* mWriteBuffer;
    StreamType mStream;
    WriterType mWriter;
};

using OF = OutputFile<rapidjson::FileWriteStream,rapidjson::PrettyWriter<rapidjson::FileWriteStream>>;
using OFC = OutputFile<rapidjson::FileWriteStream,rapidjson::Writer<rapidjson::FileWriteStream>>;

template<typename T>
void WriteArray(T& w, std::string key, std::vector<int> v)
{
    w.Key(key.c_str());
    w.StartArray();
    for ( std::vector<int>::size_type i = 0; i < v.size(); ++i) {
        w.Int(v[i]);
    }
    w.EndArray();
}

#endif

