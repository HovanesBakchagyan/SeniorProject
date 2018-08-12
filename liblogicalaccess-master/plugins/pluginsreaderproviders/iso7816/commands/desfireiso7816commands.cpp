/**
 * \file desfireiso7816commands.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief DESFire ISO7816 commands.
 */

#include "../commands/desfireiso7816commands.hpp"
#include "desfire/desfirechip.hpp"
#include "samav1iso7816commands.hpp"
#include "logicalaccess/cards/samkeystorage.hpp"
#include "desfire/nxpkeydiversification.hpp"
#include "desfire/nxpav1keydiversification.hpp"
#include "desfire/nxpav2keydiversification.hpp"
#include "samav2/samav2commands.hpp"

#include "logicalaccess/iks/IslogKeyServer.hpp"
#include "logicalaccess/settings.hpp"
#include "logicalaccess/iks/packet/DesfireAuth.hpp"
#include "logicalaccess/cards/IKSStorage.hpp"
#include "logicalaccess/iks/packet/DesfireChangeKey.hpp"
#include "logicalaccess/cards/computermemorykeystorage.hpp"
#include "samav2iso7816commands.hpp"

namespace logicalaccess
{
    DESFireISO7816Commands::DESFireISO7816Commands()
        : Commands(),
        DESFireCommands()
    {
        
    }

    DESFireISO7816Commands::~DESFireISO7816Commands()
    {
    }

    void DESFireISO7816Commands::erase()
    {
		std::vector<unsigned char> result = transmit(DF_INS_FORMAT_PICC);
		if (result.size() < 2 || result[result.size() - 2] != 0x91 || result[result.size() - 1] != 0x00)
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Erase command failed.");
    }

    void DESFireISO7816Commands::getVersion(DESFireCardVersion& dataVersion)
    {
	    auto result = transmit(DF_INS_GET_VERSION);

        if ((result.size() - 2) == 7)
        {
            if (result[result.size() - 1] == DF_INS_ADDITIONAL_FRAME)
            {
                memcpy(&dataVersion, &result[0], 7);

                result = transmit(DF_INS_ADDITIONAL_FRAME);

                if (result.size() - 2 == 7)
                {
                    if (result[result.size() - 1] == DF_INS_ADDITIONAL_FRAME)
                    {
                        memcpy(reinterpret_cast<char*>(&dataVersion) + 7, &result[0], 7);

                        result = transmit(DF_INS_ADDITIONAL_FRAME);
                        if (result.size() - 2 == 14)
                        {
                            if (result[result.size() - 1] == 0x00)
                            {
                                memcpy(reinterpret_cast<char*>(&dataVersion) + 14, &result[0], 14);
                            }
                        }
                    }
                }
            }
        }
    }

    void DESFireISO7816Commands::selectApplication(unsigned int aid)
    {
        std::vector<unsigned char> command;//, samaid;
        DESFireLocation::convertUIntToAid(aid, command);

        DESFireISO7816Commands::transmit(DF_INS_SELECT_APPLICATION, command);

        /* 
         * We directly select the keyentry to use so no need to select the app 
          if (getSAMChip())
         {
         LOG(LogLevel::INFOS) << "SelectApplication on SAM chip...");
         DESFireLocation::convertUIntToAid(aid, samaid);
         std::reverse(samaid.begin(), samaid.end());
         if (getSAMChip()->getCardType() == "SAM_AV1")
         std::dynamic_pointer_cast<SAMCommands<KeyEntryAV1Information, SETAV1> >(getSAMChip()->getCommands())->selectApplication(samaid);
         else if (getSAMChip()->getCardType() == "SAM_AV2")
         std::dynamic_pointer_cast<SAMCommands<KeyEntryAV2Information, SETAV2> >(getSAMChip()->getCommands())->selectApplication(samaid);
         }*/

        getDESFireChip()->getCrypto()->selectApplication(aid);
    }

    void DESFireISO7816Commands::createApplication(unsigned int aid, DESFireKeySettings settings, unsigned char maxNbKeys)
    {
        std::vector<unsigned char> command;

        DESFireLocation::convertUIntToAid(aid, command);
        command.push_back(static_cast<unsigned char>(settings));
        command.push_back(static_cast<unsigned char>(maxNbKeys));

        transmit(DF_INS_CREATE_APPLICATION, command, sizeof(command));
    }

    void DESFireISO7816Commands::deleteApplication(unsigned int aid)
    {
        std::vector<unsigned char> command;

        DESFireLocation::convertUIntToAid(aid, command);

        transmit(DF_INS_DELETE_APPLICATION, command, sizeof(command));
    }

    std::vector<unsigned int> DESFireISO7816Commands::getApplicationIDs()
    {
        std::vector<unsigned int> aids;
	    std::vector<unsigned char> result = transmit(DF_INS_GET_APPLICATION_IDS);

        while (result[result.size() - 1] == DF_INS_ADDITIONAL_FRAME)
        {
            for (size_t i = 0; i < result.size() - 2; i += 3)
            {
                std::vector<unsigned char> aid(result[i], result[i + 3]);
                aids.push_back(DESFireLocation::convertAidToUInt(aid));
            }

            result = transmit(DF_INS_ADDITIONAL_FRAME);
        }

        for (size_t i = 0; i < result.size() - 2; i += 3)
        {
            std::vector<unsigned char> aid(result.begin() + i, result.begin() + i + 3);
            aids.push_back(DESFireLocation::convertAidToUInt(aid));
        }

        return aids;
    }

    std::vector<unsigned char> DESFireISO7816Commands::getChangeKeySAMCryptogram(unsigned char keyno, std::shared_ptr<DESFireKey> key)
    {
        std::shared_ptr<SAMKeyStorage> samsks = std::dynamic_pointer_cast<SAMKeyStorage>(key->getKeyStorage());

		if (samsks && !getSAMChip())
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "SAMKeyStorage set on the key but no SAM reader has been set.");

