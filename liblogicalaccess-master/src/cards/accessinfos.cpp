/**
 * \file accessinfo.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Base class access informations.
 */

#include "logicalaccess/myexception.hpp"
#include "logicalaccess/cards/accessinfo.hpp"
#include "logicalaccess/services/accesscontrol/formats/format.hpp"
#include <time.h>
#include <stdlib.h>

#include "logicalaccess/logs.hpp"

#include <openssl/rand.h>

#include <iostream>
#include <string>
#include <iomanip>

namespace logicalaccess
{
    AccessInfo::AccessInfo()
    {
    }

    string AccessInfo::generateSimpleKey(size_t keySize)
    {
        std::ostringstream oss;

#ifdef _WINDOWS
        RAND_screen();
#endif

        EXCEPTION_ASSERT_WITH_LOG(RAND_status() == 1, LibLogicalAccessException, "Insufficient enthropy source");

        std::vector<unsigned char> buf;
        buf.resize(keySize, 0x00);

        if (RAND_bytes(&buf[0], static_cast<int>(keySize)) != 1)
        {
            THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Cannot retrieve cryptographically strong bytes");
        }

        for (size_t i = 0; i < buf.size(); ++i)
        {
            if (i != 0)
            {
                oss << " ";
            }

            oss << std::hex << std::setfill('0') << std::setw(2) << (size_t)buf[i];
        }

        return oss.str();
    }

    string AccessInfo::generateSimpleDESKey(size_t keySize)
    {
        std::ostringstream oss;

#ifdef _WINDOWS
        RAND_screen();
#endif

        EXCEPTION_ASSERT_WITH_LOG(RAND_status() == 1, LibLogicalAccessException, "Insufficient enthropy source");

        std::vector<unsigned char> buf;
        buf.resize(keySize, 0x00);

        if (RAND_bytes(&buf[0], static_cast<int>(keySize)) != 1)
        {
            THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Cannot retrieve cryptographically strong bytes");
        }

        for (size_t i = 0; i < buf.size(); ++i)
        {
            if (i != 0)
            {
                oss << " ";
            }

            buf.data()[i] = buf[i] & (0xfe | Format::calculateParity(&buf[i], 1, PT_ODD, 0, 7));

            oss << std::hex << std::setfill('0') << std::setw(2) << (size_t)buf[i];
        }

        return oss.str();
    }

    bool AccessInfo::operator==(const AccessInfo& ai) const
    {
        return (typeid(ai) == typeid(*this));
    }
}