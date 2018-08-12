/**
 * \file readermemorykeystorage.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Reader memory key storage description.
 */

#ifndef LOGICALACCESS_READERMEMORYKEYSTORAGE_HPP
#define LOGICALACCESS_READERMEMORYKEYSTORAGE_HPP

#include "logicalaccess/cards/keystorage.hpp"

namespace logicalaccess
{
    /**
     * \brief A reader memory key storage class.
     */
    class LIBLOGICALACCESS_API ReaderMemoryKeyStorage : public KeyStorage
    {
    public:

        /**
         * \brief Constructor.
         */
        ReaderMemoryKeyStorage();

        /**
         * \brief Get the key storage type.
         * \return The key storage type.
         */
        virtual KeyStorageType getType() const;

        /**
         * \brief Set the key slot.
         * \param key_slot The key slot.
         */
        void setKeySlot(unsigned char key_slot);

        /**
         * \brief Get the key slot.
         * \return The key slot.
         */
        unsigned char getKeySlot() const;

        /**
         * \brief Set if the key is stored volatile.
         * \param vol True if the key is stored volatile, false otherwise.
         */
        void setVolatile(bool vol);

        /**
         * \brief Set if the key is stored volatile.
         * \return True if the key is stored volatile, false otherwise.
         */
        bool getVolatile() const;

        /**
         * \brief Serialize the current object to XML.
         * \param parentNode The parent node.
         * \return The XML string.
         */
        virtual void serialize(boost::property_tree::ptree& parentNode);

        /**
         * \brief UnSerialize a XML node to the current object.
         * \param node The XML node.
         */
        virtual void unSerialize(boost::property_tree::ptree& node);

        /**
         * \brief Get the default Xml Node name for this object.
         * \return The Xml node name.
         */
        virtual std::string getDefaultXmlNodeName() const;

    protected:

        /**
         * \brief The key slot.
         */
        unsigned char d_key_slot;

        /**
         * \brief True if the key is stored volatile, false otherwise.
         */
        bool d_volatile;
    };
}

#endif /* LOGICALACCESS_READERMEMORYKEYSTORAGE_HPP */