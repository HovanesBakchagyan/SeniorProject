#include "pluginscards/mifare/mifarelocation.hpp"
#include "logicalaccess/dynlibrary/idynlibrary.hpp"
#include "logicalaccess/dynlibrary/librarymanager.hpp"
#include "logicalaccess/readerproviders/readerconfiguration.hpp"
#include "logicalaccess/services/storage/storagecardservice.hpp"
#include "pluginscards/mifare/mifarechip.hpp"
#include "pluginscards/mifareplus/mifareplusaccessinfo_sl1.hpp"
#include "logicalaccess/cards/aes128key.hpp"

#include "logicalaccess/logs.hpp"
#include "lla-tests/macros.hpp"
#include "lla-tests/utils.hpp"

void introduction()
{
    PRINT_TIME("This test target MifarePlus cards in Security Level 1. Test the access storage service");

    PRINT_TIME("You will have 20 seconds to insert a card. Test log below");
    PRINT_TIME("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

    LLA_SUBTEST_REGISTER("WriteService");
    LLA_SUBTEST_REGISTER("ReadService");
    LLA_SUBTEST_REGISTER("CorrectWriteRead");
}

int main(int ac, char **av)
{
    using namespace logicalaccess;

    prologue(ac, av);
    introduction();
    ReaderProviderPtr provider;
    ReaderUnitPtr readerUnit;
    ChipPtr chip;
    std::tie(provider, readerUnit, chip) = pcsc_test_init();

    PRINT_TIME("Chip identifier: " <<
               logicalaccess::BufferHelper::getHex(chip->getChipIdentifier()));

    LLA_ASSERT(chip->getCardType() == "MifarePlus_SL1_4K",
               "Chip is not a MifarePlus_SL1_4K, but is " + chip->getCardType() +
               " instead.");

    auto storage = std::dynamic_pointer_cast<logicalaccess::StorageCardService>(
            chip->getService(logicalaccess::CST_STORAGE));

    // We want to write data on sector 1.
    std::shared_ptr<logicalaccess::MifareLocation> mlocation(
            new logicalaccess::MifareLocation());
    mlocation->sector = 4;
    mlocation->block = 0;


    // Key to perform AES auth in SL1.
    std::shared_ptr<logicalaccess::AES128Key> aes_key = std::make_shared<logicalaccess::AES128Key>();
    aes_key->fromString("ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff");

    // Key to use for sector authentication
    std::shared_ptr<logicalaccess::MifarePlusSL1AccessInfo> maiToUse(
            new logicalaccess::MifarePlusSL1AccessInfo());
    maiToUse->keyA->fromString("00 00 00 00 00 00");
    maiToUse->keyB->fromString("00 00 00 00 00 00");
    maiToUse->aesAuthKey = aes_key;
    maiToUse->sab.setAReadBWriteConfiguration();

    // Data to write
    std::vector<uint8_t> writedata(16, 'f');

    // Data read
    std::vector<uint8_t> readdata;

    storage->writeData(mlocation, maiToUse, maiToUse, writedata, logicalaccess::CB_DEFAULT);
    using namespace logicalaccess; // required for overload of std::ostrean(vector &)
    PRINT_TIME("Wrote: " << writedata);
    LLA_SUBTEST_PASSED("WriteService")

    const int mifare_block_size = 16;
    // We read the data on the same location. Remember, the key is now changed.
    readdata = storage
            ->readData(mlocation, maiToUse, mifare_block_size, logicalaccess::CB_DEFAULT);
    PRINT_TIME("Read: " << readdata);
    LLA_SUBTEST_PASSED("ReadService")

    LLA_ASSERT(std::equal(writedata.begin(), writedata.end(), readdata.begin()),
               "Data read is not what we wrote.");
    LLA_SUBTEST_PASSED("CorrectWriteRead");

    pcsc_test_shutdown(readerUnit);
    return 0;
}
