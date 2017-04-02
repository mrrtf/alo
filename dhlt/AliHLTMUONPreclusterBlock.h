#ifndef ALIHLTMUONPRECLUSTERBLOCK_H
#define ALIHLTMUONPRECLUSTERBLOCK_H

/* This file is property of and copyright by the ALICE HLT Project        *
 * ALICE Experiment at CERN, All rights reserved.                         *
 * See cxx source for full Copyright notice                               */

// $Id$

/**
 * @file   AliHLTMUONPreclusterBlock.h
 * @author Philippe Pillot <pillot@subatech.in2p3.fr>
 * @date   01 December 2014
 * @brief  Class to read/write a data block of preclusters.
 */

#include <vector>
#include "AliHLTLogging.h"
#include "AliHLTMUONDigitBlock.h"

extern "C"
{
  /**
   * Gives the fired channel/pad information which was considered during
   * hit reconstruction and correlated into a common cluster.
   */
  struct AliHLTMUONPreclusterStruct
  {
    AliHLTUInt16_t fNDigits; // number of digits attached to this precluster
    const AliHLTMUONDigitStruct *fDigits; // pointer to the 1st element of the array of digits
  };
}

/**
 * Class to handle the writing/reading of a block of preclusters
 * to/from the provided buffer of the given size.
 */
class AliHLTMUONPreclusterBlock : public AliHLTLogging
{
public:

  static const AliHLTComponentDataType& DataType();

  /**
   * Default constructor that produces an object not usable yet.
   * The method Reset(...) must be called to initialize it properly.
   */
  AliHLTMUONPreclusterBlock();

  /**
   * Constructor that sets the internal pointer to the start of the buffer
   * space to read/write to and the total size of the buffer in bytes.
   * @param buffer  The pointer to the first byte of the memory buffer.
   * @param size    The total size of the buffer in bytes.
   * @param write   Tell wether the buffer is for writing or reading.
   */
  AliHLTMUONPreclusterBlock(void *buffer, AliHLTUInt32_t size, bool write = false);

  /**
   * Destructor to delete internal list of preclusters
   */
  virtual ~AliHLTMUONPreclusterBlock();

  /**
   * Method that resets the internal pointer to the start of the new buffer
   * space to read/write to and the total size of this buffer in bytes.
   * @param buffer  The pointer to the first byte of the memory buffer.
   * @param size    The total size of the buffer in bytes.
   * @param write   Tell wether the buffer is for writing or reading.
   */
  int Reset(void *buffer, AliHLTUInt32_t size, bool write = false);

  /**
   * Method to fill the buffer with a new precluster and its first digit
   * @param digit  Reference to the first digit to add to the new precluster.
   */
  int StartPrecluster(const AliHLTMUONDigitStruct &digit);

  /**
   * Method to add a new digit to the current precluster
   * @param digit  Reference to the digit to add to the current precluster.
   */
  int AddDigit(const AliHLTMUONDigitStruct &digit);

  /**
   * Return the number of bytes currently used for the data block in the buffer.
   */
  AliHLTUInt32_t BytesUsed() const {return (fCurrentSize > fSize) ? 0 : fCurrentSize;}

  /**
   * Return the number of preclusters currently stored in the data block.
   */
  AliHLTUInt16_t GetNPreclusters() const {return (fNPreclusters == 0x0) ? 0 : *fNPreclusters;}

  /**
   * Return the vector of preclusters currently stored in the data block.
   */
  const std::vector<AliHLTMUONPreclusterStruct>& GetPreclusters() const
  { return fPreclusters; }

  /**
   * Return the name of the class for log messages.
   */
  virtual const char* Class_Name() { return "AliHLTMUONPreclusterBlock"; }

private:

  // Do not allow copying of this class.

  /// Not implemented.
  AliHLTMUONPreclusterBlock(const AliHLTMUONPreclusterBlock& /*obj*/);

  /// Not implemented.
  AliHLTMUONPreclusterBlock& operator = (const AliHLTMUONPreclusterBlock& /*obj*/);

  /// read the buffer
  int ReadBuffer();

  static const AliHLTUInt32_t fgkSizeOfUShort = sizeof(AliHLTUInt16_t);
  static const AliHLTUInt32_t fgkSizeOfDigit = sizeof(AliHLTMUONDigitStruct);

  /// total size of the buffer
  AliHLTUInt32_t fSize;

  /// read/write mode
  bool fWriteMode;
  AliHLTUInt32_t fSize4PreCl; ///< total buffer size minus the one resquested to add a new precluster
  AliHLTUInt32_t fSize4Digit; ///< total buffer size minus the one resquested to add a new digit
  AliHLTUInt32_t fCurrentSize; ///< size of the used part of the buffer

  /// running pointer on the buffer
  /**
  indicates where the next chunk of data is written in "write" mode,
  or where it is read, in "read" mode
  **/
  void* fBuffer;

  /// number of preclusters
  AliHLTUInt16_t *fNPreclusters;

  /// number of digits in the last precluster (write mode)
  AliHLTUInt16_t *fLastNDigits;

  /// list of preclusters
  std::vector<AliHLTMUONPreclusterStruct> fPreclusters;

};

/// stream operator for printout
std::ostream& operator << ( std::ostream& stream, const AliHLTMUONPreclusterStruct& );

/// stream operator for printout
std::ostream& operator << ( std::ostream& stream, const AliHLTMUONPreclusterBlock& );

#endif
