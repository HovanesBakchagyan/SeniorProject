/**
 * \file mifareultralightpcsccommands.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Mifare Ultralight pcsc card.
 */

#ifndef LOGICALACCESS_MIFAREULTRALIGHTPCSCCOMMANDS_HPP
#define LOGICALACCESS_MIFAREULTRALIGHTPCSCCOMMANDS_HPP

#include "mifareultralight/mifareultralightcommands.hpp"
#include "../readercardadapters/pcscreadercardadapter.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
    /**
     * \brief The Mifare Ultralight commands class for PCSC reader.
     */
    class LIBLOGICALACCESS_API MifareUltralightPCSCCommands : public virtual MifareUltralightCommands
    {
    public:

        /**
         * \brief Constructor.
         */
        MifareUltralightPCSCCommands();

        /**
         * \brief Destructor.
         */
        virtual ~MifareUltralightPCSCCommands();

        /**
         * \brief Get the PC/SC reader/card adapter.
         * \return The PC/SC reader/card adapter.
         */
        std::shared_ptr<PCSCReaderCardAdapter> getPCSCReaderCardAdapter() { return std::dynamic_pointer_cast<PCSCReaderCardAdapter>(getReaderCardAdapter()); };

        /**
         * \brief Read a whole page.
         * \param sector The page number, from 0 to 15.
         * \param buf A buffer to fill with the data of the page.
         * \param buflen The length of buffer. Must be at least 4 bytes long or the call will fail.
         * \return The number of bytes red, or a negative value on error.
         */
        virtual std::vector<unsigned char> readPage(int page);

        /**
         * \brief Write a whole page.
         * \param sector The page number, from 0 to 15.
         * \param buf A buffer to from which to copy the data.
         * \param buflen The length of buffer. Must be at least 4 bytes long or the call will fail.
         * \return The number of bytes written, or a negative value on error.
         */
        virtual void writePage(int page, const std::vector<unsigned char>& buf);

    protected:
    };
}

#endif /* LOGICALACCESS_MIFAREULTRALIGHTPCSCCOMMANDS_HPP */