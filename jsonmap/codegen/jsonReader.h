//
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See https://alice-o2.web.cern.ch/ for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef ALO_JSONMAP_READER_JSONREADER_H
#define ALO_JSONMAP_READER_JSONREADER_H

#include <cstdlib>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

template<typename StreamType>
class InputDocument {
 public:
  InputDocument(const char* filename) : mFile(fopen(filename,"r")),
                                        mReadBuffer(new char[65536]),
                                        mStream(mFile,mReadBuffer,sizeof(mReadBuffer)), mDocument() {
    mDocument.ParseStream(mStream);
  }


  rapidjson::Document& document() { return mDocument; }

  virtual ~InputDocument() {
    delete[] mReadBuffer;
    fclose(mFile);
  }

 private:
  FILE* mFile;
  char* mReadBuffer;
  StreamType mStream;
  rapidjson::Document mDocument;
};

using InputWrapper = InputDocument<rapidjson::FileReadStream>;

#endif
