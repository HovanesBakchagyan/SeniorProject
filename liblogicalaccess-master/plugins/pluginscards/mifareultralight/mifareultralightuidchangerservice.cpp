#include "mifareultralightuidchangerservice.hpp"
#include "logicalaccess/logs.hpp"
#include "logicalaccess/myexception.hpp"
#include "mifareultralightlocation.hpp"

using namespace logicalaccess;

MifareUltralightUIDChangerService::MifareUltralightUIDChangerService(
    std::shared_ptr<StorageCardService> storage)
    : UIDChangerService(storage->getChip())
    , storage_(storage)
{
}

void MifareUltralightUIDChangerService::changeUID(
    const std::vector<uint8_t> &new_uid)
{
    auto loc = std::make_shared<MifareUltralightLocation>();
    EXCEPTION_ASSERT_WITH_LOG(new_uid.size() == 7, LibLogicalAccessException,
                              "Excepted 'new_uid' to have size 7.");

    // Page 0 contains 3 bytes of the 7 bytes UID.
    // Page 1 contains the last 4.
    auto uid_page0 = ByteVector(new_uid.begin(), new_uid.begin() + 3);
    uid_page0.push_back(0);
    auto uid_page1 = ByteVector(new_uid.begin() + 3, new_uid.end());

    loc->byte = 0;
    loc->page = 1;
    storage_->writeData(loc, nullptr, nullptr, uid_page1, CB_DEFAULT);

    loc->byte = 0;
    loc->page = 0;
    storage_->writeData(loc, nullptr, nullptr, uid_page0, CB_DEFAULT);
}
