/**
 * \file bigendiandatarepresentation.cpp
 * \author Arnaud H <arnaud-dev@islog.com>, Maxime C. <maxime-dev@islog.com>
 * \brief BigEndian Data Representation.
 */

#include "logicalaccess/services/accesscontrol/encodings/bigendiandatarepresentation.hpp"
#include "logicalaccess/services/accesscontrol/formats/bithelper.hpp"

#ifdef __unix__
#include <cstring>
#endif

namespace logicalaccess
{
    BigEndianDataRepresentation::BigEndianDataRepresentation()
    {
    }

    BigEndianDataRepresentation::~BigEndianDataRepresentation()
    {
    }

    string BigEndianDataRepresentation::getName() const
    {
        return string("Big Endian");
    }

    EncodingType BigEndianDataRepresentation::getType() const
    {
        return ET_BIGENDIAN;
    }

    unsigned int BigEndianDataRepresentation::convertNumeric(const void* data, size_t dataLengthBytes, unsigned int dataLengthBits, void* convertedData, size_t convertedLengthBytes)
    {
        unsigned int ret = 0;

#if __BYTE_ORDER != __LITTLE_ENDIAN

        if (convertedLengthBytes >= dataLengthBytes)
        {
            memset(convertedData, 0x00, convertedLengthBytes);
            memcpy(convertedData, data, dataLengthBytes);
        }
        ret = dataLengthBits;

#else

        if (convertedData != NULL)
        {
            BitHelper::swapBytes(convertedData, convertedLengthBytes, data, dataLengthBytes, dataLengthBits);
        }
        ret = dataLengthBits;

#endif

        return ret;
    }

    unsigned int BigEndianDataRepresentation::convertBinary(const void* data, size_t dataLengthBytes, unsigned int dataLengthBits, void* convertedData, size_t convertedLengthBytes)
    {
        unsigned int ret = 0;

        if (convertedLengthBytes >= dataLengthBytes)
        {
            memset(convertedData, 0x00, convertedLengthBytes);
            memcpy(convertedData, data, dataLengthBytes);
        }
        ret = dataLengthBits;

        return ret;
    }

    unsigned int BigEndianDataRepresentation::convertLength(unsigned int lengthBits)
    {
        return lengthBits;
    }

    unsigned int BigEndianDataRepresentation::revertNumeric(const void* data, size_t dataLengthBytes, unsigned int dataLengthBits, void* convertedData, size_t convertedLengthBytes)
    {
        unsigned int ret = 0;

#if __BYTE_ORDER != __LITTLE_ENDIAN

        if (convertedLengthBytes >= dataLengthBytes)
        {
            memset(convertedData, 0x00, convertedLengthBytes);
            memcpy(convertedData, data, dataLengthBytes);
        }
        ret = dataLengthBits;

#else

        if (convertedData != NULL)
        {
            BitHelper::swapBytes(convertedData, convertedLengthBytes, data, dataLengthBytes, dataLengthBits);
        }
        ret = dataLengthBits;

#endif

        return ret;
    }

    unsigned int BigEndianDataRepresentation::revertBinary(const void* data, size_t dataLengthBytes, unsigned int dataLengthBits, void* convertedData, size_t convertedLengthBytes)
    {
        unsigned int ret = 0;

        if (convertedLengthBytes >= dataLengthBytes)
        {
            memset(convertedData, 0x00, convertedLengthBytes);
            memcpy(convertedData, data, dataLengthBytes);
        }
        ret = dataLengthBits;

        return ret;
    }
}