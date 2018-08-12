/**
 * \file cardsformatcomposite.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Cards format composite.
 */

#ifndef LOGICALACCESS_CARDSFORMATCOMPOSITE_HPP
#define LOGICALACCESS_CARDSFORMATCOMPOSITE_HPP

#include <map>
#include <logicalaccess/cards/chip.hpp>
#include <logicalaccess/xmlserializable.hpp>

namespace logicalaccess
{
    /**
     * \brief Format information for a card type.
     */
    struct FormatInfos
    {
        std::shared_ptr<Format> format; /**< \brief Format information. */
        std::shared_ptr<Location> location; /**< \brief Location information. */
        std::shared_ptr<AccessInfo> aiToUse; /**< \brief Access information to use. */
        std::shared_ptr<AccessInfo> aiToWrite; /**< \brief Access information to write. */

        FormatInfos()
        {
        }
    };

    /**
     * \brief A format infos map.
     */
    typedef std::map<std::string, FormatInfos> FormatInfosList;

    /**
     * \brief A format infos pair.
     */
    typedef std::pair<std::string, FormatInfos> FormatInfosPair;

    /**
     * \brief A card type list.
     */
    typedef std::vector<std::string> CardTypeList;

    /**
     * \brief A Cards format composite class.
     */
    class LIBLOGICALACCESS_API CardsFormatComposite : public XmlSerializable
    {
    public:
        using XmlSerializable::serialize;
        using XmlSerializable::unSerialize;

        /**
         * \brief Constructor.
         */
        CardsFormatComposite();

        /**
         * \brief Destructor.
         */
        virtual ~CardsFormatComposite();

        /**
         * \brief Create a format instance from Xml string.
         * \param xmlstring The Xml string.
         * \param rootNode The root node.
         * \return The format instance.
         */
        std::shared_ptr<Format> createFormatFromXml(const std::string& xmlstring, const std::string& rootNode);

        /**
         * \brief Add a format for a card type.
         * \param type The card type.
         * \param format The format.
         * \param location The format location.
         * \param aiToUse The format access information.
         * \param aiToWrite The format access information to write in write mode.
         */
        void addFormatForCard(std::string type, std::shared_ptr<Format> format,
                              std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse,
                              std::shared_ptr<AccessInfo> aiToWrite = std::shared_ptr<AccessInfo>());

        /**
         * \brief Retrieve format information for a card type.
         * \param type The card type.
         * \param format The format.
         * \param location The format location.
         * \param aiToUse The format access information.
         * \param aiToWrite The format access information to write in write mode.
         */
        void retrieveFormatForCard(std::string type, std::shared_ptr<Format>* format,
                                   std::shared_ptr<Location>* location, std::shared_ptr<AccessInfo>* aiToUse,
                                   std::shared_ptr<AccessInfo>* aiToWrite = NULL);

        /**
         * \brief Remove a format for a card type.
         * \param type The card type.
         */
        void removeFormatForCard(std::string type);

        /**
         * \brief Get configured card type.
         * \return The configured card type list.
         */
        CardTypeList getConfiguredCardTypes();

        /**
         * \brief Read format from a card.
         * \return The format.
         */
        std::shared_ptr<Format> readFormat();

        /**
         * \brief Read format from a card.
         * \param chip The chip object.
         * \return The format.
         */
        std::shared_ptr<Format> readFormat(std::shared_ptr<Chip> chip);

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

        /**
         * \brief Get the reader unit.
         * \return The reader unit.
         */
        std::shared_ptr<ReaderUnit> getReaderUnit() const;

        /*
         * \brief Set the reader unit.
         * \param unit The reader unit.
         */
        void setReaderUnit(std::weak_ptr<ReaderUnit> unit);

    protected:

        FormatInfosList formatsList;	/**< \brief The configured formats' list */

        /**
         * \brief The reader unit.
         */
        std::weak_ptr<ReaderUnit> d_ReaderUnit;
    };
}

#endif /* LOGICALACCESS_CARDSFORMATCOMPOSITE_HPP */