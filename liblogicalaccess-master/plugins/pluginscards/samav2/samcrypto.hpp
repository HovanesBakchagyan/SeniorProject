/**
 * \file samcrypto.hpp
 * \author Adrien J. <adrien.jund@islog.com>
 * \brief SAMDESfireCrypto header.
 */

#ifndef SAMCRYPTO_HPP
#define SAMCRYPTO_HPP

#include "logicalaccess/crypto/des_cipher.hpp"
#include "logicalaccess/crypto/aes_cipher.hpp"
#include "desfire/desfirecrypto.hpp"

#ifndef UNIX
#include "logicalaccess/msliblogicalaccess.h"
#endif

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
    /**
     * \brief SAM cryptographic functions.
     */
    class LIBLOGICALACCESS_API SAMDESfireCrypto : public DESFireCrypto
    {
    public:
        /**
         * \brief Constructor
         */
        SAMDESfireCrypto();

        /**
         * \brief Destructor
         */
        virtual ~SAMDESfireCrypto();

        std::vector<unsigned char> authenticateHostP1(std::shared_ptr<DESFireKey> key, std::vector<unsigned char> encRndB, unsigned char keyno);

        void authenticateHostP2(unsigned char keyno, std::vector<unsigned char> encRndA1, std::shared_ptr<DESFireKey> key);

        std::vector<unsigned char> sam_crc_encrypt(std::vector<unsigned char> d_sessionKey, std::vector<unsigned char> vectordata, std::shared_ptr<DESFireKey> key);
    };
}

#endif