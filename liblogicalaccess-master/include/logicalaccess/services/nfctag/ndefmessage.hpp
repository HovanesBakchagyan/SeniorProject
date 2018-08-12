/**
 * \file ndefmessage.hpp
 * \author Adrien J. <adrien-dev@islog.com>
 * \brief NDEF Message.
 */

#ifndef LOGICALACCESS_NDEFMESSAGE_HPP
#define LOGICALACCESS_NDEFMESSAGE_HPP

#include "logicalaccess/xmlserializable.hpp"
#include "logicalaccess/services/nfctag/ndefrecord.hpp"

namespace logicalaccess
{
    enum NdefType {
        Text = 0x54,
        Uri = 0x55
    };

    enum UriType {
        NO_PREFIX = 0x00, // no prefix
        HTTP_WWW = 0x01, // http://www.
        HTTPS_WWW = 0x02, // https://www.
        HTTP = 0x03, // http://
        HTTPS = 0x04, // https://
        TEL = 0x05, // tel:
        MAIL_TO = 0x06, // mailto:
        URI_FILE = 0x1D // file://
    };

    class LIBLOGICALACCESS_API NdefMessage : public XmlSerializable
    {
    public:
        NdefMessage() {};
        NdefMessage(const std::vector<unsigned char>& data);
        virtual ~NdefMessage() {};

        std::vector<unsigned char> encode();

        void addRecord(std::shared_ptr<NdefRecord> record) { m_records.push_back(record); };
        void addMimeMediaRecord(std::string mimeType, std::vector<unsigned char> payload);
		void addTextRecord(std::string text);
        void addTextRecord(std::vector<unsigned char> text, std::string encoding = "us-ascii");
        void addUriRecord(std::string uri, UriType uritype);
        void addEmptyRecord();

        size_t getRecordCount() const { return m_records.size(); };
        std::vector<std::shared_ptr<NdefRecord> >& getRecords() { return m_records; };

        virtual void serialize(boost::property_tree::ptree& parentNode);
        virtual void unSerialize(boost::property_tree::ptree& node);
        virtual std::string getDefaultXmlNodeName() const;

        static std::shared_ptr<NdefMessage> TLVToNdefMessage(std::vector<unsigned char> tlv);

        static std::vector<unsigned char> NdefMessageToTLV(std::shared_ptr<NdefMessage> record);

    private:
        std::vector<std::shared_ptr<NdefRecord> > m_records;
    };
}

#endif