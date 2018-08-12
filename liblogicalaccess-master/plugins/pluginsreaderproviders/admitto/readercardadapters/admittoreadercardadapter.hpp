/**
 * \file admittoreadercardadapter.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Default Admitto reader/card adapter.
 */

#ifndef LOGICALACCESS_DEFAULTADMITTOREADERCARDADAPTER_HPP
#define LOGICALACCESS_DEFAULTADMITTOREADERCARDADAPTER_HPP

#include "logicalaccess/cards/readercardadapter.hpp"

#include <string>
#include <vector>

#include "logicalaccess/logs.hpp"
#include "logicalaccess/bufferhelper.hpp"

namespace logicalaccess
{
    /**
     * \brief A default Admitto reader/card adapter class.
     */
    class LIBLOGICALACCESS_API AdmittoReaderCardAdapter : public ReaderCardAdapter
    {
    public:

        static const unsigned char CR; /**< \brief The first stop byte. */
        static const unsigned char LF; /**< \brief The second stop byte. */

        /**
         * \brief Constructor.
         */
        AdmittoReaderCardAdapter();

        /**
         * \brief Destructor.
         */
        virtual ~AdmittoReaderCardAdapter();

        /**
         * \brief Adapt the command to send to the reader.
         * \param command The command to send.
         * \return The adapted command to send.
         */
        virtual std::vector<unsigned char> adaptCommand(const std::vector<unsigned char>& command);

        /**
         * \brief Adapt the asnwer received from the reader.
         * \param answer The answer received.
         * \return The adapted answer received.
         */
        virtual std::vector<unsigned char> adaptAnswer(const std::vector<unsigned char>& answer);
    };
}

#endif /* LOGICALACCESS_DEFAULTADMITTOREADERCARDADAPTER_HPP */