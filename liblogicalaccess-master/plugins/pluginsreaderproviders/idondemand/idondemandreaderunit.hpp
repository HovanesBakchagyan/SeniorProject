/**
 * \file idondemandreaderunit.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief IdOnDemand Reader unit.
 */

#ifndef LOGICALACCESS_IDONDEMANDCREADERUNIT_HPP
#define LOGICALACCESS_IDONDEMANDCREADERUNIT_HPP

#include "logicalaccess/readerproviders/readerunit.hpp"
#include "idondemandreaderunitconfiguration.hpp"

namespace logicalaccess
{
    class Profile;
    class IdOnDemandReaderCardAdapter;
    class IdOnDemandReaderProvider;

    /**
     * \brief The IdOnDemand reader unit class.
     */
    class LIBLOGICALACCESS_API IdOnDemandReaderUnit : public ReaderUnit
    {
    public:

        /**
         * \brief Constructor.
         */
        IdOnDemandReaderUnit();

        /**
         * \brief Destructor.
         */
        virtual ~IdOnDemandReaderUnit();

        /**
         * \brief Get the reader unit name.
         * \return The reader unit name.
         */
        virtual std::string getName() const;

        /**
         * \brief Get the connected reader unit name.
         * \return The connected reader unit name.
         */
        virtual std::string getConnectedName();

        /**
         * \brief Set the card type.
         * \param cardType The card type.
         */
        virtual void setCardType(std::string cardType);

        /**
         * \brief Wait for a card insertion.
         * \param maxwait The maximum time to wait for, in milliseconds. If maxwait is zero, then the call never times out.
         * \return True if a card was inserted, false otherwise. If a card was inserted, the name of the reader on which the insertion was detected is accessible with getReader().
         * \warning If the card is already connected, then the method always fail.
         */
        virtual bool waitInsertion(unsigned int maxwait);

        /**
         * \brief Wait for a card removal.
         * \param maxwait The maximum time to wait for, in milliseconds. If maxwait is zero, then the call never times out.
         * \return True if a card was removed, false otherwise. If a card was removed, the name of the reader on which the removal was detected is accessible with getReader().
         */
        virtual bool waitRemoval(unsigned int maxwait);

        /**
         * \brief Create the chip object from card type.
         * \param type The card type.
         * \return The chip.
         */
        virtual std::shared_ptr<Chip> createChip(std::string type);

        /**
         * \brief Get the first and/or most accurate chip found.
         * \return The single chip.
         */
        virtual std::shared_ptr<Chip> getSingleChip();

        /**
         * \brief Get chip available in the RFID rang.
         * \return The chip list.
         */
        virtual std::vector<std::shared_ptr<Chip> > getChipList();

        /**
         * \brief Get the default IdOnDemand reader/card adapter.
         * \return The default IdOnDemand reader/card adapter.
         */
        virtual std::shared_ptr<IdOnDemandReaderCardAdapter> getDefaultIdOnDemandReaderCardAdapter();

        /**
         * \brief Connect to the card.
         * \return True if the card was connected without error, false otherwise.
         *
         * If the card handle was already connected, connect() first call disconnect(). If you intend to do a reconnection, call reconnect() instead.
         */
        virtual bool connect();

        /**
         * \brief Disconnect from the reader.
         * \see connect
         *
         * Calling this method on a disconnected reader has no effect.
         */
        virtual void disconnect();

        /**
         * \brief Check if the card is connected.
         * \return True if the card is connected, false otherwise.
         */
        virtual bool isConnected();

        /**
         * \brief Connect to the reader. Implicit connection on first command sent.
         * \return True if the connection successed.
         */
        virtual bool connectToReader();

        /**
         * \brief Disconnect from reader.
         */
        virtual void disconnectFromReader();

        /**
         * \brief Get the reader ping command.
         * \return The ping command.
         */
        virtual std::vector<unsigned char> getPingCommand() const;

        /**
         * \brief Get a string hexadecimal representation of the reader serial number
         * \return The reader serial number or an empty string on error.
         */
        virtual std::string getReaderSerialNumber();

        /**
         * \brief Serialize the current object to XML.
         * \param parentNode The parent node.
         */
        virtual void serialize(boost::property_tree::ptree& parentNode);

        /**
         * \brief UnSerialize a XML node to the current object.
         * \param node The XML node.
         */
        virtual void unSerialize(boost::property_tree::ptree& node);

        /**
         * \brief Get the IdOnDemand reader unit configuration.
         * \return The IdOnDemand reader unit configuration.
         */
        std::shared_ptr<IdOnDemandReaderUnitConfiguration> getIdOnDemandConfiguration() { return std::dynamic_pointer_cast<IdOnDemandReaderUnitConfiguration>(getConfiguration()); };

        /**
         * \brief Get the IdOnDemand reader provider.
         * \return The IdOnDemand reader provider.
         */
        std::shared_ptr<IdOnDemandReaderProvider> getIdOnDemandReaderProvider() const;

        /**
         * \brief Authenticate the reader with the configured authentication code for SDK use.
         */
        void authenticateSDK();

        /**
         * \brief Authenticate the reader for SDK use.
         * \param authCode The authentication code.
         */
        void authenticateSDK(std::string authCode);

        /**
         * \brief Causes the reader's speaker to beep when a programming routine is completed.
         */
        void beep();

        /**
         * \brief Read a card presented to the reader. The card's data is loaded into the internal reader buffer and can be displayed
         * using the verify() command, and can be re-writen to a new card by using the write() command.
         * \return True if the card is read successfully, false otherwise.
         */
        bool read();

        /**
         * \brief Write the internal reader buffer to the card.
         * \return True if the card is write successfully, false otherwise.
         */
        bool write();

        /**
         * \brief Verify the presented card with internal buffer.
         * \return True if the card is verified successfully, false otherwise.
         */
        bool verify();

        /**
         * \brief Turn on/off the EM card programming.
         * \param activate True to turn on the EM card, false otherwise.
         */
        void activateEMCard(bool activate);

        /**
         * \brief Turn on/off the Atmel card programming.
         * \param activate True to turn on the Atmel card, false otherwise.
         */
        void activateAtmelCard(bool activate);

        /**
         * \brief Get chip in air if available.
         * \return The chip.
         * \remarks This function change the reader state! Do not use in multi-threading process.
         */
        std::shared_ptr<Chip> getChipInAir();
    };
}

#endif