/**
 * \file SAMAV1ISO7816Commands.hpp
 * \author Adrien J. <adrien.jund@islog.com>
 * \brief SAMAV1ISO7816Commands commands.
 */

#ifndef LOGICALACCESS_SAMISO7816CARDPROVIDER_HPP
#define LOGICALACCESS_SAMISO7816CARDPROVIDER_HPP

#include "samav2/samcommands.hpp"
#include "../readercardadapters/iso7816readercardadapter.hpp"
#include "../iso7816readerunitconfiguration.hpp"
#include "samav2/samcrypto.hpp"
#include "samav2/samkeyentry.hpp"
#include "samav2/samcrypto.hpp"
#include "samav2/samcommands.hpp"

#include <openssl/rand.h>
#include "logicalaccess/crypto/symmetric_key.hpp"
#include "logicalaccess/crypto/aes_symmetric_key.hpp"
#include "logicalaccess/crypto/aes_initialization_vector.hpp"
#include "logicalaccess/crypto/aes_cipher.hpp"
#include "logicalaccess/crypto/cmac.hpp"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/thread/thread_time.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "logicalaccess/myexception.hpp"

#define DEFAULT_SAM_CLA 0x80

namespace logicalaccess
{
    /**
     * \brief The SAMISO7816 commands class.
     */
    template <typename T, typename S>
    class LIBLOGICALACCESS_API SAMISO7816Commands : public virtual SAMCommands < T, S >
    {
    public:

        /**
         * \brief Constructor.
         */
        SAMISO7816Commands()
        {
            /*
            # Only one active MIFARE authentication at a time is supported by SAM AV2, so interleaved processing of the commands over differents LCs in parallel is not possible.
            d_named_mutex.reset(new boost::interprocess::named_mutex(boost::interprocess::open_or_create, "sam_mutex"));
            bool locked = d_named_mutex->timed_lock(boost::get_system_time() + boost::posix_time::seconds(5));

            boost::interprocess::shared_memory_object shm_obj(boost::interprocess::open_or_create, "sam_memory", boost::interprocess::read_write);
            boost::interprocess::offset_t size = 0;
            shm_obj.get_size(size);
            if (size != 4)
            shm_obj.truncate(4);

            d_region.reset(new boost::interprocess::mapped_region(shm_obj, boost::interprocess::read_write));

            char *addr = (char*)d_region->get_address();

            if (size != 4 || !locked)
            std::memset(addr, 0, d_region->get_size());

            unsigned char x = 0;
            for (; x < d_region->get_size(); ++x)
            {
            if (addr[x] == 0)
            {
            addr[x] = 1;
            break;
            }
            }

            d_named_mutex->unlock();

            if (x < d_region->get_size())
            d_cla = DEFAULT_SAM_CLA + x;
            else
            THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "No channel available.");*/
            d_cla = DEFAULT_SAM_CLA;
            d_LastSessionIV.resize(16);
        }

        /**
         * \brief Destructor.
         */
        virtual ~SAMISO7816Commands()
        {
            /*d_named_mutex->lock();
            char *addr = (char*)d_region->get_address();
            addr[d_cla - 0x80] = 0;
            d_named_mutex->unlock();

            if (!boost::interprocess::shared_memory_object::remove("sam_memory"))
            LOG(LogLevel::INFOS) << "SAM Shared Memory removed failed. It is probably still open by a process.");*/

            //we do not remove named_mutex - it can still be used by another process
        }

        std::shared_ptr<ISO7816ReaderCardAdapter> getISO7816ReaderCardAdapter() { return std::dynamic_pointer_cast<ISO7816ReaderCardAdapter>(this->getReaderCardAdapter()); };

        virtual std::vector<unsigned char> transmit(std::vector<unsigned char> cmd, bool first = true, bool last = true)
        {
            return getISO7816ReaderCardAdapter()->sendCommand(cmd);
        }

