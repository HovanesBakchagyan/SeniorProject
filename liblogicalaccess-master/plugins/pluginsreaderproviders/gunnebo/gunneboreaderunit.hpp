/**
 * \file gunneboreaderunit.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Gunnebo Reader unit.
 */

#ifndef LOGICALACCESS_GUNNEBOREADERUNIT_HPP
#define LOGICALACCESS_GUNNEBOREADERUNIT_HPP

#include "logicalaccess/readerproviders/readerunit.hpp"
#include "gunneboreaderunitconfiguration.hpp"

namespace logicalaccess
{
    class Profile;
    class GunneboReaderCardAdapter;
    class GunneboReaderProvider;

    /**
     * \brief The Gunnebo reader unit class.
     */
    class LIBLOGICALACCESS_API GunneboReaderUnit : public ReaderUnit
    {
    public:

        /**
         * \brief Constructor.
         */
        GunneboReaderUnit();

        /**
         * \brief Destructor.
         */
        virtual ~GunneboReaderUnit();

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
         * \brief Get the default Gunnebo reader/card adapter.
         * \return The default Gunnebo reader/card adapter.
         */
        virtual std::shared_ptr<GunneboReaderCardAdapter> getDefaultGunneboReaderCardAdapter();

        /**
         * \brief Connect to the card.
         * \return True if the card was connected without error, false otherwise.
         *
         * If the card handle was already connected, connect() first call disconnect(). If you intend to do a reconnection, call reconnect() instead.
         */
        bool connect();

        /**
         * \brief Disconnect from the reader.
         * \see connect
         *
         * Calling this method on a disconnected reader has no effect.
         */
        void disconnect();

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
         * \brief Get the Gunnebo reader unit configuration.
         * \return The Gunnebo reader unit configuration.
         */
        std::shared_ptr<GunneboReaderUnitConfiguration> getGunneboConfiguration() { return std::dynamic_pointer_cast<GunneboReaderUnitConfiguration>(getConfiguration()); };

        /**
         * \brief Get the Gunnebo reader provider.
         * \return The Gunnebo reader provider.
         */
        std::shared_ptr<GunneboReaderProvider> getGunneboReaderProvider() const;

        /**
        * \brief Set the reader unit configuration.
        * \param config The reader unit configuration.
        */
        virtual void setConfiguration(std::shared_ptr<ReaderUnitConfiguration> config);

    private:

        /**
         * \brief Process the incomming serial data into the card identifier.
         * \param port The data read on the COM port.
         * \return The card identifier.
         */
        std::vector<unsigned char> processCardId(std::vector<unsigned char>& rawSerialData);

    protected:

        /**
         * \brief The new identifier that will be used for the next waitInsertion after the waitRemoval.
         */
        std::vector<unsigned char> removalIdentifier;
    };
}

#endif