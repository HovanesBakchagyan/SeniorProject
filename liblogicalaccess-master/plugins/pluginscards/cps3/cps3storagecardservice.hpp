/**
 * \file cps3storagecardservice.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief CPS3 storage card service.
 */

#ifndef LOGICALACCESS_CPS3STORAGECARDSERVICE_HPP
#define LOGICALACCESS_CPS3STORAGECARDSERVICE_HPP

#include "../iso7816/iso7816storagecardservice.hpp"
#include "cps3chip.hpp"

namespace logicalaccess
{
    /**
     * \brief The CPS3 storage card service base class.
     */
    class LIBLOGICALACCESS_API CPS3StorageCardService : public ISO7816StorageCardService
    {
    public:

        /**
         * \brief Constructor.
         */
		CPS3StorageCardService(std::shared_ptr<Chip> chip);

        /**
         * \brief Destructor.
         */
		virtual ~CPS3StorageCardService();

        /**
         * \brief Read data on a specific Tag-It location, using given Tag-It keys.
         * \param location The data location.
         * \param aiToUse The key's informations to use for write access.
         * \param length to read.
         * \param behaviorFlags Flags which determines the behavior.
		 * \return Data readed
         */
        virtual std::vector<unsigned char> readData(std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse, size_t dataLength, CardBehavior behaviorFlags);

    protected:

		std::shared_ptr<CPS3Chip> getCPS3Chip() { return std::dynamic_pointer_cast<CPS3Chip>(getISO7816Chip()); };
    };
}

#endif