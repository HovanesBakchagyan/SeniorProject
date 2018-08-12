/**
 * \file rfideasreaderprovider.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief RFIDeas card reader provider.
 */

#ifndef LOGICALACCESS_READERRFIDEAS_PROVIDER_HPP
#define LOGICALACCESS_READERRFIDEAS_PROVIDER_HPP

#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "rfideasreaderunit.hpp"

#include <string>
#include <vector>

#include "logicalaccess/logs.hpp"

namespace logicalaccess
{
#define READER_RFIDEAS		"RFIDeas"

    /**
     * \brief RFIDeas Reader Provider class.
     */
    class LIBLOGICALACCESS_API RFIDeasReaderProvider : public ReaderProvider
    {
    public:

        /**
         * \brief Get the RFIDeasReaderProvider instance. Singleton because we can only have one communication, and RFIDeas can't have shared access, so we share the same reader unit.
         */
        static std::shared_ptr<RFIDeasReaderProvider> getSingletonInstance();

        /**
         * \brief Destructor.
         */
        ~RFIDeasReaderProvider();

        /**
         * \brief Release the provider resources.
         */
        virtual void release();

        /**
         * \brief Get the reader provider type.
         * \return The reader provider type.
         */
        virtual std::string getRPType() const { return READER_RFIDEAS; };

        /**
         * \brief Get the reader provider name.
         * \return The reader provider name.
         */
        virtual std::string getRPName() const { return "RFIDeas"; };

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
        RFIDeasReaderProvider();

        /**
         * \brief The reader list.
         */
        ReaderList d_readers;
    };
}

#endif /* LOGICALACCESS_READERRFIDEAS_PROVIDER_HPP */