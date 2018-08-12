/**
 * \file pcscreaderunit.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief PC/SC Reader unit.
 */

#ifndef LOGICALACCESS_ISO7816READERUNIT_HPP
#define LOGICALACCESS_ISO7816READERUNIT_HPP

#include "logicalaccess/readerproviders/readerunit.hpp"
#include "iso7816readerunitconfiguration.hpp"
#include "logicalaccess/cards/readermemorykeystorage.hpp"
#include "logicalaccess/readerproviders/readerunit.hpp"

namespace logicalaccess
{
    class Chip;
    class SAMChip;
    class ISO7816ReaderProvider;

    /**
     * \brief The ISO7816 reader unit class.
     */
    class LIBLOGICALACCESS_API ISO7816ReaderUnit : public ReaderUnit
    {
    public:
		ISO7816ReaderUnit(std::string rpt);

        virtual ~ISO7816ReaderUnit();
        /**
         * \brief Wait for a card insertion.
         * \param maxwait The maximum time to wait for, in milliseconds. If maxwait is zero, then the call never times out.
         * \return True if a card was inserted, false otherwise. If a card was inserted, the name of the reader on which the insertion was detected is accessible with getReader().
         * \warning If the card is already connected, then the method always fail.
         */
        virtual bool waitInsertion(unsigned int /*maxwait*/) override { return true; }

        /**
         * \brief Wait for a card removal.
         * \param maxwait The maximum time to wait for, in milliseconds. If maxwait is zero, then the call never times out.
         * \return True if a card was removed, false otherwise. If a card was removed, the name of the reader on which the removal was detected is accessible with getReader().
         */
        virtual bool waitRemoval(unsigned int /*maxwait*/) { return true; }

        /**
         * \brief Check if the card is connected.
         * \return True if the card is connected, false otherwise.
         */
        virtual bool isConnected() { return true; }

        /**
         * \brief Set the forced card type.
         * \param cardType The forced card type.
         */
        virtual void setCardType(std::string /*cardType*/) {}

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
         * \brief Connect to the card.
         * \return True if the card was connected without error, false otherwise.
         *
         * If the card handle was already connected, connect() first call disconnect(). If you intend to do a reconnection, call reconnect() instead.
         */
        virtual bool connect() { return true; }

        /**
         * \brief Disconnect from the card.
         */
        virtual void disconnect() {}

        /**
         * \brief Connect to the reader. Implicit connection on first command sent.
         * \return True if the connection succeeded.
         */
        virtual bool connectToReader();

        /**
         * \brief Disconnect from reader.
         */
        virtual void disconnectFromReader();

        virtual void reloadReaderConfiguration();

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
         * \brief Get the reader unit name.
         * \return The reader unit name.
         */
        virtual std::string getName() const { return ""; }

        /**
         * \brief Get a string hexadecimal representation of the reader serial number
         * \return The reader serial number or an empty string on error.
         */
        virtual std::string getReaderSerialNumber() { return ""; }

        /**
         * \brief Get The SAM Chip
         */
        virtual std::shared_ptr<SAMChip> getSAMChip();

        /**
         * \brief Set the SAM Chip
         */
        virtual void setSAMChip(std::shared_ptr<SAMChip> t);

        /**
         * \brief Get The SAM ReaderUnit
         */
        virtual std::shared_ptr<ISO7816ReaderUnit> getSAMReaderUnit();

        /**
         * \brief Set the SAM ReaderUnit
         */
        virtual void setSAMReaderUnit(std::shared_ptr<ISO7816ReaderUnit> t);

      protected:
        virtual std::shared_ptr<ResultChecker> createDefaultResultChecker() const override;

        /**
        * \brief Get the ISO7816 reader provider.
        * \return The ISO7816 reader provider.
        */
        std::shared_ptr<ISO7816ReaderProvider> getISO7816ReaderProvider() const;

		/**
		 * \brief Get the ISO7816 reader unit configuration.
		 * \return The ISO7816 reader unit configuration.
		 */
		std::shared_ptr<ISO7816ReaderUnitConfiguration> getISO7816Configuration() { return std::dynamic_pointer_cast<ISO7816ReaderUnitConfiguration>(getConfiguration()); };

		virtual bool reconnect(int action);

      protected:
        /**
         * \brief The SAM chip.
         */
        std::shared_ptr<SAMChip> d_sam_chip;

        /**
         * \brief The SAM ReaderUnit used SAM Authentication
         */
        std::shared_ptr<ISO7816ReaderUnit> d_sam_readerunit;

        /**
         * \brief The client context.
         */
        std::string d_client_context;

        /**
        * \brief The sam client context.
        */
        std::string d_sam_client_context;
    };
}

#endif