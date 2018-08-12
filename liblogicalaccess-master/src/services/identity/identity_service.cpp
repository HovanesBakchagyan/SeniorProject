#include "logicalaccess/services/identity/identity_service.hpp"

using namespace logicalaccess;


IdentityCardService::IdentityCardService(std::shared_ptr<Chip> chip)
	: CardService(chip, CST_IDENTITY)
{
}

void IdentityCardService::setAccessInfo(std::shared_ptr<AccessInfo> ai)
{
    access_info_ = ai;
}
