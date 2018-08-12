/**
 * \file smartidreaderprovider.hpp
 * \author Maxime C. <maxime-dev@islog.com>, Julien K. <julien-dev@islog.com>
 * \brief SmartID card reader provider.
 */

#ifndef LOGICALACCESS_READERSMARTID_PROVIDER_HPP
#define LOGICALACCESS_READERSMARTID_PROVIDER_HPP

#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "smartidreaderunit.hpp"

#include <string>
#include <vector>

#include "logicalaccess/logs.hpp"

namespace logicalaccess
{
#define READER_SMARTID		"SmartID"

    /**
     * \brief SmartID Reader Provider class.
     */
    class LIBLOGICALACCESS_API SmartIDReaderProvider : public ReaderProvider
    {
    public:

        /**
         * \brief Get the SmartIDReaderProvider instance. Singleton because we can only have one COM communication, and the SmartID can't have shared access, so we share the same reader unit.
         */
        static std::shared_ptr<SmartIDReaderProvider> getSingletonInstance();

        /**
         * \brief Destructor.
         */
        ~SmartIDReaderProvider();

        /**
         * \brief Release the provider resources.
         */
        virtual void release();

        /**
         * \brief Get the reader provider type.
         * \return The reader provider type.
         */
        virtual std::string getRPType() const { return READER_SMARTID; };

        /**
         * \brief Get the reader provider name.
         * \return The reader provider name.
         */
        virtual std::string getRPName() const { return "HID SmartID (T=CL)"; };

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
        SmartIDReaderProvider();

        /**
         * \brief The reader list.
         */
        ReaderList d_readers;
    };
}

#endif /* LOGICALACCESS_READERSMARTID_PROVIDER_HPP */