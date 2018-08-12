/**
 * \file iso14443areadercommunication.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief ISO14443-3A Reader communication.
 */

#ifndef LOGICALACCESS_ISO14443AREADERCOMMUNICATION_HPP
#define LOGICALACCESS_ISO14443AREADERCOMMUNICATION_HPP

#include "logicalaccess/readerproviders/readercommunication.hpp"

#include <string>
#include <vector>

#include "logicalaccess/logs.hpp"

namespace logicalaccess
{
    /**
     * \brief A ISO14443-3A reader communication base class.
     */
    class LIBLOGICALACCESS_API ISO14443AReaderCommunication : public virtual ReaderCommunication
    {
    public:

        /**
         * \brief Send a REQA command from the PCD to the PICC.
         * \return The ATQB PICC result.
         */
        virtual std::vector<unsigned char> requestA() = 0;

        /**
         * \brief Send a RATS command from the PCD to the PICC.
         * \return The ATS PICC result.
         */
        virtual std::vector<unsigned char> requestATS() = 0;

        /**
         * \brief Send a HLTB command from the PCD to the PICC.
         */
        virtual void haltA() = 0;

        /**
         * \brief Manage collision.
         * \return The chip UID.
         */
        virtual std::vector<unsigned char> anticollisionA() = 0;
    };
}

#endif /* LOGICALACCESS_ISO14443AREADERCOMMUNICATION_HPP */