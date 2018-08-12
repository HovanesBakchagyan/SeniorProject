/**
 * \file mifareultralightcspringcardcommands.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Mifare Ultralight C - SpringCard.
 */

#include "../commands/mifareultralightcspringcardcommands.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "../pcscreaderprovider.hpp"
#include "logicalaccess/cards/computermemorykeystorage.hpp"
#include "logicalaccess/cards/readermemorykeystorage.hpp"
#include "logicalaccess/cards/samkeystorage.hpp"

namespace logicalaccess
{
    MifareUltralightCSpringCardCommands::MifareUltralightCSpringCardCommands()
        : MifareUltralightCPCSCCommands()
    {
    }

    MifareUltralightCSpringCardCommands::~MifareUltralightCSpringCardCommands()
    {
    }

	void MifareUltralightCSpringCardCommands::startGenericSession()
	{
		// Suspend card tracking
		getPCSCReaderCardAdapter()->sendAPDUCommand(0xFF, 0xFB, 0x01, 0x00);
	}

	void MifareUltralightCSpringCardCommands::stopGenericSession()
	{
		// Resume card tracking
		getPCSCReaderCardAdapter()->sendAPDUCommand(0xFF, 0xFB, 0x00, 0x00);
	}

    std::vector<unsigned char> MifareUltralightCSpringCardCommands::sendGenericCommand(const std::vector<unsigned char>& data)
    {
        return getPCSCReaderCardAdapter()->sendAPDUCommand(0xFF, 0xFE, 0x01, 0x08, static_cast<unsigned char>(data.size()), data);
    }
}