/**
 * \file infineonmydchip.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Infineon My-D chip.
 */

#ifndef LOGICALACCESS_INFINEONMYDCHIP_HPP
#define LOGICALACCESS_INFINEONMYDCHIP_HPP

#include "../iso15693/iso15693chip.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
#define CHIP_INFINEONMYD	"InfineonMYD"

    /**
     * \brief The Infineon My-D base chip class.
     */
    class LIBLOGICALACCESS_API InfineonMYDChip : public ISO15693Chip
    {
    public:

        /**
         * \brief Constructor.
         */
        InfineonMYDChip();

        /**
         * \brief Destructor.
         */
        virtual ~InfineonMYDChip();

        /**
         * \brief Get the root location node.
         * \return The root location node.
         */
        virtual std::shared_ptr<LocationNode> getRootLocationNode();

    protected:
    };
}

#endif