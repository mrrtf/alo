#ifndef ALIHLTMUONDIGITBLOCK_H
#define ALIHLTMUONDIGITBLOCK_H

#include "TObject.h"
#include "AliHLTComponent.h"
#include "AliHLTMUONDataBlockWriter.h"
#include "AliHLTMUONUtils.h"
#include "AliHLTMUONDataTypes.h"
#include <ostream>

extern "C"
{

    /**
     * Gives the fired digit/pad information.
     */
    struct AliHLTMUONDigitStruct
    {
        AliHLTUInt32_t fId;    // Digit ID in the current mapping (from OCDB)
        AliHLTUInt16_t fIndex; // Digit index in the new mapping (produced internally)
        AliHLTUInt16_t fADC;   // ADC value of signal
    };

    /**
     * AliHLTMUONDigitBlockStruct defines the format of the internal digit data block.
     */
    struct AliHLTMUONDigitsBlockStruct
    {
        AliHLTMUONDataBlockHeader fHeader; // Common data block header

        // Array of digits/pads.
        //AliHLTMUONDigitStruct fDigit[/*fHeader.fNrecords*/];
    };

} // extern "C"


/**
 * Stream operator for usage with std::ostream classes which prints the
 * AliHLTMUONDigitStruct in the following format:
 *   {fId = xx, fIndex = yy, fADC = zz}
 */
std::ostream& operator << (
        std::ostream& stream, const AliHLTMUONDigitStruct& digit
        );

/**
 * Stream operator for usage with std::ostream classes which prints the
 * AliHLTMUONDigitsBlockStruct in the following format:
 *   {fHeader = xx, fDigit[] = [{..}, {..}, ...]}
 */
std::ostream& operator << (
        std::ostream& stream, const AliHLTMUONDigitsBlockStruct& block
        );


inline bool operator == (
        const AliHLTMUONDigitStruct& a,
        const AliHLTMUONDigitStruct& b
        )
{
    return a.fId == b.fId and a.fIndex == b.fIndex and a.fADC == b.fADC;
}

inline bool operator != (
        const AliHLTMUONDigitStruct& a,
        const AliHLTMUONDigitStruct& b
        )
{
    return not operator == (a, b);
}


bool operator == (
        const AliHLTMUONDigitsBlockStruct& a,
        const AliHLTMUONDigitsBlockStruct& b
        );

inline bool operator != (
        const AliHLTMUONDigitsBlockStruct& a,
        const AliHLTMUONDigitsBlockStruct& b
        )
{
    return not operator == (a, b);
}

namespace AliHLTMUONDigitBlock {

    const int kDataBlockType = 2003;

    bool HeaderOk(
            const AliHLTMUONDigitsBlockStruct& block,
            AliHLTMUONUtils::WhyNotValid* reason, AliHLTUInt32_t& reasonCount
            );

    /**
     * Method used to check if the header information corresponds to the
     * supposed type of the digits data block.
     * This method will return either kHeaderContainsWrongType or
     * kHeaderContainsWrongRecordWidth as the reason code.
     * \param [in]  block  The data block to check.
     * \param [out] reason  If this is not NULL, then the variable pointed to
     *      by this pointer will be filled with the reason code describing why
     *      the header is not valid, if and only if a problem is found with
     *      the data.
     * \returns  true if there is no problem with the header and false otherwise.
     */
    bool HeaderOk(const AliHLTMUONDigitsBlockStruct& block, AliHLTMUONUtils::WhyNotValid* reason = NULL);

    bool IntegrityOk(
            const AliHLTMUONDigitStruct& digit,
            AliHLTMUONUtils::WhyNotValid* reason,
            AliHLTUInt32_t& reasonCount
            );
    bool IntegrityOk(
            const AliHLTMUONDigitsBlockStruct& block,
            AliHLTMUONUtils::WhyNotValid* reason,
            AliHLTUInt32_t* recordNum,
            AliHLTUInt32_t& reasonCount
            );

    /**
     * This method is used to check more extensively if the integrity of the
     * digit data structure is OK and returns true in that case.
     * \param [in]  digit  The digit structure to check.
     * \param [out] reason  If this is not NULL, then it will be filled with
     *      the reason code describing why the structure is not valid, if and
     *      only if a problem is found with the data.
     * \returns  true if there is no problem with the data and false otherwise.
     */
    bool IntegrityOk(
            const AliHLTMUONDigitStruct& digit,
            AliHLTMUONUtils::WhyNotValid* reason = NULL
            );

    /**
     * This method is used to check more extensively if the integrity of the
     * dHLT raw internal data block is OK and returns true in that case.
     * \param [in]  block  The digits data block to check.
     * \param [out] reason  If this is not NULL, then it will be filled with
     *      the reason code describing why the data block is not valid, if and
     *      only if a problem is found with the data.
     * \param [out] recordNum  If this is not NULL, then it will be filled with
     *      the number of the digit structure that had a problem. This value
     *      will only contain a valid value if the method
     *      RecordNumberWasSet(*reason) returns true. Thus, 'reason' must be set.
     * \returns  true if there is no problem with the data and false otherwise.
     */
    bool IntegrityOk(
            const AliHLTMUONDigitsBlockStruct& block,
            AliHLTMUONUtils::WhyNotValid* reason = NULL, AliHLTUInt32_t* recordNum = NULL
            );

    const AliHLTComponentDataType& DigitBlockDataType();
}

typedef AliHLTMUONDataBlockWriter<
AliHLTMUONDigitsBlockStruct,
    AliHLTMUONDigitStruct,
    AliHLTMUONDataBlockType(AliHLTMUONDigitBlock::kDataBlockType)
    > AliHLTMUONDigitsBlockWriter;

#endif