        std::shared_ptr<SAMCommands<KeyEntryAV1Information, SETAV1> > samav1commands = std::dynamic_pointer_cast<SAMCommands<KeyEntryAV1Information, SETAV1>>(getSAMChip()->getCommands());
        std::shared_ptr<SAMCommands<KeyEntryAV2Information, SETAV2> > samav2commands = std::dynamic_pointer_cast<SAMCommands<KeyEntryAV2Information, SETAV2>>(getSAMChip()->getCommands());

		std::shared_ptr<DESFireCrypto> crypto = getDESFireChip()->getCrypto();
		std::shared_ptr<DESFireKey> oldkey = std::dynamic_pointer_cast<DESFireKey>(crypto->getKey(0, keyno));

        ChangeKeyInfo samck;
        memset(&samck, 0x00, sizeof(samck));
        samck.currentKeySlotNo = 0;
		samck.isMasterKey = (crypto->d_currentAid == 0 && keyno == 0x00) ? 1 : 0;
        samck.newKeySlotNo = samsks->getKeySlot();
        samck.newKeySlotV = key->getKeyVersion();
        samck.desfireNumber = keyno;

		if (oldkey && std::dynamic_pointer_cast<SAMKeyStorage>(oldkey->getKeyStorage()))
		{
			std::shared_ptr<SAMKeyStorage> oldsamks = std::dynamic_pointer_cast<SAMKeyStorage>(oldkey->getKeyStorage());
			samck.currentKeySlotNo = oldsamks->getKeySlot();
			samck.currentKeySlotV = oldkey->getKeyVersion();
		}
		else
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Current key required on SAM to change the key.");

		if ((crypto->d_currentKeyNo == 0 && keyno == 0) || (keyno == 0xE))
            samck.oldKeyInvolvement = 1;
        else
			samck.oldKeyInvolvement = 0;

        std::vector<unsigned char> diversify;
        if (key->getKeyDiversification())
			key->getKeyDiversification()->initDiversification(crypto->getIdentifier(), crypto->d_currentAid, key, keyno, diversify);
        if (oldkey->getKeyDiversification())
			oldkey->getKeyDiversification()->initDiversification(crypto->getIdentifier(), crypto->d_currentAid, oldkey, keyno, diversify);

        ChangeKeyDiversification keyDiv;
        memset(&keyDiv, 0x00, sizeof(keyDiv));
        if (std::dynamic_pointer_cast<NXPKeyDiversification>(key->getKeyDiversification())
            || std::dynamic_pointer_cast<NXPKeyDiversification>(oldkey->getKeyDiversification()))
        {
            std::vector<unsigned char> diversifyNew, diversifyOld;
            std::shared_ptr<KeyDiversification> nxpdiv = key->getKeyDiversification();
            std::shared_ptr<KeyDiversification> oldnxpdiv = oldkey->getKeyDiversification();

            if (nxpdiv)
            {
				nxpdiv->initDiversification(crypto->getIdentifier(), crypto->d_currentAid, key, keyno, diversifyNew);
                keyDiv.diversifyNew = 0x01;
                keyDiv.divInput = new unsigned char[diversifyNew.size()];
                memcpy(keyDiv.divInput, &diversifyNew[0], diversifyNew.size());
                keyDiv.divInputSize = static_cast<unsigned char>(diversifyNew.size());
            }

            if (oldnxpdiv)
            {
				oldnxpdiv->initDiversification(crypto->getIdentifier(), crypto->d_currentAid, key, keyno, diversifyOld);
				if (!keyDiv.divInput)
				{
					keyDiv.divInput = new unsigned char[diversifyOld.size()];
					memcpy(keyDiv.divInput, &diversifyOld[0], diversifyOld.size());
					keyDiv.divInputSize = static_cast<unsigned char>(diversifyOld.size());
				}

                if (nxpdiv && diversifyNew.size() != 0 && !std::equal(diversifyNew.begin(), diversifyNew.end(), diversifyOld.begin())
                    && typeid(*nxpdiv) != typeid(*oldnxpdiv))
                    THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Current and New Key should have the same system identifier and same NXP Div type.");

                keyDiv.diversifyCurrent = 0x01;
            }

            if (std::dynamic_pointer_cast<NXPAV2KeyDiversification>(nxpdiv)
				|| std::dynamic_pointer_cast<NXPAV2KeyDiversification>(oldnxpdiv))
                keyDiv.divType = NXPKeyDiversificationType::SAMAV2;
            else
                keyDiv.divType = NXPKeyDiversificationType::SAMAV1;
        }

		if (crypto->d_auth_method != CM_LEGACY)
		{
			//AES has keep IV option, need to load current IV
			if (samav1commands)
				samav1commands->loadInitVector(crypto->d_lastIV);
			else
				samav2commands->loadInitVector(crypto->d_lastIV);
		}

        std::vector<unsigned char> ret;
        if (samav1commands)
            ret = samav1commands->changeKeyPICC(samck, keyDiv);
        else
            ret = samav2commands->changeKeyPICC(samck, keyDiv);

        if (keyDiv.divInput != NULL)
            delete[] keyDiv.divInput;

