/**
 * \file topazstoragecardservice.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Topaz storage card service.
 */

#include <logicalaccess/logs.hpp>
#include "topazstoragecardservice.hpp"
#include "topazchip.hpp"
#include "topazaccessinfo.hpp"

namespace logicalaccess
{
    TopazStorageCardService::TopazStorageCardService(std::shared_ptr<Chip> chip)
        : StorageCardService(chip)
    {
    }

    TopazStorageCardService::~TopazStorageCardService()
    {
    }

    void TopazStorageCardService::erase(std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse)
    {
        std::shared_ptr<TopazLocation> mLocation = std::dynamic_pointer_cast<TopazLocation>(location);
        if (!mLocation)
        {
            return;
        }

        std::vector<unsigned char> zeroblock(8, 0x00);

        writeData(location, aiToUse, std::shared_ptr<AccessInfo>(), zeroblock, CB_DEFAULT);
    }

    void TopazStorageCardService::writeData(std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> /*aiToUse*/, std::shared_ptr<AccessInfo> aiToWrite, const std::vector<unsigned char>& data, CardBehavior behaviorFlags)
    {
        EXCEPTION_ASSERT_WITH_LOG(location, std::invalid_argument, "location cannot be null.");

        std::shared_ptr<TopazLocation> tLocation = std::dynamic_pointer_cast<TopazLocation>(location);
        EXCEPTION_ASSERT_WITH_LOG(tLocation, std::invalid_argument, "location must be a TopazLocation.");
        std::shared_ptr<TopazAccessInfo> tAi = std::dynamic_pointer_cast<TopazAccessInfo>(aiToWrite);

        size_t totaldatalen = data.size() + tLocation->byte;
        int nbPages = 0;
        size_t buflen = 0;
        while (buflen < totaldatalen)
        {
            buflen += 8;
            nbPages++;
        }

        if (nbPages >= 1)
        {
            std::vector<unsigned char> dataPages;
            dataPages.resize(buflen, 0x00);
			std::copy(data.begin(), data.end(), dataPages.begin() + tLocation->byte);

            if (behaviorFlags & CB_AUTOSWITCHAREA)
            {
                getTopazChip()->getTopazCommands()->writePages(tLocation->page,
                    tLocation->page + nbPages - 1, dataPages);
            }
            else
            {
                getTopazChip()->getTopazCommands()->writePage(tLocation->page, dataPages);
            }

            if (tAi && tAi->lockPage)
            {
                for (int i = tLocation->page; i < tLocation->page + nbPages; ++i)
                {
                    getTopazChip()->getTopazCommands()->lockPage(i);
                }
            }
        }
    }

    std::vector<unsigned char> TopazStorageCardService::readData(std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> /*aiToUse*/, size_t length, CardBehavior behaviorFlags)
    {
        EXCEPTION_ASSERT_WITH_LOG(location, std::invalid_argument, "location cannot be null.");
		std::vector<unsigned char> ret;
        std::shared_ptr<TopazLocation> tLocation = std::dynamic_pointer_cast<TopazLocation>(location);
        EXCEPTION_ASSERT_WITH_LOG(tLocation, std::invalid_argument, "location must be a TopazLocation.");

        size_t totaldatalen = length + tLocation->byte;
        int nbPages = 0;
        size_t buflen = 0;
        while (buflen < totaldatalen)
        {
            buflen += 8;
            nbPages++;
        }

        if (nbPages >= 1)
        {
            std::vector<unsigned char> dataPages;

            if (behaviorFlags & CB_AUTOSWITCHAREA)
            {
                dataPages = getTopazChip()->getTopazCommands()->readPages(tLocation->page, tLocation->page + nbPages - 1);
            }
            else
            {
                dataPages = getTopazChip()->getTopazCommands()->readPage(tLocation->page);
            }

			ret.insert(ret.end(), dataPages.begin() + tLocation->byte, dataPages.begin() + tLocation->byte + length);
        }
		return ret;
    }

    unsigned int TopazStorageCardService::readDataHeader(std::shared_ptr<Location> /*location*/, std::shared_ptr<AccessInfo> /*aiToUse*/, void* /*data*/, size_t /*dataLength*/)
    {
        return 0;
    }

    void TopazStorageCardService::erase()
    {
		std::vector<unsigned char> zeroblock(8, 0x00);
        for (unsigned int i = 1; i < getTopazChip()->getNbBlocks(); ++i)
        {
            // Don't try to write on Reserved/Lock/OTP bytes
            if (i != 13 && i != 14 && i != 15)
            {
                getTopazChip()->getTopazCommands()->writePage(i, zeroblock);
            }
        }
    }
}