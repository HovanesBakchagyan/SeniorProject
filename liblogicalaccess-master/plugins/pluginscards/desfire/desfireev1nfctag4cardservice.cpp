/**
 * \file desfireev1nfctag4cardservice.cpp
 * \author Adrien J. <adrien-dev@islog.com>
 * \brief DESFireEV1 NFC Tag type 4 CardService.
 */

#include "desfireev1nfctag4cardservice.hpp"

namespace logicalaccess
{
    void DESFireEV1NFCTag4CardService::createNFCApplication(unsigned int aid, std::shared_ptr<logicalaccess::DESFireKey> masterPICCKey, unsigned short isoFIDApplication, unsigned short isoFIDCapabilityContainer, unsigned short isoFIDNDEFFile, unsigned short NDEFFileSize)
    {
        std::shared_ptr<logicalaccess::DESFireCommands> desfirecommand(std::dynamic_pointer_cast<logicalaccess::DESFireCommands>(getChip()->getCommands()));
        std::shared_ptr<logicalaccess::DESFireEV1Commands> desfireev1command(std::dynamic_pointer_cast<logicalaccess::DESFireEV1Commands>(getChip()->getCommands()));

        desfirecommand->selectApplication(0x00);
        if (masterPICCKey)
        {
            try
            {
                desfirecommand->authenticate(0x00, masterPICCKey);
            }
            catch (std::exception&) { }
        }

        unsigned char dfName[] = { 0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01 };
        std::vector<unsigned char> dfNameVector(dfName, dfName + 7);
        desfireev1command->createApplication(aid, logicalaccess::DESFireKeySettings::KS_DEFAULT, 1, logicalaccess::DESFireKeyType::DF_KEY_DES, logicalaccess::FidSupport::FIDS_ISO_FID, isoFIDApplication, dfNameVector);

        std::shared_ptr<logicalaccess::DESFireKey> key(new logicalaccess::DESFireKey("00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"));
        key->setKeyType(logicalaccess::DESFireKeyType::DF_KEY_DES);
        desfirecommand->selectApplication(aid);
        desfirecommand->authenticate(0x00, key);

        logicalaccess::DESFireAccessRights dar;
        dar.changeAccess = logicalaccess::TaskAccessRights::AR_KEY0;
        dar.readAccess = logicalaccess::TaskAccessRights::AR_FREE;
        dar.readAndWriteAccess = logicalaccess::TaskAccessRights::AR_KEY0;
        dar.writeAccess = logicalaccess::TaskAccessRights::AR_KEY0;

        desfireev1command->createStdDataFile(0x01, logicalaccess::EncryptionMode::CM_PLAIN, dar, 0x0f, isoFIDCapabilityContainer);
        writeCapabilityContainer(isoFIDCapabilityContainer, isoFIDNDEFFile, NDEFFileSize);

        dar.changeAccess = logicalaccess::TaskAccessRights::AR_KEY0;
        dar.readAccess = logicalaccess::TaskAccessRights::AR_FREE;
        dar.readAndWriteAccess = logicalaccess::TaskAccessRights::AR_FREE;
        dar.writeAccess = logicalaccess::TaskAccessRights::AR_FREE;

        desfireev1command->createStdDataFile(0x02, logicalaccess::EncryptionMode::CM_PLAIN, dar, 0xff, isoFIDNDEFFile);
    }

    void DESFireEV1NFCTag4CardService::deleteNFCApplication(unsigned int aid, std::shared_ptr<logicalaccess::DESFireKey> masterPICCKey)
    {
        std::shared_ptr<logicalaccess::DESFireCommands> desfirecommand(std::dynamic_pointer_cast<logicalaccess::DESFireCommands>(getChip()->getCommands()));

        desfirecommand->selectApplication(0x00);
        if (masterPICCKey)
        {
            desfirecommand->authenticate(0x00, masterPICCKey);
        }
        desfirecommand->deleteApplication(aid);
    }

    std::shared_ptr<logicalaccess::NdefMessage> DESFireEV1NFCTag4CardService::readNDEFFile(unsigned short isoFIDApplication, unsigned short isoFIDNDEFFile)
    {
        std::shared_ptr<logicalaccess::DESFireCommands> desfirecommand(std::dynamic_pointer_cast<logicalaccess::DESFireCommands>(getChip()->getCommands()));

        desfirecommand->selectApplication(0x00);
        return ISO7816NFCTag4CardService::readNDEFFile(isoFIDApplication, isoFIDNDEFFile);
    }

    void DESFireEV1NFCTag4CardService::writeNDEF(std::shared_ptr<logicalaccess::NdefMessage> records)
    {
        std::shared_ptr<logicalaccess::DESFireKey> picckey(new logicalaccess::DESFireKey("00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"));
        createNFCApplication(0x000001, picckey);
        writeNDEFFile(records);
    }

    void DESFireEV1NFCTag4CardService::eraseNDEF()
    {
        std::shared_ptr<logicalaccess::DESFireCommands> desfirecommand(std::dynamic_pointer_cast<logicalaccess::DESFireCommands>(getChip()->getCommands()));
        desfirecommand->erase();
    }
}