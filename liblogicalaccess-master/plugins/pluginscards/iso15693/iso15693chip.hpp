/**
 * \file iso15693chip.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief ISO15693 chip.
 */

#ifndef LOGICALACCESS_ISO15693CHIP_HPP
#define LOGICALACCESS_ISO15693CHIP_HPP

#include "logicalaccess/cards/chip.hpp"
#include "iso15693commands.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
#define	CHIP_ISO15693	"ISO15693"

    /**
     * \brief The ISO15693 base chip class.
     */
    class LIBLOGICALACCESS_API ISO15693Chip : public Chip
    {
    public:

        /**
         * \brief Constructor.
         */
        ISO15693Chip(std::string ct);

        /**
         * \brief Constructor.
         */
        ISO15693Chip();

        /**
         * \brief Destructor.
         */
        virtual ~ISO15693Chip();

        /**
         * \brief Get the generic card type.
         * \return The generic card type.
         */
        virtual std::string getGenericCardType() const { return CHIP_ISO15693; };

        /**
         * \brief Get the root location node.
         * \return The root location node.
         */
        virtual std::shared_ptr<LocationNode> getRootLocationNode();

        /**
         * \brief Get a card service for this chip.
         * \param serviceType The card service type.
         * \return The card service.
         */
        virtual std::shared_ptr<CardService> getService(CardServiceType serviceType);

		/**
		* \brief Create default ISO15693 location.
		* \return Default ISO15693 location.
		*/
		virtual std::shared_ptr<Location> createLocation() const;

        /**
         * \brief Get the ISO15693 card provider for I/O access.
         * \return The ISO15693 card provider.
         */
        std::shared_ptr<ISO15693Commands> getISO15693Commands() { return std::dynamic_pointer_cast<ISO15693Commands>(getCommands()); };

    protected:
    };
}

#endif