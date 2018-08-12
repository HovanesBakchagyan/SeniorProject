#include <pluginscards/mifareplus/mifareplussl1commands.hpp>
#include <logicalaccess/services/storage/storagecardservice.hpp>
#include "logicalaccess/dynlibrary/idynlibrary.hpp"
#include "logicalaccess/dynlibrary/librarymanager.hpp"
#include "logicalaccess/readerproviders/readerconfiguration.hpp"

#include "pluginscards/mifareplus/mifareplusaccessinfo_sl1.hpp"

#include "lla-tests/macros.hpp"
#include "lla-tests/utils.hpp"

void introduction()
{
    PRINT_TIME("Test the AES Authentication for a Mifare Plus in Security Level 1.");

    PRINT_TIME("You will have 20 seconds to insert a card. Test log below");
    PRINT_TIME("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

    LLA_SUBTEST_REGISTER("AES_Auth");
}

using namespace logicalaccess;


int main(int ac, char **av)
{
    prologue(ac, av);
    introduction();
    ReaderProviderPtr provider;
    ReaderUnitPtr readerUnit;
    ChipPtr chip;
    std::tie(provider, readerUnit, chip) = pcsc_test_init();

    PRINT_TIME("Chip identifier: " <<
               logicalaccess::BufferHelper::getHex(chip->getChipIdentifier()));
    PRINT_TIME("Chip type = " << chip->getCardType());

    auto mfp_sl1_cmd = std::dynamic_pointer_cast<MifarePlusSL1Commands>(chip->getCommands());
    LLA_ASSERT(mfp_sl1_cmd, "No (or invalid) command object.");

    std::shared_ptr<AES128Key> aes_key = std::make_shared<AES128Key>();
    aes_key->fromString("ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff");
	//aes_key->fromString("00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");

    LLA_ASSERT(mfp_sl1_cmd->AESAuthenticateSL1(aes_key), "AES Authentication failed");
    LLA_SUBTEST_PASSED("AES_Auth");

    pcsc_test_shutdown(readerUnit);
    return EXIT_SUCCESS;
}