        virtual SAMVersion getVersion()
        {
            unsigned char cmd[] = { d_cla, 0x60, 0x00, 0x00, 0x00 };
            std::vector<unsigned char> cmd_vector(cmd, cmd + 5), result;
            SAMVersion	info;
            memset(&info, 0x00, sizeof(SAMVersion));

            result = transmit(cmd_vector);

            if (result.size() == 33 && result[31] == 0x90 && result[32] == 0x00)
            {
                memcpy(&info, &result[0], sizeof(info));

                /*if (info.hardware.vendorid == 0x04)
                    std::cout << "Vendor: NXP" << std::endl;
                    if (info.hardware.majorversion == 0x03)
                    std::cout << "Major version: T1AD2060" << std::endl;
                    if (info.hardware.minorversion == 0x04)
                    std::cout << "Major version: T1AR1070" << std::endl;
                    std::cout << "Storage size: " << (unsigned int)info.hardware.storagesize << std::endl;
                    std::cout << "Communication protocol type : " << (unsigned int)info.hardware.protocoltype << std::endl;*/
            }
            else
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "getVersion failed.");

            return info;
        }

        virtual std::vector<unsigned char> decipherData(std::vector<unsigned char> data, bool islastdata)
        {
            unsigned char p1 = 0x00;
            std::vector<unsigned char> datawithlength(3);

            if (!islastdata)
                p1 = 0xaf;
            else
            {
                datawithlength[0] = (unsigned char)(data.size() & 0xff0000);
                datawithlength[1] = (unsigned char)(data.size() & 0x00ff00);
                datawithlength[2] = (unsigned char)(data.size() & 0x0000ff);
            }
            datawithlength.insert(datawithlength.end(), data.begin(), data.end());

            unsigned char cmd[] = { d_cla, 0xdd, p1, 0x00, (unsigned char)(datawithlength.size()), 0x00 };
            std::vector<unsigned char> cmd_vector(cmd, cmd + 6), result;
            cmd_vector.insert(cmd_vector.end() - 1, datawithlength.begin(), datawithlength.end());

            result = transmit(cmd_vector);

            if (result.size() >= 2 && result[result.size() - 2] != 0x90 &&
                ((p1 == 0x00 && result[result.size() - 1] != 0x00) || (p1 == 0xaf && result[result.size() - 1] != 0xaf)))
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "decipherData failed.");

