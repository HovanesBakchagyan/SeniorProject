/**
 * \file twiccardprovider.h
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Twic card provider.
 */

#ifndef LOGICALACCESS_TWICSTORAGECARDSERVICE_HPP
#define LOGICALACCESS_TWICSTORAGECARDSERVICE_HPP

#include "logicalaccess/key.hpp"
#include "../iso7816/iso7816storagecardservice.hpp"
#include "twiclocation.hpp"
#include "twiccommands.hpp"
#include "twicchip.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
    /**
     * \brief The Twic storage card service base class.
     */
    class LIBLOGICALACCESS_API TwicStorageCardService : public ISO7816StorageCardService
    {
    public:

        /**
         * \brief Constructor.
         * \param chip The chip.
         */
        TwicStorageCardService(std::shared_ptr<Chip> chip);

        /**
         * \brief Destructor.
         */
        virtual ~TwicStorageCardService();

		/**
         * \brief Read data on a specific DESFire location, using given DESFire keys.
         * \param location The data location.
         * \param aiToUse The key's informations to use for write access.
		 * \param length to read.
         * \param behaviorFlags Flags which determines the behavior.
		 * \return Data readed
         */
        virtual std::vector<unsigned char> readData(std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse, size_t length, CardBehavior behaviorFlags);

    protected:

        std::shared_ptr<TwicChip> getTwicChip() { return std::dynamic_pointer_cast<TwicChip>(getISO7816Chip()); };
    };
}

#endif