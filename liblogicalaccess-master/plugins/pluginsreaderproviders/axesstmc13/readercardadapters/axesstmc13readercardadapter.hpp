/**
 * \file axesstmc13readercardadapter.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Default AxessTMC13 reader/card adapter.
 */

#ifndef LOGICALACCESS_DEFAULTAXESSTMC13READERCARDADAPTER_HPP
#define LOGICALACCESS_DEFAULTAXESSTMC13READERCARDADAPTER_HPP

#include "logicalaccess/cards/readercardadapter.hpp"

#include <string>
#include <vector>

#include "logicalaccess/logs.hpp"

namespace logicalaccess
{
    /**
     * \brief A default AxessTMC13 reader/card adapter class.
     */
    class LIBLOGICALACCESS_API AxessTMC13ReaderCardAdapter : public ReaderCardAdapter
    {
    public:

        static const unsigned char START0; /**< \brief The first start byte. */
        static const unsigned char START1; /**< \brief The second start byte. */
        static const unsigned char CR; /**< \brief The stop byte. */

        /**
         * \brief Constructor.
         */
        AxessTMC13ReaderCardAdapter();

        /**
         * \brief Destructor.
         */
        virtual ~AxessTMC13ReaderCardAdapter();

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

#endif /* LOGICALACCESS_DEFAULTAXESSTMC13READERCARDADAPTER_H */