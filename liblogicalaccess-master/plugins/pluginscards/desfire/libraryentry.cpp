#include <string>
#include <memory>
#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "desfireev1chip.hpp"
#include "desfirechip.hpp"
#include "sagemkeydiversification.hpp"
#include "nxpav2keydiversification.hpp"
#include "nxpav1keydiversification.hpp"
#include "omnitechkeydiversification.hpp"
#include "logicalaccess/cards/keydiversification.hpp"

#ifdef _MSC_VER
#include "logicalaccess/msliblogicalaccess.h"
#else
#ifndef LIBLOGICALACCESS_API
#define LIBLOGICALACCESS_API
#endif
#ifndef DISABLE_PRAGMA_WARNING
#define DISABLE_PRAGMA_WARNING /**< \brief winsmcrd.h was modified to support this macro, to avoid MSVC specific warnings pragma */
#endif
#endif

extern "C"
{
    LIBLOGICALACCESS_API char *getLibraryName()
    {
        return (char *)"DESFire";
    }

    LIBLOGICALACCESS_API void getDESFireEV1Chip(std::shared_ptr<logicalaccess::Chip>* chip)
    {
        if (chip != NULL)
        {
            *chip = std::shared_ptr<logicalaccess::DESFireEV1Chip>(new logicalaccess::DESFireEV1Chip());
        }
    }

    LIBLOGICALACCESS_API void getDESFireChip(std::shared_ptr<logicalaccess::Chip>* chip)
    {
        if (chip != NULL)
        {
            *chip = std::shared_ptr<logicalaccess::DESFireChip>(new logicalaccess::DESFireChip());
        }
    }

    LIBLOGICALACCESS_API void getNXPAV1Diversification(std::shared_ptr<logicalaccess::KeyDiversification>* keydiversification)
    {
        if (keydiversification != NULL)
        {
            *keydiversification = std::shared_ptr<logicalaccess::NXPAV1KeyDiversification>(new logicalaccess::NXPAV1KeyDiversification());
        }
    }

    LIBLOGICALACCESS_API void getNXPAV2Diversification(std::shared_ptr<logicalaccess::KeyDiversification>* keydiversification)
    {
        if (keydiversification != NULL)
        {
            *keydiversification = std::shared_ptr<logicalaccess::NXPAV2KeyDiversification>(new logicalaccess::NXPAV2KeyDiversification());
        }
    }

    LIBLOGICALACCESS_API void getSagemDiversification(std::shared_ptr<logicalaccess::KeyDiversification>* keydiversification)
    {
        if (keydiversification != NULL)
        {
            *keydiversification = std::shared_ptr<logicalaccess::SagemKeyDiversification>(new logicalaccess::SagemKeyDiversification());
        }
    }

	LIBLOGICALACCESS_API void getOmnitechDiversification(std::shared_ptr<logicalaccess::KeyDiversification>* keydiversification)
    {
        if (keydiversification != NULL)
        {
            *keydiversification = std::shared_ptr<logicalaccess::OmnitechKeyDiversification>(new logicalaccess::OmnitechKeyDiversification());
        }
    }

    LIBLOGICALACCESS_API bool getChipInfoAt(unsigned int index, char* chipname, size_t chipnamelen, void** getterfct)
    {
        bool ret = false;
        if (chipname != NULL && chipnamelen == PLUGINOBJECT_MAXLEN && getterfct != NULL)
        {
            switch (index)
            {
            case 0:
            {
                *getterfct = (void*)&getDESFireChip;
                sprintf(chipname, CHIP_DESFIRE);
                ret = true;
            }
                break;

            case 1:
            {
                *getterfct = (void*)&getDESFireEV1Chip;
                sprintf(chipname, CHIP_DESFIRE_EV1);
                ret = true;
            }
                break;
            }
        }

        return ret;
    }
}