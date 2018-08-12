/**
 * \file a3mlgm5600readerprovider.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief A3MLGM5600 card reader provider.
 */

#ifndef LOGICALACCESS_READERA3MLGM5600_PROVIDER_HPP
#define LOGICALACCESS_READERA3MLGM5600_PROVIDER_HPP

#include "a3mlgm5600readerunit.hpp"
#include "logicalaccess/readerproviders/readerprovider.hpp"

#include <string>
#include <vector>

#include "logicalaccess/logs.hpp"

namespace logicalaccess
{
#define READER_A3MLGM5600		"A3MLGM5600"

    /**
     * \brief A3MLGM5600 Reader Provider class.
     */
    class LIBLOGICALACCESS_API A3MLGM5600ReaderProvider : public ReaderProvider
    {
    public:

        /**
         * \brief Get the A3MLGM5600ReaderProvider instance. Singleton because we can only have one UDP communication, and A3MLGM5600 can't have shared access, so we share the same reader unit.
         */
        static std::shared_ptr<A3MLGM5600ReaderProvider> getSingletonInstance();

        /**
         * \brief Destructor.
         */
        ~A3MLGM5600ReaderProvider();

        /**
         * \brief Release the provider resources.
         */
        virtual void release();

        /**
         * \brief Get the reader provider type.
         * \return The reader provider type.
         */
        virtual std::string getRPType() const { return READER_A3MLGM5600; };

        /**
         * \brief Get the reader provider name.
         * \return The reader provider name.
         */
        virtual std::string getRPName() const { return "A3M LGM5600"; };

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
        A3MLGM5600ReaderProvider();

        /**
         * \brief The reader list.
         */
        ReaderList d_readers;
    };
}

#endif /* LOGICALACCESS_READERA3MLGM5600_PROVIDER_HPP */