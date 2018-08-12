/**
 * \file twicaccesscontrolcardservice.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Twic Access Control Card service.
 */

#ifndef LOGICALACCESS_TWICACCESSCONTROLCARDSERVICE_HPP
#define LOGICALACCESS_TWICACCESSCONTROLCARDSERVICE_HPP

#include "twicchip.hpp"
#include "logicalaccess/services/accesscontrol/accesscontrolcardservice.hpp"

namespace logicalaccess
{
    class TwicCardProvider;

    /**
     * \brief The Twic access control card service class.
     */
    class LIBLOGICALACCESS_API TwicAccessControlCardService : public AccessControlCardService
    {
    public:

        /**
         * \brief Constructor.
         * \param chip The associated chip.
         */
        TwicAccessControlCardService(std::shared_ptr<Chip> chip);

        /**
         * \brief Destructor.
         */
        ~TwicAccessControlCardService();

        /**
         * \brief Read format from the card.
         * \param format The format to read.
         * \param location The format location.
         * \param aiToUse The key's informations to use.
         * \return The format read on success, null otherwise.
         */
        virtual std::shared_ptr<Format> readFormat(std::shared_ptr<Format> format, std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse);

        /**
         * \brief Write format to the card.
         * \param format The format to write.
         * \param location The format location.
         * \param aiToUse The key's informations to use.
         * \param aiToWrite The key's informations to write.
         * \return True on success, false otherwise.
         */
        virtual bool writeFormat(std::shared_ptr<Format> format, std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse, std::shared_ptr<AccessInfo> aiToWrite);

		/**
		* \brief Get the supported format list.
		* \return The format list.
		*/
		virtual FormatList getSupportedFormatList();

    protected:

        std::shared_ptr<TwicChip> getTwicChip() { return std::dynamic_pointer_cast<TwicChip>(getChip()); };
    };
}

#endif