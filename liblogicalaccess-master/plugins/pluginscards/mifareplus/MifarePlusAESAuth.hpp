#pragma once

#include <logicalaccess/cards/aes128key.hpp>
#include "../mifare/mifareaccessinfo.hpp"

namespace logicalaccess
{
    /**
     * Implement AES authentication operation
     * for MifarePlus cards.
     *
     * This is implemented as a separate object to use reuse between various
     * mifare plus.
     */
    class MifarePlusAESAuth
    {
    public:
        MifarePlusAESAuth(std::shared_ptr<ReaderCardAdapter> rca);

        /**
         * Authenticate a key from a keyslot.
         * This is used for switching security level or AES auth in SL1.
         */
        bool AESAuthenticate(std::shared_ptr<AES128Key> key, uint16_t keyslot);

    private:
        bool aes_auth_step2(std::vector<uint8_t> rnd_b,
                            std::shared_ptr<AES128Key> key);
        bool aes_auth_final(const ByteVector &rnd_a,
                            const ByteVector &rnd_a_reader,
                            std::shared_ptr<AES128Key> key);

        std::shared_ptr<ReaderCardAdapter> rca_;
    };
}