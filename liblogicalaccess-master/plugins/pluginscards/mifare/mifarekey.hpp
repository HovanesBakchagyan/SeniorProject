/**
 * \file mifarekey.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Mifare key.
 */

#ifndef LOGICALACCESS_MIFAREKEY_HPP
#define LOGICALACCESS_MIFAREKEY_HPP

#include "logicalaccess/key.hpp"

/**
 * \brief The default Mifare key size
 */
#define MIFARE_KEY_SIZE 6

namespace logicalaccess
{
    /**
     * \brief A Mifare Key class.
     */
    class LIBLOGICALACCESS_API MifareKey : public Key
    {
    public:
        using XmlSerializable::serialize;
        using XmlSerializable::unSerialize;

        /**
         * \brief Build an empty 6-bytes Mifare key.
         */
        MifareKey();

        /**
         * \brief Build a Mifare key given a string representation of it.
         * \param str The string representation.
         */
        MifareKey(const std::string& str);

        /**
         * \brief Build a Mifare key given a buffer.
         * \param buf The buffer.
         * \param buflen The buffer length.
         */
        MifareKey(const void* buf, size_t buflen);

        /**
         * \brief Clear the key.
         */
        virtual void clear();

        /**
         * \brief Get the key length.
         * \return The key length.
         */
        inline size_t getLength() const { return MIFARE_KEY_SIZE; };

        /**
         * \brief Get the key data.
         * \return The key data.
         */
        inline const unsigned char* getData() const { return d_key; };

        /**
         * \brief Get the key data.
         * \return The key data.
         */
        inline unsigned char* getData() { return d_key; };

        /**
         * \brief Serialize the current object to XML.
         * \param parentNode The parent node.
         */
        virtual void serialize(boost::property_tree::ptree& parentNode) override;

        /**
         * \brief UnSerialize a XML node to the current object.
         * \param node The XML node.
         */
        virtual void unSerialize(boost::property_tree::ptree& node) override;

        /**
         * \brief Get the default Xml Node name for this object.
         * \return The Xml node name.
         */
        virtual std::string getDefaultXmlNodeName() const override;

    private:

        /**
         * \brief The key bytes
         */
        unsigned char d_key[MIFARE_KEY_SIZE];
    };
}

#endif /* LOGICALACCESS_MIFAREKEY_HPP */