/**
 * \file twiciso7816commands.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Twic ISO7816 commands.
 */

#ifndef LOGICALACCESS_TWICISO7816COMMANDS_HPP
#define LOGICALACCESS_TWICISO7816COMMANDS_HPP

#include "logicalaccess/key.hpp"
#include "twic/twiccommands.hpp"
#include "iso7816iso7816commands.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
    class TwicProfile;

    /**
     * \brief The Twic ISO7816 commands base class.
     */
    class LIBLOGICALACCESS_API TwicISO7816Commands : public ISO7816ISO7816Commands
#ifndef SWIG
	, public TwicCommands
#endif            
	{
    public:

        /**
         * \brief Constructor.
         */
        TwicISO7816Commands();

        /**
         * \brief Destructor.
         */
        virtual ~TwicISO7816Commands();

        /**
         * \brief Select the TWIC application.
         */
        virtual void selectTWICApplication();

        /**
         * \brief Get the Unsigned Cardholder Unique Identifier.
         * \return the data.
         */
        virtual std::vector<unsigned char>  getUnsignedCardholderUniqueIdentifier();

        /**
         * \brief Get the TWIC Privacy Key.
         * \return the data.
         * \remarks Only accessible with the contact chip.
         */
        virtual std::vector<unsigned char>  getTWICPrivacyKey();

        /**
         * \brief Get the Cardholder Unique Identifier.
         * \return the data.
         */
        virtual std::vector<unsigned char>  getCardholderUniqueIdentifier();

        /**
         * \brief Get the Cardholder Fingerprints.
         * \return the data.
         */
        virtual std::vector<unsigned char>  getCardHolderFingerprints();

        /**
         * \brief Get the Security Object.
         * \return the data.
         */
        virtual std::vector<unsigned char>  getSecurityObject();

        /**
         * \brief Get TWIC data object.
         * \param dataObject The data object.
         * \return the data.
         */
        virtual std::vector<unsigned char> getTWICData(int64_t dataObject);

    protected:
    };
}

#endif