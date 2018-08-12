/**
 * \file mifarepcsccommands.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Mifare pcsc card.
 */

#ifndef LOGICALACCESS_MIFAREPCSCCOMMANDS_HPP
#define LOGICALACCESS_MIFAREPCSCCOMMANDS_HPP

#include "mifare/mifarecommands.hpp"
#include "../readercardadapters/pcscreadercardadapter.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
    template<typename T, typename T2>
    class MifarePlusSL1Policy;

    /**
     * \brief The Mifare card provider class for PCSC reader.
     */
    class LIBLOGICALACCESS_API MifarePCSCCommands : public virtual MifareCommands
    {
    public:

        /**
         * \brief Constructor.
         */
        MifarePCSCCommands();

        /**
         * \brief Destructor.
         */
        virtual ~MifarePCSCCommands();

        /**
         * \brief Get the PC/SC reader/card adapter.
         * \return The PC/SC reader/card adapter.
         */
        std::shared_ptr<PCSCReaderCardAdapter> getPCSCReaderCardAdapter() { return std::dynamic_pointer_cast<PCSCReaderCardAdapter>(getReaderCardAdapter()); };

        /**
         * \brief Read bytes from the card.
         * \param blockno The block number.
         * \param len The count of bytes to read. (0 <= len < 16)
         * \param buf The buffer in which to place the data.
         * \param buflen The length of buffer.
         * \return The count of bytes red.
         */
        virtual std::vector<unsigned char> readBinary(unsigned char blockno, size_t len);

        /**
         * \brief Write bytes to the card.
         * \param blockno The block number.
         * \param buf The buffer containing the data.
         * \param buflen The length of buffer.
         * \return The count of bytes written.
         */
        virtual void updateBinary(unsigned char blockno, const std::vector<unsigned char>& buf);

        virtual void increment(uint8_t blockno, uint32_t value) override;

        virtual void decrement(uint8_t blockno, uint32_t value) override;

    protected:

        /**
         * \brief Load a key to the reader.
         * \param keyno The key number.
         * \param keytype The mifare key type.
         * \param key The key.
         * \param vol Use volatile memory.
         * \return true on success, false otherwise.
         */
        bool loadKey(unsigned char keyno, MifareKeyType keytype, std::shared_ptr<MifareKey> key, bool vol = false);

        /**
         * \brief Load a key on a given location.
         * \param location The location.
         * \param key The key.
         * \param keytype The mifare key type.
         */
		virtual void loadKey(std::shared_ptr<Location> location, MifareKeyType keytype, std::shared_ptr<MifareKey> key);

        /**
         * \brief Authenticate a block, given a key number.
         * \param blockno The block number.
         * \param key_storage The key storage used for authentication.
         * \param keytype The key type.
         */
        virtual void authenticate(unsigned char blockno, std::shared_ptr<KeyStorage> key_storage, MifareKeyType keytype);

        /**
         * \brief Authenticate a block, given a key number.
         * \param blockno The block number.
         * \param keyno The key number, previously loaded with Mifare::loadKey().
         * \param keytype The key type.
         */
        virtual void authenticate(unsigned char blockno, unsigned char keyno, MifareKeyType keytype) override;

        template<typename T, typename T2>
        friend class MifarePlusSL1Policy;
    };
}

#endif /* LOGICALACCESS_MIFAREPCSCCOMMANDS_HPP */