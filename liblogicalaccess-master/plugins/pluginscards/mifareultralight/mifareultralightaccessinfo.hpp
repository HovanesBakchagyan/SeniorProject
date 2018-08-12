/**
 * \file mifareultralightaccessinfo.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Mifare Ultralight AccessInfo.
 */

#ifndef LOGICALACCESS_MIFAREULTRALIGHTACCESSINFO_HPP
#define LOGICALACCESS_MIFAREULTRALIGHTACCESSINFO_HPP

#include "logicalaccess/cards/accessinfo.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace logicalaccess
{
    /**
     * \brief A Mifare Ultralight access informations.
     */
    class LIBLOGICALACCESS_API MifareUltralightAccessInfo : public AccessInfo
    {
    public:
        /**
         * \brief Constructor.
         */
        MifareUltralightAccessInfo();

        /**
         * \brief Destructor.
         */
        virtual ~MifareUltralightAccessInfo();

        /**
         * \brief Generate pseudo-random Mifare Ultralight access informations.
         */
        virtual void generateInfos();

        /**
         * \brief Get the card type for this access infos.
         * \return The card type.
         */
        virtual std::string getCardType() const;

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
         * \brief Equality operator
         * \param ai Access infos to compare.
         * \return True if equals, false otherwise.
         */
        virtual bool operator==(const AccessInfo& ai) const;

    public:

        /**
         * \brief Page is locked ?
         */
        bool lockPage;
    };
}

#endif /* LOGICALACCESS_MIFAREULTRALIGHTACCESSINFO_H */