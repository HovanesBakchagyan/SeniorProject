/**
 * \file desfireiso7816commands.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief DESFire commands.
 */

#ifndef LOGICALACCESS_DESFIREISO7816CARDPROVIDER_HPP
#define LOGICALACCESS_DESFIREISO7816CARDPROVIDER_HPP

#include "desfire/desfirecommands.hpp"
#include "desfire/desfirecrypto.hpp"
#include "../readercardadapters/iso7816readercardadapter.hpp"
#include "../iso7816readerunit.hpp"
#include "samav2/samchip.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
#define DESFIRE_CLEAR_DATA_LENGTH_CHUNK	32

    /**
     * \brief The DESFire base commands class.
     */
    class LIBLOGICALACCESS_API DESFireISO7816Commands : public virtual DESFireCommands
    {
    public:

        /**
         * \brief Constructor.
         */
        DESFireISO7816Commands();

        /**
         * \brief Destructor.
         */
        virtual ~DESFireISO7816Commands();

        /**
         * Override setChip() so we can steal the DESFireProfile and forward
         * it to the crypto context.
         */
        virtual void setChip(std::shared_ptr<Chip> chip) override;

        /**
         * \brief Erase the card.
         */
        virtual void erase();

        /**
         * \brief Select an application.
         * \param aid The Application ID
         */
        virtual void selectApplication(unsigned int aid);

        /**
         * \brief Create a new application.
         * \param aid The Application ID to create
         * \param settings Key settings
         * \param maxNbKeys Maximum number of keys
         */
        virtual void createApplication(unsigned int aid, DESFireKeySettings settings, unsigned char maxNbKeys);

        /**
         * \brief Delete an application.
         * \param aid The Application ID to delete
         */
        virtual void deleteApplication(unsigned int aid);

        /**
         * \brief Get the current application list.
         * \return The current application list.
         */
        virtual std::vector<unsigned int> getApplicationIDs();

        /**
         * \brief Change a key in the current application.
         * \param keyno The key number to change
         * \param key The new key
         */
        virtual void changeKey(unsigned char keyno, std::shared_ptr<DESFireKey> key);

        /**
         * \brief Get key settings on the current application.
         * \param settings Key settings
         * \param maxNbKeys Maximum number of keys
         */
        virtual void getKeySettings(DESFireKeySettings& settings, unsigned char& maxNbKeys);

		/**
		* \brief Get a key version of a key
		* \param keyno The key no.
		*/
		virtual uint8_t getKeyVersion(uint8_t keyno);

        /**
         * \brief Change key settings for the current application.
         * \param settings The new key settings
         */
        virtual void changeKeySettings(DESFireKeySettings settings);

        /**
         * \brief Get the File IDentifiers of all active files within the currently selected application
         * \return The file IDs.
         */
        virtual std::vector<unsigned char> getFileIDs();

        /**
         * \brief Get settings of a specific file in the current application.
         * \param fileno The file number
         * \param fileSetting The file setting
         */
        virtual void getFileSettings(unsigned char fileno, FileSetting& fileSetting);

        /**
         * \brief Change file settings of a specific file in the current application.
         * \param fileno The file number
         * \param comSettings The file communication setting
         * \param accessRights The file access rights
         * \param plain Communication is currently in plain data.
         */
        virtual void changeFileSettings(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, bool plain);

        /**
         * \brief Create a new data file in the current application.
         * \param fileno The new file number
         * \param comSettings The file communication setting
         * \param accessRights The file access rights
         * \param fileSize The file size.
         */
        virtual void createStdDataFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int fileSize);

        /**
         * \brief Create a new backup file in the current application.
         * \param fileno The new file number
         * \param comSettings The file communication setting
         * \param accessRights The file access rights
         * \param fileSize The file size.
         */
        virtual void createBackupFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int fileSize);

        /**
         * \brief Create a new value file in the current application.
         * \param fileno The new file number
         * \param comSettings The file communication setting
         * \param accessRights The file access rights
         * \param lowerLimit The file size (in bytes)
         * \param upperLimit The file size (in bytes)
         * \param value The default value
         * \param limitedCreditEnabled Set if the limited credit is enabled
         */
        virtual void createValueFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int lowerLimit, unsigned int upperLimit, unsigned int value, bool limitedCreditEnabled);

        /**
         * \brief Create a new linear record file in the current application.
         * \param fileno The new file number
         * \param comSettings The file communication setting
         * \param accessRights The file access rights
         * \param fileSize The file size.
         * \param maxNumberOfRecords Max number of records in the file.
         */
        virtual void createLinearRecordFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int fileSize, unsigned int maxNumberOfRecords);

        /**
         * \brief Create a new cyclic record file in the current application.
         * \param fileno The new file number
         * \param comSettings The file communication setting
         * \param accessRights The file access rights
         * \param fileSize The file size.
         * \param maxNumberOfRecords Max number of records in the file.
         */
        virtual void createCyclicRecordFile(unsigned char fileno, EncryptionMode comSettings, DESFireAccessRights accessRights, unsigned int fileSize, unsigned int maxNumberOfRecords);

        /**
         * \brief Delete a file in the current application.
         * \param fileno The file number to delete
         */
        virtual void deleteFile(unsigned char fileno);

        /**
         * \brief Read data from a specific file.
         * \param fileno The file number
         * \param offset The byte offset
         * \param length The data length to read
         * \param mode The communication mode
         * \return The bytes readed.
         */
        virtual std::vector<unsigned char> readData(unsigned char fileno, unsigned int offset, unsigned int length, EncryptionMode mode);

        /**
         * \brief Write data into a specific file.
         * \param fileno The file number
         * \param offset The byte offset
         * \param data The data buffer
         * \param mode The communication mode
         */
        virtual void writeData(unsigned char fileno, unsigned int offset, const std::vector<unsigned char>& data, EncryptionMode mode);

        /**
         * \brief Get value from a specific value file.
         * \param handle The SCardHandle
         * \param fileno The file number
         * \param mode The communicatio mode
         * \param value The value stored in the card
         */
        virtual void getValue(unsigned char fileno, EncryptionMode mode, unsigned int& value);

        /**
         * \brief Credit a specific value file.
         * \param fileno The file number
         * \param value The value to credit
         * \param mode The communication mode
         */
        virtual void credit(unsigned char fileno, unsigned int value, EncryptionMode mode);

        /**
         * \brief Debit a specific value file.
         * \param fileno The file number
         * \param value The value to debit
         * \param mode The communication mode
         */
        virtual void debit(unsigned char fileno, unsigned int value, EncryptionMode mode);

        /**
         * \brief Limit credit on a specific value file.
         * \param handle The SCardHandle
         * \param fileno The file number
         * \param value The value to limit credit
         * \param mode The communication mode
         */
        virtual void limitedCredit(unsigned char fileno, unsigned int value, EncryptionMode mode);

        /**
         * \brief Write record into a specific record file.
         * \param fileno The file number
         * \param offset The byte offset
         * \param length The length to write
         * \param data The data buffer
         * \param mode The communication mode
         */
        virtual void writeRecord(unsigned char fileno, unsigned int offset, const std::vector<unsigned char>& data, EncryptionMode mode);

        /**
         * \brief Read record from a specific record file.
         * \param fileno The file number
         * \param offset The byte offset
         * \param length The length to read
         * \param data The data buffer
         * \param mode The communication mode
         * \return The number of bytes read.
         */
        virtual std::vector<unsigned char> readRecords(unsigned char fileno, unsigned int offset, unsigned int length, EncryptionMode mode);

        /**
         * \brief Clear a specific record file.
         * \param fileno The file number.
         */
        virtual void clearRecordFile(unsigned char fileno);

        /**
         * \brief Commit the transaction.
         */
        virtual void commitTransaction();

        /**
         * \brief Abort the transaction.
         */
        virtual void abortTransaction();

        /**
         * \brief Authenticate, given a key number.
         * \param keyno The key number, previously loaded on the profile.
         */
        virtual void authenticate(unsigned char keyno = 0);

        /**
         * \brief Authenticate, given a key number.
         * \param keyno The key number.
         * \param key The key.
         */
        virtual void authenticate(unsigned char keyno, std::shared_ptr<DESFireKey> key);

        void iks_des_authenticate(unsigned char keyno, std::shared_ptr<DESFireKey> key);

        /**
         * \brief Get the card version information.
         * \param dataVersion The card version information structure that will be filled
         */
        virtual void getVersion(DESFireCardVersion& dataVersion);

        /**
         * \brief Get the ISO7816 reader/card adapter.
         * \return The ISO7816 reader/card adapter.
         */
        std::shared_ptr<ISO7816ReaderCardAdapter> getISO7816ReaderCardAdapter() { return std::dynamic_pointer_cast<ISO7816ReaderCardAdapter>(getReaderCardAdapter()); };

        /**
         * \brief Set the SAM Chip.
         * \param t The SAM Chip.
         */
        void setSAMChip(std::shared_ptr<SAMChip> t) { d_SAM_chip = t; }

        /**
         * \brief get the SAM Chip.
         */
        std::shared_ptr<SAMChip> getSAMChip() { return d_SAM_chip; }

		/**
		* \brief retrieve key from SAM AV2 dump key.
		*/
		void getKeyFromSAM(std::shared_ptr<DESFireKey> key, std::vector<unsigned char> diversify);

		ByteVector sam_authenticate_p1(std::shared_ptr<DESFireKey> key, ByteVector rndb, ByteVector diversify);

		void sam_authenticate_p2(unsigned char keyno, ByteVector rndap);

    protected:

		ByteVector getKeyInformations(std::shared_ptr<DESFireKey> key, uint8_t keyno);

        std::vector<unsigned char> getChangeKeySAMCryptogram(unsigned char keyno, std::shared_ptr<DESFireKey> key);

        std::vector<unsigned char> getChangeKeyIKSCryptogram(unsigned char keyno, std::shared_ptr<DESFireKey> key);

        /**
         * \brief Generic method to read data from a file.
         * \param err The last error code
         * \param firstMsg The first message to send
         * \param length The length to read
         * \param mode The communication mode
         * \return The data buffer.
         */
        virtual std::vector<unsigned char> handleReadData(unsigned char err, const std::vector<unsigned char>& firstMsg, unsigned int length, EncryptionMode mode);

        /**
         * \brief Generic method to write data into a file.
         * \param cmd The command to send
         * \param parameters The command parameters
         * \param data The data buffer
         * \param mode The communication mode
         */
		virtual void handleWriteData(unsigned char cmd, const std::vector<unsigned char>& parameters, const std::vector<unsigned char>& data, EncryptionMode mode);

        /**
         * \brief Transmit a command.
         * \param cmd The command code.
         * \param lc Undocumented.
         * \return The response.
         */
        virtual std::vector<unsigned char> transmit(unsigned char cmd, unsigned char lc);

        /**
         * \brief Transmit a command.
         * \param cmd The command code.
         * \param buf The command buffer.
         * \param buflen The command buffer length.
         * \param forceLc Undocumented.
         * \return The response.
         */
        virtual std::vector<unsigned char> transmit(unsigned char cmd, const std::vector<unsigned char>& data = std::vector<unsigned char>(), unsigned char lc = 0, bool forceLc = false);

        /**
         * \brief The SAMChip used for the SAM Commands.
         */
        std::shared_ptr<SAMChip> d_SAM_chip;
    };
}

#endif /* LOGICALACCESS_DESFIREISO7816COMMANDS_HPP */