            return std::vector<unsigned char>(result.begin(), result.end() - 2);
        }

        virtual std::vector<unsigned char> encipherData(std::vector<unsigned char> data, bool islastdata)
        {
            unsigned char p1 = 0x00;

            if (!islastdata)
                p1 = 0xaf;
            unsigned char cmd[] = { d_cla, 0xed, p1, 0x00, (unsigned char)(data.size()), 0x00 };
            std::vector<unsigned char> cmd_vector(cmd, cmd + 6), result;
            cmd_vector.insert(cmd_vector.end() - 1, data.begin(), data.end());
            result = transmit(cmd_vector);

            if (result.size() >= 2 && result[result.size() - 2] != 0x90 &&
                ((p1 == 0x00 && result[result.size() - 1] != 0x00) || (p1 == 0xaf && result[result.size() - 1] != 0xaf)))
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "encipherData failed.");

            return std::vector<unsigned char>(result.begin(), result.end() - 2);
        }

        virtual void disableKeyEntry(unsigned char keyno)
        {
            unsigned char cmd[] = { d_cla, 0xd8, keyno, 0x00 };
            std::vector<unsigned char> cmd_vector(cmd, cmd + 4), result;

            result = transmit(cmd_vector);

            if (result.size() >= 2 && (result[result.size() - 2] != 0x90 || result[result.size() - 1] != 0x00))
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "disableKeyEntry failed.");
        }

        virtual std::vector<unsigned char> dumpSessionKey()
        {
            unsigned char cmd[] = { d_cla, 0xd5, 0x00, 0x00, 0x00 };
            std::vector<unsigned char> cmd_vector(cmd, cmd + 5), result;

            result = transmit(cmd_vector);

            if (result.size() >= 2 && (result[result.size() - 2] != 0x90 || result[result.size() - 1] != 0x00))
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "dumpSessionKey failed.");

            return std::vector<unsigned char>(result.begin(), result.end() - 2);
        }

		virtual void loadInitVector(std::vector<unsigned char> iv)
		{
			EXCEPTION_ASSERT_WITH_LOG((iv.size() == 0x08 || iv.size() == 0x10), LibLogicalAccessException,
				"loadInitVector need a 16 or 8 bytes vector");

			std::vector<unsigned char> loadInitVector = { 0x80, 0x71, 0x00, 0x00,
				static_cast<unsigned char>(iv.size()),
			};
			loadInitVector.insert(loadInitVector.end(), iv.begin(), iv.end());

			transmit(loadInitVector);
		}

        virtual std::string getSAMTypeFromSAM()
        {
            unsigned char cmd[] = { d_cla, 0x60, 0x00, 0x00, 0x00 };
            std::vector<unsigned char> cmd_vector(cmd, cmd + 5), result;

            result = transmit(cmd_vector);

            if (result.size() > 3)
            {
                if (result[result.size() - 3] == 0xA1)
                    return "SAM_AV1";
                else if (result[result.size() - 3] == 0xA2)
                    return "SAM_AV2";
            }
            return "SAM_NONE";
        }

        virtual std::shared_ptr<SAMDESfireCrypto> getCrypto() { return d_crypto; };
        virtual void setCrypto(std::shared_ptr<SAMDESfireCrypto> t) { d_crypto = t; };

        virtual void lockUnlock(std::shared_ptr<DESFireKey> masterKey, SAMLockUnlock state, unsigned char keyno, unsigned char unlockkeyno, unsigned char unlockkeyversion)
        {
            std::vector<unsigned char> result;
            unsigned char p1_part1 = state;
            unsigned int le = 2;

            std::vector<unsigned char> maxChainBlocks(3, 0x00); //MaxChainBlocks - unlimited

            std::vector<unsigned char> data_p1(2, 0x00);
            data_p1[0] = keyno;
            data_p1[1] = masterKey->getKeyVersion();

            if (state == SAMLockUnlock::SwitchAV2Mode)
            {
                le += 3;
                data_p1.insert(data_p1.end(), maxChainBlocks.begin(), maxChainBlocks.end());
            }
            else if (state == SAMLockUnlock::LockWithSpecifyingKey)
            {
                le += 2;
                data_p1.push_back(unlockkeyno);
                data_p1.push_back(unlockkeyversion);
            }

            result = this->getISO7816ReaderCardAdapter()->sendAPDUCommand(d_cla, 0x10, p1_part1, 0x00, le, data_p1, 0x00);
            if (result.size() != 14 || result[12] != 0x90 || result[13] != 0xAF)
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "lockUnlock P1 Failed.");

            std::vector<unsigned char> keycipher(masterKey->getData(), masterKey->getData() + masterKey->getLength());
            std::shared_ptr<openssl::OpenSSLSymmetricCipher> cipher(new openssl::AESCipher());
            std::vector<unsigned char> emptyIV(16), rnd1;

            /* Create rnd2 for p3 - CMAC: rnd2 | P2 | other data */
            std::vector<unsigned char>  rnd2(result.begin(), result.begin() + 12);
            rnd2.push_back(p1_part1); //P1_part1
            rnd2.insert(rnd2.end(), data_p1.begin() + 2, data_p1.end()); //last data

            /* ZeroPad */
            if (state == SAMLockUnlock::LockWithSpecifyingKey)
            {
                rnd2.push_back(0x00);
            }
            else if (state != SAMLockUnlock::SwitchAV2Mode)
            {
                rnd2.resize(rnd2.size() + 3);
            }

            std::vector<unsigned char> macHost = openssl::CMACCrypto::cmac(keycipher, cipher, 16, rnd2, emptyIV, 16);
            truncateMacBuffer(macHost);

            rnd1.resize(12);
            if (RAND_bytes(&rnd1[0], static_cast<int>(rnd1.size())) != 1)
            {
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Cannot retrieve cryptographically strong bytes");
            }

            std::vector<unsigned char> data_p2;
            data_p2.insert(data_p2.end(), macHost.begin(), macHost.begin() + 8);
            data_p2.insert(data_p2.end(), rnd1.begin(), rnd1.end());

            result = this->getISO7816ReaderCardAdapter()->sendAPDUCommand(d_cla, 0x10, 0x00, 0x00, 0x14, data_p2, 0x00);
            if (result.size() != 26 || result[24] != 0x90 || result[25] != 0xAF)
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "lockUnlock P2 Failed.");

            /* Check CMAC - Create rnd1 for p3 - CMAC: rnd1 | P1 | other data */
            rnd1.insert(rnd1.end(), rnd2.begin() + 12, rnd2.end()); //p2 data without rnd2

            macHost = openssl::CMACCrypto::cmac(keycipher, cipher, 16, rnd1, emptyIV, 16);
            truncateMacBuffer(macHost);

            for (unsigned char x = 0; x < 8; ++x)
            {
                if (macHost[x] != result[x])
                    THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "lockUnlock P2 CMAC from SAM is Wrong.");
            }

            /* Create kxe - d_authkey */
            generateAuthEncKey(keycipher, rnd1, rnd2);

            //create rndA
            std::vector<unsigned char> rndA(16);
            if (RAND_bytes(&rndA[0], static_cast<int>(rndA.size())) != 1)
            {
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "Cannot retrieve cryptographically strong bytes");
            }

            //decipher rndB
            std::shared_ptr<openssl::SymmetricKey> symkey(new openssl::AESSymmetricKey(openssl::AESSymmetricKey::createFromData(d_authKey)));
            std::shared_ptr<openssl::InitializationVector> iv(new openssl::AESInitializationVector(openssl::AESInitializationVector::createFromData(emptyIV)));

            std::vector<unsigned char> encRndB(result.begin() + 8, result.end() - 2);
            std::vector<unsigned char> dencRndB;

            cipher->decipher(encRndB, dencRndB, *symkey.get(), *iv.get(), false);

            //create rndB'
            std::vector<unsigned char> rndB1;
            rndB1.insert(rndB1.begin(), dencRndB.begin() + 2, dencRndB.begin() + dencRndB.size());
            rndB1.push_back(dencRndB[0]);
            rndB1.push_back(dencRndB[1]);

            std::vector<unsigned char> dataHost, encHost;
            dataHost.insert(dataHost.end(), rndA.begin(), rndA.end()); //RndA
            dataHost.insert(dataHost.end(), rndB1.begin(), rndB1.end()); //RndB'

            iv.reset(new openssl::AESInitializationVector(openssl::AESInitializationVector::createFromData(emptyIV)));
            cipher->cipher(dataHost, encHost, *symkey.get(), *iv.get(), false);

            result = this->getISO7816ReaderCardAdapter()->sendAPDUCommand(d_cla, 0x10, 0x00, 0x00, 0x20, encHost, 0x00);
            if (result.size() != 18 || result[16] != 0x90 || result[17] != 0x00)
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "lockUnlock P3 Failed.");

            std::vector<unsigned char> encSAMrndA(result.begin(), result.end() - 2), SAMrndA;
            iv.reset(new openssl::AESInitializationVector(openssl::AESInitializationVector::createFromData(emptyIV)));
            cipher->decipher(encSAMrndA, SAMrndA, *symkey.get(), *iv.get(), false);
            SAMrndA.insert(SAMrndA.begin(), SAMrndA.end() - 2, SAMrndA.end());

            if (!std::equal(SAMrndA.begin(), SAMrndA.begin() + 16, rndA.begin()))
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "lockUnlock P3 RndA from SAM is invalide.");
        }

        void selectApplication(std::vector<unsigned char> aid)
        {
            unsigned char cmd[] = { d_cla, 0x5a, 0x00, 0x00, 0x03 };
            std::vector<unsigned char> cmd_vector(cmd, cmd + 5), result;
            cmd_vector.insert(cmd_vector.end(), aid.begin(), aid.end());

            result = transmit(cmd_vector);

            if (result.size() >= 2 && (result[result.size() - 2] != 0x90 || result[result.size() - 1] != 0x00))
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, "selectApplication failed.");
        }

        std::vector<unsigned char> changeKeyPICC(const ChangeKeyInfo& info, const ChangeKeyDiversification& diversifycation)
        {
            unsigned char keyCompMeth = 0;

			keyCompMeth = info.oldKeyInvolvement;

            unsigned char cfg = info.desfireNumber & 0xf;
            if (info.isMasterKey)
                cfg |= 0x10;
            std::vector<unsigned char> data(4);
            data[0] = info.currentKeySlotNo;
            data[1] = info.currentKeySlotV;
            data[2] = info.newKeySlotNo;
            data[3] = info.newKeySlotV;

            if (diversifycation.divType != NXPKeyDiversificationType::NO_DIV)
            {
                if (diversifycation.divType == NXPKeyDiversificationType::SAMAV2)
                    keyCompMeth |= 0x20;

                keyCompMeth |= diversifycation.diversifyCurrent == 0x01 ? 0x04 : 0x00;
                keyCompMeth |= diversifycation.diversifyNew == 0x01 ? 0x02 : 0x00;

				data.insert(data.end(), diversifycation.divInput, diversifycation.divInput + diversifycation.divInputSize);
            }

            unsigned char cmd[] = { d_cla, 0xc4, keyCompMeth, cfg, (unsigned char)(data.size()), 0x00 };
            std::vector<unsigned char> cmd_vector(cmd, cmd + 6), result;
            cmd_vector.insert(cmd_vector.end() - 1, data.begin(), data.end());

            result = transmit(cmd_vector);

            if (result.size() >= 2 && (result[result.size() - 2] != 0x90 || result[result.size() - 1] != 0x00))
            {
                char tmp[64];
                sprintf(tmp, "changeKeyPICC failed (%x %x).", result[result.size() - 2], result[result.size() - 1]);
                THROW_EXCEPTION_WITH_LOG(LibLogicalAccessException, tmp);
            }

            return std::vector<unsigned char>(result.begin(), result.end() - 2);
        }

    protected:

        std::shared_ptr<SAMDESfireCrypto> d_crypto;

        //std::shared_ptr<boost::interprocess::mapped_region> d_region;

        //std::shared_ptr<boost::interprocess::named_mutex> d_named_mutex;

        unsigned char d_cla;

        std::vector<unsigned char> d_authKey;

        std::vector<unsigned char> d_sessionKey;

        std::vector<unsigned char> d_LastSessionIV;

        void truncateMacBuffer(std::vector<unsigned char>& data)
        {
            unsigned char truncateCount = 0;
            unsigned char count = 1;

            while (count < data.size())
            {
                data[truncateCount] = data[count];
                count += 2;
                ++truncateCount;
            }
        }

        void generateAuthEncKey(std::vector<unsigned char> keycipher, std::vector<unsigned char> rnd1, std::vector<unsigned char> rnd2)
        {
            std::vector<unsigned char> SV1a(16), emptyIV(16);

            std::copy(rnd1.begin() + 7, rnd1.begin() + 12, SV1a.begin());
            std::copy(rnd2.begin() + 7, rnd2.begin() + 12, SV1a.begin() + 5);
            std::copy(rnd1.begin(), rnd1.begin() + 5, SV1a.begin() + 10);

            for (unsigned char x = 0; x <= 4; ++x)
            {
                SV1a[x + 10] ^= rnd2[x];
            }

            SV1a[15] = 0x91; /* AES 128 */
            /* TODO AES 192 */

            std::shared_ptr<openssl::SymmetricKey> symkey(new openssl::AESSymmetricKey(openssl::AESSymmetricKey::createFromData(keycipher)));
            std::shared_ptr<openssl::InitializationVector> iv(new openssl::AESInitializationVector(openssl::AESInitializationVector::createFromData(emptyIV)));
            std::shared_ptr<openssl::OpenSSLSymmetricCipher> cipher(new openssl::AESCipher());

            cipher->cipher(SV1a, d_authKey, *symkey.get(), *iv.get(), false);
        }
    };
}

#endif /* LOGICALACCESS_SAMAV1ISO7816COMMANDS_HPP */