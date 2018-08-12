/**
 * \file admittoreaderprovider.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief Admitto reader provider.
 */

#include "admittoreaderprovider.hpp"
#include "logicalaccess/readerproviders/serialportdatatransport.hpp"
#include "logicalaccess/myexception.hpp"

#ifdef __unix__
#include <stdlib.h>
#include <sys/time.h>
#endif

#include <sstream>
#include <iomanip>

#include "admittoreaderunit.hpp"

namespace logicalaccess
{
    AdmittoReaderProvider::AdmittoReaderProvider() :
        ReaderProvider()
    {
    }

    AdmittoReaderProvider::~AdmittoReaderProvider()
    {
        release();
    }

    void AdmittoReaderProvider::release()
    {
    }

    std::shared_ptr<AdmittoReaderProvider> AdmittoReaderProvider::getSingletonInstance()
    {
        LOG(LogLevel::INFOS) << "Getting singleton instance...";
        static std::shared_ptr<AdmittoReaderProvider> instance;
        if (!instance)
        {
            instance.reset(new AdmittoReaderProvider());
            instance->refreshReaderList();
        }

        return instance;
    }

    std::shared_ptr<ReaderUnit> AdmittoReaderProvider::createReaderUnit()
    {
        LOG(LogLevel::INFOS) << "Creating new reader unit with empty port... (Serial port auto-detect will be used when connecting to reader)";

        std::shared_ptr<AdmittoReaderUnit> ret(new AdmittoReaderUnit());
        ret->setReaderProvider(std::weak_ptr<ReaderProvider>(shared_from_this()));

        return ret;
    }

    bool AdmittoReaderProvider::refreshReaderList()
    {
        d_readers.clear();

        std::vector<std::shared_ptr<SerialPortXml> > ports;
        EXCEPTION_ASSERT_WITH_LOG(SerialPortXml::EnumerateUsingCreateFile(ports), LibLogicalAccessException, "Can't enumerate the serial port list.");

        for (std::vector<std::shared_ptr<SerialPortXml> >::iterator i = ports.begin(); i != ports.end(); ++i)
        {
            std::shared_ptr<AdmittoReaderUnit> unit(new AdmittoReaderUnit());
            std::shared_ptr<SerialPortDataTransport> dataTransport = std::dynamic_pointer_cast<SerialPortDataTransport>(unit->getDataTransport());
            dataTransport->setSerialPort(*i);
            unit->setReaderProvider(std::weak_ptr<ReaderProvider>(shared_from_this()));
            d_readers.push_back(unit);
        }

        return true;
    }
}