/**
 * \file iso15693pcsccommands.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief ISO15693 PC/SC Commands
 */

#ifndef LOGICALACCESS_ISO15693PCSCCOMMANDS_HPP
#define LOGICALACCESS_ISO15693PCSCCOMMANDS_HPP

#include "iso15693/iso15693commands.hpp"
#include "../readercardadapters/pcscreadercardadapter.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
    /**
     * \brief A ISO 15693 PC/SC commands base class.
     */
    class LIBLOGICALACCESS_API ISO15693PCSCCommands : public ISO15693Commands
    {
    public:
        /**
         * \brief Constructor.
         */
        ISO15693PCSCCommands();

        /**
         * \brief Destructor.
         */
        virtual ~ISO15693PCSCCommands();

        virtual void stayQuiet();
        virtual std::vector<unsigned char> readBlock(size_t block, size_t le = 0);
        virtual void writeBlock(size_t block, const std::vector<unsigned char>& data);
        virtual void lockBlock(size_t block);
        virtual void writeAFI(size_t afi);
        virtual void lockAFI();
        virtual void writeDSFID(size_t dsfid);
        virtual void lockDSFID();
        virtual ISO15693PCSCCommands::SystemInformation getSystemInformation();
        virtual unsigned char getSecurityStatus(size_t block);

        /**
         * \brief Get the PC/SC reader/card adapter.
         * \return The PC/SC reader/card adapter.
         */
        virtual std::shared_ptr<PCSCReaderCardAdapter> getPCSCReaderCardAdapter() { return std::dynamic_pointer_cast<PCSCReaderCardAdapter>(getReaderCardAdapter()); };
    };
}

#endif /* LOGICALACCESS_ISO15693PCSCCOMMANDS_HPP */