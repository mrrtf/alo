/**************************************************************************
 * Copyright(c) 1998-2007, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// $Id$

///
/// @file   AliHLTMUONDigitBlock.cxx
/// @author Philippe Pillot <pillot@subatech.in2p3.fr>
//  @author Laurent Aphecetche (re-massaged to fit into alo repo in March 2017)
/// @date   16 December 2014
/// @brief  Implementation of the stream and comparison operators.
///
/// The digits blocks are internal dimuon HLT block structures
/// containing information about reconstructed digits.
///

#include "AliHLTMUONDigitBlock.h"
#include "AliHLTMUONUtils.h"
#include <cassert>

std::ostream& operator << (
        std::ostream& stream, const AliHLTMUONDigitStruct& digit
        )
{
    std::ios::fmtflags oldflags = stream.flags();
    stream	<< "{fId = " << digit.fId
        << ", fIndex = " << digit.fIndex
        << ", fADC = " << digit.fADC << "}";
    stream.flags(oldflags);
    return stream;
}

std::ostream& operator << (
        std::ostream& stream, const AliHLTMUONDigitsBlockStruct& block
        )
{
    assert( AliHLTMUONDigitBlock::IntegrityOk(block) );

    const AliHLTMUONDigitStruct* digit =
        reinterpret_cast<const AliHLTMUONDigitStruct*>(&block + 1);
    stream 	<< "{fHeader = " << block.fHeader << ", fDigit[] = [";
    if (block.fHeader.fNrecords > 0) stream << digit[0];
    for (AliHLTUInt32_t i = 1; i < block.fHeader.fNrecords; i++)
        stream << ", " << digit[i];
    stream << "]}";
    return stream;
}


bool operator == (
        const AliHLTMUONDigitsBlockStruct& a,
        const AliHLTMUONDigitsBlockStruct& b
        )
{
    assert( AliHLTMUONDigitBlock::IntegrityOk(a) );
    assert( AliHLTMUONDigitBlock::IntegrityOk(b) );

    const AliHLTMUONDigitStruct* digitA =
        reinterpret_cast<const AliHLTMUONDigitStruct*>(&a + 1);
    const AliHLTMUONDigitStruct* digitB =
        reinterpret_cast<const AliHLTMUONDigitStruct*>(&b + 1);

    // First check if the blocks have the same header. If they do then check
    // if every digit is the same. In either case if we find a difference
    // return false.
    if (a.fHeader != b.fHeader) return false;
    for (AliHLTUInt32_t i = 0; i < a.fHeader.fNrecords; i++)
        if (digitA[i] != digitB[i]) return false;
    return true;
}


namespace AliHLTMUONDigitBlock {

    const AliHLTComponentDataType& DigitBlockDataType()
    {
        // Returns a information block type for digits.
        // Returns a information block type for digits.
        static const AliHLTComponentDataType digitBlockDataType = AliHLTComponentDataTypeInitializer("DIGITS  ", kAliHLTDataOriginMUON);
        return digitBlockDataType;
    }

    bool IntegrityOk(
            const AliHLTMUONDigitStruct& digit,
            AliHLTMUONUtils::WhyNotValid* reason,
            AliHLTUInt32_t& reasonCount
            )
    {
        /// This method is used to check more extensively if the integrity of the
        /// digit structure is OK and returns true in that case.
        /// \param [in]  digit  The digit structure to check.
        /// \param [out] reason  If this is not NULL, then it is assumed to point
        ///      to an array of at least 'reasonCount' number of elements. It will
        ///      be filled with the reason codes describing why the structure is
        ///      not valid.
        /// \param [in,out] reasonCount  This should initially specify the size of
        ///      the array pointed to by 'reason'. It will be filled with the number
        ///      of items actually filled into the reason array upon exit from this
        ///      method.
        /// \returns  true if there is no problem with the structure and false otherwise.

        AliHLTUInt32_t maxCount = reasonCount;
        reasonCount = 0;
        bool result = true;

        // Check that the digit ID has a valid value.
        if (not (digit.fId > 0))
        {
            if (reason != NULL and reasonCount < maxCount)
            {
                reason[reasonCount] = AliHLTMUONUtils::kInvalidIdValue;
                reasonCount++;
            }
            result = false;
        }

        // Check that the ADC signal has a valid value, which fits into 12 bits.
        if ((digit.fADC & (~0xFFF)) != 0)
        {
            if (reason != NULL and reasonCount < maxCount)
            {
                reason[reasonCount] = AliHLTMUONUtils::kInvalidSignal;
                reasonCount++;
            }
            result = false;
        }

        return result;
    }


    bool IntegrityOk(
            const AliHLTMUONDigitsBlockStruct& block,
            AliHLTMUONUtils::WhyNotValid* reason,
            AliHLTUInt32_t* recordNum,
            AliHLTUInt32_t& reasonCount
            )
    {
        /// This method is used to check more extensively if the integrity of the
        /// dHLT internal digits data block is OK and returns true in that case.
        /// \param [in]  block  The digits data block to check.
        /// \param [out] reason  If this is not NULL, then it is assumed to point
        ///      to an array of at least 'reasonCount' number of elements. It will
        ///      be filled with the reason codes describing why the data block is
        ///      not valid.
        /// \param [out] recordNum  If this is not NULL, then it is assumed to point
        ///      to an array of at least 'reasonCount' number of elements. It will
        ///      be filled with the number of the digit structure that had a problem.
        ///      The value 'recordNum[i]' will only contain a valid value if
        ///      the corresponding 'reason[i]' contains one of:
        ///        - kInvalidIdValue
        ///        - kInvalidSignal
        /// \note You can use RecordNumberWasSet(reason[i]) to check if 'recordNum[i]'
        ///      was set and is valid or not.
        /// \param [in,out] reasonCount  This should initially specify the size of
        ///      the array pointed to by 'reason' and 'recordNum'. It will be filled
        ///      with the number of items actually filled into the arrays upon exit
        ///      from this method.
        /// \returns  true if there is no problem with the data and false otherwise.

        AliHLTUInt32_t maxCount = reasonCount;
        bool result = HeaderOk(block, reason, reasonCount);

        const AliHLTMUONDigitStruct* digit =
            reinterpret_cast<const AliHLTMUONDigitStruct*>(&block + 1);

        // Check integrity of individual digit structures.
        for (AliHLTUInt32_t i = 0; i < block.fHeader.fNrecords; i++)
        {
            AliHLTUInt32_t filledCount = maxCount - reasonCount;
            if (not IntegrityOk(digit[i], reason+reasonCount, filledCount))
            {
                // reasons filled in IntegrityOk, now we just need to adjust
                // reasonCount and fill the recordNum values.
                if (recordNum != NULL)
                {
                    for (AliHLTUInt32_t n = 0; n < filledCount; n++)
                        recordNum[reasonCount + n] = i;
                }
                reasonCount += filledCount;
                result = false;
            }
        }

        return result;
    }


    bool HeaderOk(
            const AliHLTMUONDigitsBlockStruct& block,
            AliHLTMUONUtils::WhyNotValid* reason, AliHLTUInt32_t& reasonCount
            )
    {
        /// Method used to check if the header information corresponds to the
        /// supposed type of the raw dHLT data block.
        /// \param [in]  block  The data block to check.
        /// \param [out] reason  If this is not NULL, then it is assumed to point
        ///      to an array of at least 'reasonCount' number of elements. It will
        ///      be filled with the reason codes describing why the header is not
        ///      valid.
        /// \param [in,out] reasonCount  This should initially specify the size of
        ///      the array pointed to by 'reason'. It will be filled with the number
        ///      of items actually filled into the reason array upon exit from this
        ///      method.
        /// \returns  true if there is no problem with the header and false otherwise.

        AliHLTUInt32_t maxCount = reasonCount;
        reasonCount = 0;
        bool result = true;

        // The block must have the correct type.
        if (block.fHeader.fType != kDataBlockType)
        {
            if (reason != NULL and reasonCount < maxCount)
            {
                reason[reasonCount] = AliHLTMUONUtils::kHeaderContainsWrongType;
                reasonCount++;
            }
            result = false;
        }

        // The block's record width must be the correct size.
        if (block.fHeader.fRecordWidth != sizeof(AliHLTMUONDigitStruct))
        {
            if (reason != NULL and reasonCount < maxCount)
            {
                reason[reasonCount] = AliHLTMUONUtils::kHeaderContainsWrongRecordWidth;
                reasonCount++;
            }
            result = false;
        }

        return result;
    }


    bool HeaderOk(const AliHLTMUONDigitsBlockStruct& block, AliHLTMUONUtils::WhyNotValid* reason)
    {
        AliHLTUInt32_t count = 1;
        return HeaderOk(block, reason, count);
    }

    bool IntegrityOk(
            const AliHLTMUONDigitStruct& digit,
            AliHLTMUONUtils::WhyNotValid* reason)
    {
        AliHLTUInt32_t count = 1;
        return IntegrityOk(digit, reason, count);
    }

    bool IntegrityOk(
            const AliHLTMUONDigitsBlockStruct& block,
            AliHLTMUONUtils::WhyNotValid* reason, AliHLTUInt32_t* recordNum)
    {
        AliHLTUInt32_t count = 1;
        return IntegrityOk(block, reason, recordNum, count);
    }
}

