/**
 * \file rplethreaderunit.hpp
 * \author Maxime C. <maxime-dppev@islog.com>
 * \brief Rpleth Reader unit.
 */

#ifndef LOGICALACCESS_RPLETHREADERUNIT_HPP
#define LOGICALACCESS_RPLETHREADERUNIT_HPP

#include "../iso7816/iso7816readerunit.hpp"
#include "rpleth_fwd.hpp"

namespace logicalaccess
{

    /**
     * \brief The Rpleth reader unit class.
     */
    class LIBLOGICALACCESS_API RplethReaderUnit : public ISO7816ReaderUnit
    {
    public:

        /**
         * \brief Constructor.
         */
        RplethReaderUnit();

        /**
         * \brief Destructor.
         */
        virtual ~RplethReaderUnit();

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
         * \brief Get the current chip in air.
         * \return The chip in air.
         */
        std::shared_ptr<Chip> getChipInAir(unsigned int maxwait = 2000);

        /**
         * \brief Get the default Rpleth reader/card adapter.
         * \return The default Rpleth reader/card adapter.
         */
        virtual std::shared_ptr<RplethReaderCardAdapter> getDefaultRplethReaderCardAdapter();

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
         * \return True if the connection succeeded.
         */
        virtual bool connectToReader();

        /**
         * \brief Disconnect from reader.
         */
        virtual void disconnectFromReader();

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

        std::shared_ptr<RplethReaderUnitConfiguration> getRplethConfiguration();

        /**
         * \brief Get the Rpleth reader provider.
         * \return The Rpleth reader provider.
         */
        std::shared_ptr<RplethReaderProvider> getRplethReaderProvider() const;

        /**
         * \brief Get reader dhcp state.
         */
        bool getDhcpState();

        /**
         * \brief Set the reader dhcp state.
         */
        void setDhcpState(bool status);

        /**
         * \brief Set the Rpleth IP Address.
         * \param readerAddress The new Rpleth IP Address.
         */
        void setReaderIp(const std::vector<unsigned char>& address);

        /**
         * \brief Set the Rpleth MAC Address.
         * \param readerAddress The new Rpleth MAC Address.
         */
        void setReaderMac(const std::vector<unsigned char>& address);

        /**
         * \brief Set the Rpleth Subnet mask.
         * \param readerAddress The new Rpleth Subnet Subnet.
         */
        void setReaderSubnet(const std::vector<unsigned char>& address);

        /**
         * \brief Set the Rpleth Gateway Address.
         * \param readerAddress The new Rpleth Gateway Address.
         */
        void setReaderGateway(const std::vector<unsigned char>& address);

        /**
         * \brief Set the Rpleth port.
         * \param readerAddress The new Rpleth port.
         */
        void setReaderPort(int port);

        /**
         * \brief Reset the Rpleth reader.
         */
        void resetReader();

        /**
         * \brief Set the Rpleth default message.
         * \param message The new default message.
         */
        void setReaderMessage(const std::string& message);

        /**
         * \brief Send a nop.
         */
        void nop();

		/**
		 * \brief Set the client context.
		 * \param context The context.
		 */
		virtual void setContext(const std::string& context);

        /**
        * \brief Get the client context.
        * \return The context.
        */
        virtual std::string getContext();

        /**
         * \brief Request the last badge.
         * \param timeout The time to wait.
         * \return The last badge.
         */
        std::vector<unsigned char> badge(long int timeout = 2000);

        std::string getProxyReaderType();

        std::vector<unsigned char> getInsertedChipIdentifier();

        void sendCardWaited(std::vector<unsigned char> card);

        std::vector<unsigned char> receiveCardWaited(bool present = false);

    protected:

        /**
         * \brief Send a command to the reader.
         * \param command The command buffer.
         * \param timeout The command timeout.
         * \return The result of the command.
         */
        std::vector<unsigned char> receiveBadge(long int timeout = 2000);

        /**
         * \brief Take the csn from wiegand full trame.
         * \param trame The wiegnad full trame.
         * \return The csn contains into the wiegand trame.
         */
        std::vector<unsigned char> getCsn(const std::vector<unsigned char>& trame);

        /**
         * \brief Last successed RATS result.
         */
        std::vector<unsigned char> d_successedRATS;

        /**
         * \brief The new identifier that will be used for the next waitInsertion after the waitRemoval.
         */
        std::vector<unsigned char> removalIdentifier;

        std::shared_ptr<ReaderUnit> d_proxyReader;
    };
}

#endif
