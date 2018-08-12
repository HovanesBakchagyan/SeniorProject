/**
 * \file idondemandreaderprovider.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief IdOnDemand card reader provider.
 */

#ifndef LOGICALACCESS_READERIDONDEMAND_PROVIDER_HPP
#define LOGICALACCESS_READERIDONDEMAND_PROVIDER_HPP

#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "idondemandreaderunit.hpp"

#include <string>
#include <vector>

namespace logicalaccess
{
#define READER_IDONDEMAND		"IdOnDemand"

    /**
     * \brief IdOnDemand Reader Provider class.
     */
    class LIBLOGICALACCESS_API IdOnDemandReaderProvider : public ReaderProvider
    {
    public:

        /**
         * \brief Get the ElatecReaderProvider instance. Singleton because we can only have one COM communication, and Elatec can't have shared access, so we share the same reader unit.
         */
        static std::shared_ptr<IdOnDemandReaderProvider> getSingletonInstance();

        /**
         * \brief Destructor.
         */
        ~IdOnDemandReaderProvider();

        /**
         * \brief Release the provider resources.
         */
        virtual void release();

        /**
         * \brief Get the reader provider type.
         * \return The reader provider type.
         */
        virtual std::string getRPType() const { return READER_IDONDEMAND; };

        /**
         * \brief Get the reader provider name.
         * \return The reader provider name.
         */
        virtual std::string getRPName() const { return "idOnDemand TouchSecure"; };

        /**
         * \brief List all readers of the system.
         * \return True if the list was updated, false otherwise.
         */
        virtual bool refreshReaderList();

        /**
         * \brief Get reader list for this reader provider.
         * \return The reader list.
         */
        virtual const ReaderList& getReaderList() { return d_readers; };

        /**
         * \brief Create a new reader unit for the reader provider.
         * \return A reader unit.
         */
        virtual std::shared_ptr<ReaderUnit> createReaderUnit();

    protected:

        /**
         * \brief Constructor.
         */
        IdOnDemandReaderProvider();

        /**
         * \brief The reader list.
         */
        ReaderList d_readers;
    };
}

#endif /* LOGICALACCESS_READERIDONDEMAND_PROVIDER_HPP */