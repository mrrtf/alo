#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"

template<class StreamType, class WriterType>
class OutputFile {
    public:
    OutputFile(const char* filename) : mFile(fopen(filename, "wb")),
    mWriteBuffer(new char[65536]),
    mStream(mFile,mWriteBuffer,sizeof(mWriteBuffer)),
    mWriter(mStream) {}

    ~OutputFile() {
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

template<typename T>
void WriteArray(T& w, const char* key, std::vector<int> v) {
    std::sort(v.begin(),v.end());
    w.Key(key);
    w.StartArray();
    for ( std::vector<int>::size_type i = 0; i < v.size(); ++i) {
        w.Int(v[i]);
    }
    w.EndArray();
}

#endif

