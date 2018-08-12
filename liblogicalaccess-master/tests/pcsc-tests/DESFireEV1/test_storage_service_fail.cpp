#include "logicalaccess/dynlibrary/idynlibrary.hpp"
#include "logicalaccess/dynlibrary/librarymanager.hpp"
#include "logicalaccess/readerproviders/readerconfiguration.hpp"
#include "logicalaccess/services/storage/storagecardservice.hpp"
#include "logicalaccess/services/accesscontrol/formats/wiegand26format.hpp"
#include "logicalaccess/services/accesscontrol/formats/wiegand37format.hpp"
#include "logicalaccess/services/accesscontrol/formats/customformat/numberdatafield.hpp"
#include "logicalaccess/readerproviders/serialportdatatransport.hpp"
#include "logicalaccess/services/accesscontrol/accesscontrolcardservice.hpp"

#include "pluginsreaderproviders/iso7816/commands/desfireev1iso7816commands.hpp"
#include "pluginscards/desfire/desfireev1commands.hpp"
#include "pluginscards/desfire/desfireev1chip.hpp"


#include "lla-tests/macros.hpp"
#include "lla-tests/utils.hpp"


/** This version of the tests fails.
 * Something is weird, need to investigate
 */

void introduction()
{
    prologue();
    PRINT_TIME("This test target DESFireEV1 cards. It test the storage service for writing and"
                       "reading data in a file.");

    PRINT_TIME("You will have 20 seconds to insert a card. Test log below");
    PRINT_TIME("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

    LLA_SUBTEST_REGISTER("WriteService");
    LLA_SUBTEST_REGISTER("ReadService");
    LLA_SUBTEST_REGISTER("CorrectWriteRead");
}

void create_app_and_file(std::shared_ptr<logicalaccess::DESFireISO7816Commands> cmd,
                         std::shared_ptr<logicalaccess::DESFireEV1ISO7816Commands> cmdev1)
{
    // create the application we wish to write into
    cmdev1->createApplication(0x534, logicalaccess::DESFireKeySettings::KS_DEFAULT, 3,
                              logicalaccess::DESFireKeyType::DF_KEY_AES,
                              logicalaccess::FIDS_NO_ISO_FID, 0, std::vector<unsigned char>());
    cmd->selectApplication(0x534);

    std::shared_ptr<logicalaccess::DESFireKey> key(new logicalaccess::DESFireKey());
    key->setKeyType(logicalaccess::DESFireKeyType::DF_KEY_AES);
    cmd->authenticate(0, key);

    // We can do everything with key1
    logicalaccess::DESFireAccessRights ar;
    ar.readAccess = logicalaccess::TaskAccessRights::AR_KEY0;
    ar.writeAccess = logicalaccess::TaskAccessRights::AR_KEY0;
    ar.readAndWriteAccess = logicalaccess::TaskAccessRights::AR_KEY0;
    ar.changeAccess = logicalaccess::TaskAccessRights::AR_KEY0;

    // Create the file we will use.
    int file_size = 16;
    cmdev1->createStdDataFile(0x00, logicalaccess::EncryptionMode::CM_ENCRYPT, ar, file_size, 0);
}

int main(int, char **)
{
    introduction();
    ReaderProviderPtr provider;
    ReaderUnitPtr readerUnit;
    ChipPtr chip;
    std::tie(provider, readerUnit, chip) = pcsc_test_init();

    PRINT_TIME("CHip identifier: " <<
               logicalaccess::BufferHelper::getHex(chip->getChipIdentifier()));

    LLA_ASSERT(chip->getCardType() == "DESFireEV1",
               "Chip is not an DESFireEV1, but is " + chip->getCardType() +
               " instead.");

    auto storage = std::dynamic_pointer_cast<logicalaccess::StorageCardService>(
            chip->getService(logicalaccess::CST_STORAGE));

    auto cmd = std::dynamic_pointer_cast<logicalaccess::DESFireISO7816Commands>(chip->getCommands());
    auto cmdev1 = std::dynamic_pointer_cast<logicalaccess::DESFireEV1ISO7816Commands>(chip->getCommands());

    std::shared_ptr<logicalaccess::Location> location;
    std::shared_ptr<logicalaccess::AccessInfo> aiToUse;

    // The excepted memory tree
    std::shared_ptr<logicalaccess::DESFireEV1Location> dlocation(new logicalaccess::DESFireEV1Location());

    // The Application ID to use
    dlocation->aid = 0x000534;
    // File 0 into this application
    dlocation->file = 0;
    // File communication requires encryption
    dlocation->securityLevel = logicalaccess::CM_ENCRYPT;
    dlocation->useEV1 = true;
    dlocation->cryptoMethod = logicalaccess::DF_KEY_AES;
    location = dlocation;

    std::shared_ptr<logicalaccess::DESFireAccessInfo> daiToUse(new logicalaccess::DESFireAccessInfo());
    daiToUse->masterCardKey->fromString("00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");

    //daiToUse->readKey->setKeyType(logicalaccess::DF_KEY_AES); // fix bug when using keyno = 0

    daiToUse->writeKeyno = 0;
    daiToUse->readKeyno = 0;
    aiToUse = daiToUse;

    cmd->selectApplication(0x00);
    cmd->authenticate(0);
    cmd->erase();

    create_app_and_file(cmd, cmdev1);

    // Data to write
    std::vector<uint8_t> writedata(16, 'd');
    std::vector<uint8_t> readdata;
    // Write data on the specified location with the specified key
    storage->writeData(location, aiToUse, aiToUse, writedata, logicalaccess::CB_DEFAULT);

    using namespace logicalaccess; // required for overload of std::ostrean(vector &)
    PRINT_TIME("Wrote: " << writedata);
    LLA_SUBTEST_PASSED("WriteService")

    // We read the data on the same location. Remember, the key is now changed.
    readdata = storage
            ->readData(location, aiToUse, 16, logicalaccess::CB_DEFAULT);
    PRINT_TIME("Read: " << readdata);
    LLA_SUBTEST_PASSED("ReadService")

    LLA_ASSERT(std::equal(writedata.begin(), writedata.end(), readdata.begin()),
               "Data read is not what we wrote.");
    LLA_SUBTEST_PASSED("CorrectWriteRead");

    cmd->authenticate(0);
    cmd->deleteFile(0x00);
    cmd->deleteApplication(0x534);

    pcsc_test_shutdown(readerUnit);
    return 0;
}
