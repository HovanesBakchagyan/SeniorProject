/**
 * \file scielreaderunitconfiguration.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief SCIEL Reader unit configuration.
 */

#ifndef LOGICALACCESS_SCIELREADERUNITCONFIGURATION_HPP
#define LOGICALACCESS_SCIELREADERUNITCONFIGURATION_HPP

#include "logicalaccess/readerproviders/readerunitconfiguration.hpp"

namespace logicalaccess
{
    /**
     * \brief The SCIEL reader unit configuration base class.
     */
    class LIBLOGICALACCESS_API SCIELReaderUnitConfiguration : public ReaderUnitConfiguration
    {
    public:

        /**
         * \brief Constructor.
         */
        SCIELReaderUnitConfiguration();

        /**
         * \brief Destructor.
         */
        virtual ~SCIELReaderUnitConfiguration();

        /**
         * \brief Reset the configuration to default values
         */
        virtual void resetConfiguration();

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
         * \brief Get the default Xml Node name for this object.
         * \return The Xml node name.
         */
        virtual std::string getDefaultXmlNodeName() const;

        /**
         * \brief Get the tag-out threshold.
         * \return The tag-out threshold.
         */
        unsigned char getTagOutThreshold() const { return d_tagOutThreshold; };

        /**
         * \brief Set the tag-out threshold.
         * \param threshold The tag-out threshold.
         */
        void setTagOutThreshold(unsigned char threshold) { d_tagOutThreshold = threshold; };

        /**
         * \brief Get the tag-in threshold.
         * \return The tag-in threshol.
         */
        unsigned char getTagInThreshold() const { return d_tagInThreshold; };

        /**
         * \brief Set the tag-in threshold.
         * \param threshold The tag-in threshold.
         */
        void setTagInThreshold(unsigned char threshold) { d_tagInThreshold = threshold; };

        /**
         * \brief Get the safety threshold.
         * \return The safety threshol.
         */
        unsigned char getSafetyThreshold() const { return d_safetyThreshold; };

        /**
         * \brief Set the safety threshold.
         * \param threshold The safety threshold.
         */
        void setSafetyThreshold(unsigned char threshold) { d_safetyThreshold = threshold; };

        /**
         * \brief Get the time before the tag is considered as "in", in milliseconds.
         * \return The time.
         */
        unsigned short getTimeBeforeTagIn() const { return d_timeBeforeTagIn; };

        /**
         * \brief Set the time before the tag is considered as "in", in milliseconds.
         * \param timeBefore The time.
         */
        void setTimeBeforeTagIn(unsigned short timeBefore) { d_timeBeforeTagIn = timeBefore; };

        /**
         * \brief Get the time before the tag is considered as "out", in seconds.
         * \return The time.
         */
        unsigned short getTimeBeforeTagOut() const { return d_timeBeforeTagOut; };

        /**
         * \brief Set the time before the tag is considered as "out", in seconds.
         * \param timeBefore The time.
         */
        void setTimeBeforeTagOut(unsigned short timeBefore) { d_timeBeforeTagOut = timeBefore; };

        /**
         * \brief Get the time before the tag is considered as in safety zone, in milliseconds.
         * \return The time.
         */
        unsigned short getTimeBeforeSafety() const { return d_timeBeforeSafety; };

        /**
         * \brief Set the time before the tag is considered as in safety zone, in milliseconds.
         * \param timeBefore The time.
         */
        void setTimeBeforeSafety(unsigned short timeBefore) { d_timeBeforeSafety = timeBefore; };

        /**
         * \brief Get the time before the tag is removed from the stack, in seconds.
         * \return The time.
         */
        unsigned char getTimeRemoval() const { return d_timeRemoval; };

        /**
         * \brief Set the time before the tag is removed from the stack, in seconds.
         * \param timeBefore The time.
         */
        void setTimeRemoval(unsigned char timeRemoval) { d_timeRemoval = timeRemoval; };

    protected:

        /**
         * \brief The tag-out threshold.
         */
        unsigned char d_tagOutThreshold;

        /**
         * \brief The tag-in threshold.
         */
        unsigned char d_tagInThreshold;

        /**
         * \brief The safety threshold.
         */
        unsigned char d_safetyThreshold;

        /**
         * \brief The time before the tag is declared as "in", in milliseconds.
         */
        unsigned short d_timeBeforeTagIn;

        /**
         * \brief The time before the tag is declared as "out", in milliseconds.
         */
        unsigned short d_timeBeforeTagOut;

        /**
         * \brief The time before the tag is declared as in safety zone, in milliseconds.
         */
        unsigned short d_timeBeforeSafety;

        /**
         * \brief The time before a tag is removed from the stack.
         */
        unsigned char d_timeRemoval;
    };
}

#endif