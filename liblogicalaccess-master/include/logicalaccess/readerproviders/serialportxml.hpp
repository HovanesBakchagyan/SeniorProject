/**
 * \file serialportxml.hpp
 * \brief A serial port class.
 * \author Julien K. <julien-dev@islog.com>, Maxime C. <maxime-dev@islog.com>
 */

#ifndef SERIALPORTXML_HPP
#define SERIALPORTXML_HPP

#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "logicalaccess/readerproviders/circularbufferparser.hpp"
#include "logicalaccess/readerproviders/serialport.hpp"

#include <iostream>
#include <string>

namespace logicalaccess
{
    class LIBLOGICALACCESS_API SerialPortXml : public XmlSerializable
    {
    public:

        /**
         * \brief Constructor.
         */
        SerialPortXml();

        /**
         * \brief Constructor.
         * \param dev The device name. (Example: "/dev/tty0")
         */
        SerialPortXml(const std::string& dev);

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
         * \brief Get the serial port object.
         * \return The serial port.
         */
        std::shared_ptr<SerialPort> getSerialPort() const;

        /**
         * \brief Enumate available COM port using CreateFile function.
         * \param ports The list which will contains the available COM port.
         * \return True on success, false otherwise.
         */
        static bool EnumerateUsingCreateFile(std::vector<std::shared_ptr<SerialPortXml> >& ports);

    protected:

        std::shared_ptr<SerialPort> d_serialport;
    };
}

#endif /* SERIALPORTXML_HPP */