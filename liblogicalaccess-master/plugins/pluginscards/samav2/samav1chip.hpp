/**
 * \file samav1chip.hpp
 * \author Adrien J. <adrien.jund@islog.com>
 * \brief SAMAV1Chip header.
 */

#ifndef LOGICALACCESS_SAMAV1CHIP_HPP
#define LOGICALACCESS_SAMAV1CHIP_HPP

#include "logicalaccess/cards/chip.hpp"
#include "samcommands.hpp"
#include "samchip.hpp"

namespace logicalaccess
{
#define CHIP_SAMAV1	"SAM_AV1"

    /**
     * \brief The SAM chip class.
     */
    class LIBLOGICALACCESS_API SAMAV1Chip : public SAMChip
    {
    public:

        /**
         * \brief Constructor.
         */
        SAMAV1Chip();

        SAMAV1Chip(std::string ct);

        /**
         * \brief Destructor.
         */
        ~SAMAV1Chip();

        std::shared_ptr<SAMCommands<KeyEntryAV1Information, SETAV1> > getSAMAV1Commands() { return std::dynamic_pointer_cast<SAMCommands<KeyEntryAV1Information, SETAV1>>(getCommands()); };
    protected:
    };
}

#endif /* LOGICALACCESS_SAMAV2CHIP_HPP */