		crypto->d_lastIV.clear();
		crypto->d_lastIV.resize(crypto->d_block_size);
		std::copy(ret.end() - crypto->d_block_size, ret.end(), crypto->d_lastIV.begin());
        return ret;
    }

	uint8_t DESFireISO7816Commands::getKeyVersion(uint8_t keyno)
	{
		std::vector<unsigned char> command;
		command.push_back(keyno);
		auto result = transmit(DF_INS_GET_KEY_VERSION, command);
		if (result.size() < 3)
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Invalid response for getKeyVersion.");
		return result[0];
	}

	ByteVector DESFireISO7816Commands::getKeyInformations(std::shared_ptr<DESFireKey> key, uint8_t keyno)
    {
		auto crypto = getDESFireChip()->getCrypto();
		auto samKeyStorage = std::dynamic_pointer_cast<SAMKeyStorage>(key->getKeyStorage());

		std::vector<unsigned char> diversify;
		if (key->getKeyDiversification())
			key->getKeyDiversification()->initDiversification(crypto->getIdentifier(), crypto->d_currentAid, key, keyno, diversify);

		// get key value from SAM - But what to do if the initDiversification need the key ?
		if (samKeyStorage && samKeyStorage->getDumpKey())
			getKeyFromSAM(key, diversify);

		return diversify;
    }

    void DESFireISO7816Commands::changeKey(unsigned char keyno, std::shared_ptr<DESFireKey> newkey)
    {
		std::shared_ptr<DESFireCrypto> crypto = getDESFireChip()->getCrypto();
        std::vector<unsigned char> cryptogram;

		std::shared_ptr<DESFireKey> key = std::make_shared<DESFireKey>(*newkey);
		auto oldkey = crypto->getKey(0, keyno);

		auto oldSamKeyStorage = std::dynamic_pointer_cast<SAMKeyStorage>(oldkey->getKeyStorage());
		auto newSamKeyStorage = std::dynamic_pointer_cast<SAMKeyStorage>(key->getKeyStorage());
		if (((oldSamKeyStorage && !oldSamKeyStorage->getDumpKey()) && !newSamKeyStorage)
			|| (!oldSamKeyStorage && (newSamKeyStorage && !newSamKeyStorage->getDumpKey())))
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Both keys need to be set in the SAM.");

		auto oldKeyDiversify = getKeyInformations(oldkey, keyno);
		auto newKeyDiversify = getKeyInformations(key, keyno);

		if (oldSamKeyStorage && !oldSamKeyStorage->getDumpKey())
        {
            cryptogram = getChangeKeySAMCryptogram(keyno, key);
        }
        else if (key->getKeyStorage()->getType() == KST_SERVER)
        {
            cryptogram = getChangeKeyIKSCryptogram(keyno, key);
        }
        else
        {
			cryptogram = crypto->changeKey_PICC(keyno, oldKeyDiversify, key, newKeyDiversify);
        }

        std::vector<unsigned char> command;
        command.push_back(keyno);
        if (cryptogram.size() > 0)
        {
            command.insert(command.end(), cryptogram.begin(), cryptogram.end());
            transmit(DF_INS_CHANGE_KEY, command);
			crypto->setKey(crypto->d_currentAid, 0, keyno, newkey);
        }
    }

    std::vector<unsigned char> DESFireISO7816Commands::getFileIDs()
    {
        std::vector<unsigned char> result = transmit(DF_INS_GET_FILE_IDS);
        std::vector<unsigned char> files;

        for (size_t i = 0; i < result.size() - 2; ++i)
        {
            files.push_back(result[i]);
        }

        return files;
    }

    void DESFireISO7816Commands::getKeySettings(DESFireKeySettings& settings, unsigned char& maxNbKeys)
    {
        std::vector<unsigned char> result;

        result = transmit(DF_INS_GET_KEY_SETTINGS);

        EXCEPTION_ASSERT_WITH_LOG(result.size() >= 2, LibLogicalAccessException, "incorrect result for transmit");

        settings = static_cast<DESFireKeySettings>(result[0]);
        maxNbKeys = result[1];
    }

    void DESFireISO7816Commands::changeKeySettings(DESFireKeySettings settings)
    {
        unsigned char command[1];
        command[0] = static_cast<unsigned char>(settings);
		std::vector<unsigned char> cryptogram = getDESFireChip()->getCrypto()->desfireEncrypt(std::vector<unsigned char>(command, command + sizeof(command)));
        transmit(DF_INS_CHANGE_KEY_SETTINGS, cryptogram);
    }

    void DESFireISO7816Commands::getFileSettings(unsigned char fileno, FileSetting& fileSetting)
    {
        std::vector<unsigned char> command;
        command.push_back(fileno);

        std::vector<unsigned char> result = transmit(DF_INS_GET_FILE_SETTINGS, command);
        memcpy(&fileSetting, &result[0], result.size() - 2);
    }

    std::vector<unsigned char> DESFireISO7816Commands::handleReadData(unsigned char err, const std::vector<unsigned char>& firstMsg, unsigned int length, EncryptionMode mode)
    {
		std::shared_ptr<DESFireCrypto> crypto = getDESFireChip()->getCrypto();
        std::vector<unsigned char> ret, data;

        if ((err == DF_INS_ADDITIONAL_FRAME || err == 0x00))
        {
            if (mode == CM_ENCRYPT)
            {
				crypto->initBuf();
				crypto->appendDecipherData(firstMsg);
            }
            else
            {
                ret = data = firstMsg;
                if (mode == CM_MAC)
                {
					crypto->initBuf();
                }
            }
        }

        while (err == DF_INS_ADDITIONAL_FRAME)
        {
            if (mode == CM_MAC)
            {
				if (!crypto->verifyMAC(false, data))
                {
                    THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "MAC data doesn't match.");
                }
            }

            data = transmit(DF_INS_ADDITIONAL_FRAME);
            err = data.back();
            data.resize(data.size() - 2);

            if (mode == CM_ENCRYPT)
            {
				crypto->appendDecipherData(data);
            }
            else
            {
                ret.insert(ret.end(), data.begin(), data.end());
            }
        }

        switch (mode)
        {
        case CM_PLAIN:
        {
        }
            break;

        case CM_MAC:
        {
			if (!crypto->verifyMAC(true, data))
            {
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "MAC data doesn't match.");
            }
            ret.resize(ret.size() - 4);
        }
            break;
        case CM_ENCRYPT:
        {
			ret = crypto->desfireDecrypt(length);
        }
            break;
        case CM_UNKNOWN:
        {
        }
            break;
        }

        return ret;
    }

	void DESFireISO7816Commands::handleWriteData(unsigned char cmd, const std::vector<unsigned char>& parameters, const std::vector<unsigned char>& data, EncryptionMode mode)
	{
		std::shared_ptr<DESFireCrypto> crypto = getDESFireChip()->getCrypto();

		crypto->initBuf();

		std::vector<unsigned char> cmdBuffer;
		cmdBuffer.insert(cmdBuffer.end(), parameters.begin(), parameters.end());

		switch (mode)
		{
		case CM_PLAIN:
		{
			cmdBuffer.insert(cmdBuffer.end(), data.begin(), data.end());
		}
		break;

		case CM_MAC:
		{
			std::vector<unsigned char> mac = crypto->generateMAC(cmd, data);
			cmdBuffer.insert(cmdBuffer.end(), data.begin(), data.end());
			cmdBuffer.insert(cmdBuffer.end(), mac.begin(), mac.end());
		}
		break;

		case CM_ENCRYPT:
		{
			auto edata = crypto->desfireEncrypt(data);
			cmdBuffer.insert(cmdBuffer.end(), edata.begin(), edata.end());
		}
		break;

		default:
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Unknown crypto mode.");
		}

		unsigned char err;
		size_t pos = 0;
		do
		{
			//Send the data little by little
			size_t pkSize = ((cmdBuffer.size() - pos) >= (DESFIRE_CLEAR_DATA_LENGTH_CHUNK)) ? (DESFIRE_CLEAR_DATA_LENGTH_CHUNK) : (cmdBuffer.size() - pos);

			auto command = std::vector<unsigned char>(cmdBuffer.begin() + pos, cmdBuffer.begin() + pos + pkSize);

			if (command.size() == 0)
				THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Data requested but no more data to send.");

			auto result = transmit(pos == 0 ? cmd : DF_INS_ADDITIONAL_FRAME, command);

			err = result.back();
			pos += pkSize;
		} while (err == DF_INS_ADDITIONAL_FRAME);

		if (err != 0x00)
		{
			char msgtmp[64];
			sprintf(msgtmp, "Unknown error: %x", err);
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, msgtmp);
		}
	}

    void DESFireISO7816Commands::changeFileSettings(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, bool plain)
    {
        std::vector<unsigned char> command;
        short ar = AccessRightsInMemory(accessRights);

        command.push_back(static_cast<unsigned char>(comSettings));
        BufferHelper::setUShort(command, ar);

        if (!plain)
        {
            std::vector<unsigned char> param;
            param.push_back(DF_INS_CHANGE_FILE_SETTINGS);
            param.push_back(static_cast<unsigned char>(fileno));
			command = getDESFireChip()->getCrypto()->desfireEncrypt(command, param);
        }
        unsigned char fc = static_cast<unsigned char>(fileno);
        command.insert(command.begin(), &fc, &fc + 1);

        transmit(DF_INS_CHANGE_FILE_SETTINGS, command);
    }

    void DESFireISO7816Commands::createStdDataFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int fileSize)
    {
        std::vector<unsigned char> command;
        short ar = AccessRightsInMemory(accessRights);

        command.push_back(fileno);
        command.push_back(static_cast<unsigned char>(comSettings));
        BufferHelper::setUShort(command, ar);
        command.push_back(static_cast<unsigned char>(fileSize & 0xff));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(fileSize & 0xff00) >> 8));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(fileSize & 0xff0000) >> 16));

        transmit(DF_INS_CREATE_STD_DATA_FILE, command);
    }

    void DESFireISO7816Commands::createBackupFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int fileSize)
    {
        std::vector<unsigned char> command;
        short ar = AccessRightsInMemory(accessRights);

        command.push_back(fileno);
        command.push_back(static_cast<unsigned char>(comSettings));
        BufferHelper::setUShort(command, ar);
        command.push_back(static_cast<unsigned char>(fileSize & 0xff));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(fileSize & 0xff00) >> 8));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(fileSize & 0xff0000) >> 16));

        transmit(DF_INS_CREATE_BACKUP_DATA_FILE, command);
    }

    void DESFireISO7816Commands::createValueFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int lowerLimit, unsigned int upperLimit, unsigned int value, bool limitedCreditEnabled)
    {
        std::vector<unsigned char> command;
        short ar = AccessRightsInMemory(accessRights);

        command.push_back(fileno);
        command.push_back(static_cast<unsigned char>(comSettings));
        BufferHelper::setUShort(command, ar);
        BufferHelper::setInt32(command, static_cast<int>(lowerLimit));
        BufferHelper::setInt32(command, static_cast<int>(upperLimit));
        BufferHelper::setInt32(command, value);
        command.push_back(limitedCreditEnabled ? 0x01 : 0x00);

        transmit(DF_INS_CREATE_VALUE_FILE, command);
    }

    void DESFireISO7816Commands::createLinearRecordFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int fileSize, unsigned int maxNumberOfRecords)
    {
        std::vector<unsigned char> command;
        short ar = AccessRightsInMemory(accessRights);

        command.push_back(fileno);
        command.push_back(static_cast<unsigned char>(comSettings));
        BufferHelper::setUShort(command, ar);
        command.push_back(static_cast<unsigned char>(fileSize & 0xff));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(fileSize & 0xff00) >> 8));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(fileSize & 0xff0000) >> 16));
        command.push_back(static_cast<unsigned char>(maxNumberOfRecords & 0xff));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(maxNumberOfRecords & 0xff00) >> 8));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(maxNumberOfRecords & 0xff0000) >> 16));

        transmit(DF_INS_CREATE_LINEAR_RECORD_FILE, command);
    }

    void DESFireISO7816Commands::createCyclicRecordFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int fileSize, unsigned int maxNumberOfRecords)
    {
        std::vector<unsigned char> command;
        short ar = AccessRightsInMemory(accessRights);

        command.push_back(static_cast<unsigned char>(fileno));
        command.push_back(static_cast<unsigned char>(comSettings));
        BufferHelper::setUShort(command, ar);
        command.push_back(static_cast<unsigned char>(fileSize & 0xff));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(fileSize & 0xff00) >> 8));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(fileSize & 0xff0000) >> 16));
        command.push_back(static_cast<unsigned char>(maxNumberOfRecords & 0xff));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(maxNumberOfRecords & 0xff00) >> 8));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(maxNumberOfRecords & 0xff0000) >> 16));

        transmit(DF_INS_CREATE_CYCLIC_RECORD_FILE, command);
    }

    void DESFireISO7816Commands::deleteFile(unsigned char fileno)
    {
        std::vector<unsigned char> command;

        command.push_back(fileno);

        transmit(DF_INS_DELETE_FILE, command);
    }

    std::vector<unsigned char> DESFireISO7816Commands::readData(unsigned char fileno, unsigned int offset, unsigned int length, EncryptionMode mode)
    {
        std::vector<unsigned char> command, ret;

        command.push_back(fileno);

        // Currently we have some problems to read more than 253 bytes with an Omnikey Reader.
        // So the read command is separated to some commands, 8 bytes aligned.

        for (size_t i = 0; i < length; i += 248)
        {
            size_t trunloffset = offset + i;
            size_t trunklength = ((length - i) > 248) ? 248 : (length - i);
            command.push_back(static_cast<unsigned char>(trunloffset & 0xff));
            command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(trunloffset & 0xff00) >> 8));
            command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(trunloffset & 0xff0000) >> 16));
            command.push_back(static_cast<unsigned char>(trunklength & 0xff));
            command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(trunklength & 0xff00) >> 8));
            command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(trunklength & 0xff0000) >> 16));

            std::vector<unsigned char> result = transmit(DF_INS_READ_DATA, command);
            unsigned char err = result.back();
            result.resize(result.size() - 2);
            result = handleReadData(err, result, static_cast<unsigned int>(trunklength), mode);
            ret.insert(ret.end(), result.begin(), result.end());
        }

        return ret;
    }

    void DESFireISO7816Commands::writeData(unsigned char fileno, unsigned int offset, const std::vector<unsigned char>& data, EncryptionMode mode)
    {
		std::vector<unsigned char> parameters(7);
        parameters[0] = static_cast<unsigned char>(fileno);
        parameters[1] = static_cast<unsigned char>(offset & 0xff);
        parameters[2] = static_cast<unsigned char>(static_cast<unsigned short>(offset & 0xff00) >> 8);
        parameters[3] = static_cast<unsigned char>(static_cast<unsigned int>(offset & 0xff0000) >> 16);
        parameters[4] = static_cast<unsigned char>(data.size() & 0xff);
        parameters[5] = static_cast<unsigned char>(static_cast<unsigned short>(data.size() & 0xff00) >> 8);
        parameters[6] = static_cast<unsigned char>(static_cast<unsigned int>(data.size() & 0xff0000) >> 16);

        handleWriteData(DF_INS_WRITE_DATA, parameters, data, mode);
    }

    void DESFireISO7816Commands::getValue(unsigned char fileno, EncryptionMode mode, unsigned int& value)
    {
        std::vector<unsigned char> command;
        command.push_back(fileno);

        std::vector<unsigned char> result = transmit(DF_INS_GET_VALUE, command);
        unsigned char err = result.back();
        result.resize(result.size() - 2);
        result = handleReadData(err, result, 4, mode);

        if (result.size() >= 4)
        {
            size_t offset = 0;
            value = BufferHelper::getUInt32(result, offset);
        }
    }

    void DESFireISO7816Commands::credit(unsigned char fileno, unsigned int value, EncryptionMode mode)
    {
		std::vector<unsigned char> parameters(1);
        parameters[0] = fileno;
        uint32_t dataValue = static_cast<uint32_t>(value);
        std::vector<unsigned char> data(sizeof(dataValue));
        memcpy(&data[0], &dataValue, sizeof(dataValue));
        handleWriteData(DF_INS_CREDIT, parameters, data, mode);
    }

    void DESFireISO7816Commands::debit(unsigned char fileno, unsigned int value, EncryptionMode mode)
    {
		std::vector<unsigned char> parameters(1);
        parameters[0] = fileno;
        uint32_t dataValue = static_cast<uint32_t>(value);
        std::vector<unsigned char> data(sizeof(dataValue));
        memcpy(&data[0], &dataValue, sizeof(dataValue));
        handleWriteData(DF_INS_DEBIT, parameters, data, mode);
    }

    void DESFireISO7816Commands::limitedCredit(unsigned char fileno, unsigned int value, EncryptionMode mode)
    {
		std::vector<unsigned char> parameters(1);
        parameters[0] = static_cast<unsigned char>(fileno);
        uint32_t dataValue = static_cast<uint32_t>(value);
        std::vector<unsigned char> data(sizeof(dataValue));
        memcpy(&data[0], &dataValue, sizeof(dataValue));
        handleWriteData(DF_INS_LIMITED_CREDIT, parameters, data, mode);
    }

    void DESFireISO7816Commands::writeRecord(unsigned char fileno, unsigned int offset, const std::vector<unsigned char>& data, EncryptionMode mode)
    {
		std::vector<unsigned char> parameters(7);
        parameters[0] = fileno;
        parameters[1] = static_cast<unsigned char>(offset & 0xff);
        parameters[2] = static_cast<unsigned char>(static_cast<unsigned short>(offset & 0xff00) >> 8);
        parameters[3] = static_cast<unsigned char>(static_cast<unsigned int>(offset & 0xff0000) >> 16);
        parameters[4] = static_cast<unsigned char>(data.size() & 0xff);
        parameters[5] = static_cast<unsigned char>(static_cast<unsigned short>(data.size() & 0xff00) >> 8);
        parameters[6] = static_cast<unsigned char>(static_cast<unsigned int>(data.size() & 0xff0000) >> 16);

        handleWriteData(DF_INS_WRITE_RECORD, parameters, data, mode);
    }

    std::vector<unsigned char> DESFireISO7816Commands::readRecords(unsigned char fileno, unsigned int offset, unsigned int length, EncryptionMode mode)
    {
        std::vector<unsigned char> command, ret;

        command.push_back(fileno);
        command.push_back(static_cast<unsigned char>(offset & 0xff));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(offset & 0xff00) >> 8));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(offset & 0xff0000) >> 16));
        command.push_back(static_cast<unsigned char>(length & 0xff));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned short>(length & 0xff00) >> 8));
        command.push_back(static_cast<unsigned char>(static_cast<unsigned int>(length & 0xff0000) >> 16));

        std::vector<unsigned char> result = transmit(DF_INS_READ_RECORDS, command);
        unsigned char err = result.back();
        result.resize(result.size() - 2);
        result = handleReadData(err, result, length, mode);
		ret.insert(ret.end(), result.begin(), result.end());

        return ret;
    }

    void DESFireISO7816Commands::clearRecordFile(unsigned char fileno)
    {
        std::vector<unsigned char> command;
        command.push_back(fileno);

        transmit(DF_INS_CLEAR_RECORD_FILE, command);
    }

    void DESFireISO7816Commands::commitTransaction()
    {
        transmit(DF_COMMIT_TRANSACTION);
    }

    void DESFireISO7816Commands::abortTransaction()
    {
        transmit(DF_INS_ABORT_TRANSACTION);
    }

    void DESFireISO7816Commands::authenticate(unsigned char keyno)
    {
		std::shared_ptr<DESFireKey> key = getDESFireChip()->getCrypto()->getKey(0, keyno);
        authenticate(keyno, key);
    }

	void DESFireISO7816Commands::getKeyFromSAM(std::shared_ptr<DESFireKey> key, std::vector<unsigned char> diversify)
	{
		auto samKeyStorage = std::dynamic_pointer_cast<SAMKeyStorage>(key->getKeyStorage());

		if (samKeyStorage
			&& samKeyStorage->getDumpKey()
			&& (!getSAMChip() || getSAMChip()->getCardType() != "SAM_AV2"))
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "SAMKeyStorage Dump option can only be used with SAM AV2.");

		if (key->getKeyDiversification() && !std::dynamic_pointer_cast<NXPAV2KeyDiversification>(key->getKeyDiversification()))
			diversify.clear(); // We ignore diversification if wrong type
		key->setData(std::dynamic_pointer_cast<SAMAV2Commands<KeyEntryAV2Information, SETAV2>>(getSAMChip()->getCommands())->dumpSecretKey(samKeyStorage->getKeySlot(),
			key->getKeyVersion(), diversify));
		key->setKeyStorage(std::make_shared<ComputerMemoryKeyStorage>());

		// Clear diversification since already done
		if (key->getKeyDiversification() && std::dynamic_pointer_cast<NXPAV2KeyDiversification>(key->getKeyDiversification()))
			key->setKeyDiversification(nullptr);
	}

	ByteVector DESFireISO7816Commands::sam_authenticate_p1(std::shared_ptr<DESFireKey> key, ByteVector rndb, ByteVector diversify)
    {
		unsigned char p1 = 0x00;
		ByteVector data(2);
		data[0] = std::dynamic_pointer_cast<SAMKeyStorage>(key->getKeyStorage())->getKeySlot();
		data[1] = key->getKeyVersion();
		data.insert(data.end(), rndb.begin(), rndb.end());

		if (std::dynamic_pointer_cast<NXPKeyDiversification>(key->getKeyDiversification()))
		{
			p1 |= 0x01;
			if (std::dynamic_pointer_cast<NXPAV2KeyDiversification>(key->getKeyDiversification()))
				p1 |= 0x10;
			data.insert(data.end(), diversify.begin(), diversify.end());
		}

		ByteVector cmd_vector = { 0x80, 0x0a, p1, 0x00, static_cast<unsigned char>(data.size()), 0x00 };
		cmd_vector.insert(cmd_vector.end() - 1, data.begin(), data.end());

		int trytoreconnect = 0;
		ByteVector apduresult;
		do
		{
			try
			{
				if (getSAMChip()->getCardType() == "SAM_AV1")
					apduresult = std::dynamic_pointer_cast<SAMCommands<KeyEntryAV1Information, SETAV1>>(getSAMChip()->getCommands())->transmit(cmd_vector);
				else if (getSAMChip()->getCardType() == "SAM_AV2")
					apduresult = std::dynamic_pointer_cast<SAMCommands<KeyEntryAV2Information, SETAV2>>(getSAMChip()->getCommands())->transmit(cmd_vector, true, false);
				break;
			}
			catch (CardException& ex)
			{
				if (trytoreconnect > 5 || ex.error_code() != CardException::WRONG_P1_P2
					|| !std::dynamic_pointer_cast<NXPKeyDiversification>(key->getKeyDiversification()))
					std::rethrow_exception(std::current_exception());

				//SAM av2 often fail even if parameters are correct for during diversification av2 
				LOG(LogLevel::WARNINGS) << "try to auth with SAM P1: " << trytoreconnect;
				getSAMChip()->getCommands()->getReaderCardAdapter()->getDataTransport()->getReaderUnit()->reconnect();
			}
			++trytoreconnect;
		} while (true);

		if (apduresult.size() <= 2)
			THROW_EXCEPTION_WITH_LOG(CardException, "sam authenticate DES P1 failed.");

		return ByteVector(apduresult.begin(), apduresult.end() - 2);
    }

	void DESFireISO7816Commands::sam_authenticate_p2(unsigned char keyno, ByteVector rndap)
    {
		std::shared_ptr<DESFireCrypto> crypto = getDESFireChip()->getCrypto();
		ByteVector cmd_vector = { 0x80, 0x0a, 0x00, 0x00, static_cast<uint8_t>(rndap.size()) };
		cmd_vector.insert(cmd_vector.end(), rndap.begin(), rndap.end());
		ByteVector apduresult;
		if (getSAMChip()->getCardType() == "SAM_AV1")
			apduresult = std::dynamic_pointer_cast<SAMCommands<KeyEntryAV1Information, SETAV1>>(getSAMChip()->getCommands())->transmit(cmd_vector);
		else if (getSAMChip()->getCardType() == "SAM_AV2")
			apduresult = std::dynamic_pointer_cast<SAMCommands<KeyEntryAV2Information, SETAV2>>(getSAMChip()->getCommands())->transmit(cmd_vector, true, false);
		if (apduresult.size() != 2 || apduresult[0] != 0x90 || apduresult[1] != 0x00)
			THROW_EXCEPTION_WITH_LOG(CardException, "sam authenticate DES P2 failed.");

		if (getSAMChip()->getCardType() == "SAM_AV1")
			crypto->d_sessionKey = std::dynamic_pointer_cast<SAMCommands<KeyEntryAV1Information, SETAV1>>(getSAMChip()->getCommands())->dumpSessionKey();
		else if (getSAMChip()->getCardType() == "SAM_AV2")
			crypto->d_sessionKey = std::dynamic_pointer_cast<SAMCommands<KeyEntryAV2Information, SETAV2>>(getSAMChip()->getCommands())->dumpSessionKey();
		crypto->d_currentKeyNo = keyno;
    }

	void DESFireISO7816Commands::authenticate(unsigned char keyno, std::shared_ptr<DESFireKey> currentKey)
	{
		std::vector<unsigned char> command;

		std::shared_ptr<DESFireCrypto> crypto = getDESFireChip()->getCrypto();
		if (!currentKey) {
			currentKey = crypto->getDefaultKey(DF_KEY_DES);
		}
		std::shared_ptr<DESFireKey> key = std::make_shared<DESFireKey>(*currentKey);

		auto diversify = getKeyInformations(key, keyno);

		auto samKeyStorage = std::dynamic_pointer_cast<SAMKeyStorage>(key->getKeyStorage());
		if (samKeyStorage && !getSAMChip())
			THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "SAMKeyStorage set on the key but no SAM reader has been set.");

		crypto->setKey(crypto->d_currentAid, 0, keyno, key);

		command.push_back(keyno);

		std::vector<unsigned char> result = DESFireISO7816Commands::transmit(DF_INS_AUTHENTICATE, command);
		if (result.size() >= 2 && result[result.size() - 1] == DF_INS_ADDITIONAL_FRAME && (result.size() - 2) >= 8)
		{
			result.resize(8);
			std::vector<unsigned char> rndAB, apduresult;

			if (samKeyStorage)
				rndAB = sam_authenticate_p1(key, result, diversify);
			else
				rndAB = crypto->authenticate_PICC1(keyno, diversify, result);
			result = DESFireISO7816Commands::transmit(DF_INS_ADDITIONAL_FRAME, rndAB);
			if ((result.size() - 2) >= 8)
			{
				result.resize(result.size() - 2);

				if (samKeyStorage)
					sam_authenticate_p2(keyno, result);
				else
					crypto->authenticate_PICC2(keyno, result);
			}
		}
		else
			THROW_EXCEPTION_WITH_LOG(CardException, "DESFire authentication P1 failed.");
	}

    std::vector<unsigned char> DESFireISO7816Commands::transmit(unsigned char cmd, unsigned char lc)
    {
        return transmit(cmd, std::vector<unsigned char>(), lc, true);
    }

    std::vector<unsigned char> DESFireISO7816Commands::transmit(unsigned char cmd, const std::vector<unsigned char>& data, unsigned char lc, bool forceLc)
    {
        if (data.size())
        {
            return getISO7816ReaderCardAdapter()->sendAPDUCommand(DF_CLA_ISO_WRAP, cmd, 0x00, 0x00, static_cast<unsigned char>(data.size()), data, 0x00);
        }
        else if (forceLc)
        {
            return getISO7816ReaderCardAdapter()->sendAPDUCommand(DF_CLA_ISO_WRAP, cmd, 0x00, 0x00, lc, 0x00);
        }

        return getISO7816ReaderCardAdapter()->sendAPDUCommand(DF_CLA_ISO_WRAP, cmd, 0x00, 0x00, 0x00);
    }

    void DESFireISO7816Commands::setChip(std::shared_ptr<Chip> chip)
    {
        DESFireCommands::setChip(chip);
		getDESFireChip()->getCrypto()->setCryptoContext(chip->getChipIdentifier());
    }

    void DESFireISO7816Commands::iks_des_authenticate(unsigned char keyno,
                                                      std::shared_ptr<DESFireKey> key)
    {
        assert(key->getKeyType() == DF_KEY_DES && key->getKeyStorage()->getType() == KST_SERVER);

        /**
         * Todo handle key diversification !
         * Currently this is copy-paste from DESFireISO7816Commands::authenticate()
         */
        std::vector<unsigned char> command;
		std::shared_ptr<DESFireCrypto> crypto = getDESFireChip()->getCrypto();

        if (!key) {
			key = crypto->getDefaultKey(DF_KEY_DES);
        }
		crypto->setKey(crypto->d_currentAid, 0, keyno, key);

        std::vector<unsigned char> diversify;
        if (key->getKeyDiversification())
        {
			key->getKeyDiversification()->initDiversification(crypto->getIdentifier(), crypto->d_currentAid, key, keyno, diversify);
        }
        command.push_back(keyno);

        std::vector<unsigned char> result = DESFireISO7816Commands::transmit(DF_INS_AUTHENTICATE, command);
        if (result[result.size() - 1] == DF_INS_ADDITIONAL_FRAME && (result.size() - 2) >= 8)
        {
            result.resize(8);
            std::vector<unsigned char> rndAB;
            iks::IslogKeyServer &iks = iks::IslogKeyServer::fromGlobalSettings();

            iks::DesfireAuthCommand cmd;
            cmd.key_idt_ = std::static_pointer_cast<IKSStorage>(key->getKeyStorage())->getKeyIdentity();
            cmd.step_ = 1;
            cmd.algo_ = iks::DESFIRE_AUTH_ALGO_DES;
            memcpy(&cmd.data_[0], &result[0], result.size());

            iks.send_command(cmd);
            auto resp = std::dynamic_pointer_cast<iks::DesfireAuthResponse>(iks.recv());
            EXCEPTION_ASSERT_WITH_LOG(resp, IKSException, "Cannot retrieve proper response from server.");
            rndAB = std::vector<uint8_t>(resp->data_.begin(), resp->data_.begin() + 16);

            result = DESFireISO7816Commands::transmit(DF_INS_ADDITIONAL_FRAME, rndAB);
            if ((result.size() - 2) >= 8)
            {
                result.resize(result.size() - 2);
                cmd.step_ = 2;
                assert(result.size() == 8);
                assert(result.size() <= cmd.data_.max_size());
                memcpy(&cmd.data_[0], &result[0], result.size());
                iks.send_command(cmd);
                resp = std::dynamic_pointer_cast<iks::DesfireAuthResponse>(iks.recv());
                EXCEPTION_ASSERT_WITH_LOG(resp, IKSException, "Cannot retrieve proper response from server.");
                EXCEPTION_ASSERT_WITH_LOG(resp->success_, IKSException, "Mutual Authentication failed.");

				crypto->d_sessionKey.clear();
				crypto->d_sessionKey.resize(16);
				memcpy(&crypto->d_sessionKey[0], &resp->data_[0], 16);

				crypto->d_currentKeyNo = keyno;
				crypto->d_auth_method = CM_LEGACY;
				crypto->d_mac_size = 4;
            }
        }
    }

    std::vector<unsigned char> DESFireISO7816Commands::getChangeKeyIKSCryptogram(
            unsigned char keyno, std::shared_ptr<DESFireKey> key)
    {
        auto storage = std::dynamic_pointer_cast<IKSStorage>(key->getKeyStorage());
        assert(storage);

        iks::IslogKeyServer &key_server = iks::IslogKeyServer::fromGlobalSettings();
        iks::DesfireChangeKeyCommand cmd;
		std::shared_ptr<DESFireCrypto> crypto = getDESFireChip()->getCrypto();

        cmd.newkey_idt_ = storage->getKeyIdentity();
		auto old_key = crypto->getKey(0, keyno);
        auto old_key_storage = std::dynamic_pointer_cast<IKSStorage>(old_key->getKeyStorage());
        assert(old_key_storage);
        cmd.oldkey_idt_ = old_key_storage->getKeyIdentity();
		cmd.session_key_ = crypto->d_sessionKey;

		cmd.iv_ = crypto->d_lastIV;
        cmd.keyno_ = keyno;
		cmd.flag_ = ((keyno & 0x0F) == crypto->d_currentKeyNo) ?
                IKS_COMMAND_DESFIRE_CHANGEKEY_SAME_KEY :
                IKS_COMMAND_DESFIRE_CHANGEKEY_OTHER_KEY;
       cmd.oldkey_divinfo_ = iks::KeyDivInfo::build(old_key, getChip()->getChipIdentifier(),
		   keyno, crypto->d_currentAid);
       cmd.newkey_divinfo_ = iks::KeyDivInfo::build(key, getChip()->getChipIdentifier(),
		   keyno, crypto->d_currentAid);

        key_server.send_command(cmd);


        auto resp = std::dynamic_pointer_cast<iks::DesfireChangeKeyResponse>(key_server.recv());
        EXCEPTION_ASSERT_WITH_LOG(resp, IKSException, "Cannot retrieve a proper response from IKS.");
        EXCEPTION_ASSERT_WITH_LOG(resp->status_ == iks::SMSG_STATUS_SUCCESS,
                                  IKSException, "Cannot retrieve a proper response from IKS.");

            // When changing an AES key in "OTHER_KEY" mode
        if (cmd.flag_ & IKS_COMMAND_DESFIRE_CHANGEKEY_OTHER_KEY)
			crypto->d_lastIV = std::vector<unsigned char>(resp->bytes_.end() - 16, resp->bytes_.end());
        return resp->bytes_;
    }
}
