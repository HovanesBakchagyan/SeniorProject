#include <string>
#include <memory>
#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "rfideasreaderprovider.hpp"
#include "logicalaccess/logicalaccess_api.hpp"

extern "C"
{
    LIBLOGICALACCESS_API char *getLibraryName()
    {
        return (char *)"RFIDeas";
    }

    LIBLOGICALACCESS_API void getRFIDeasReader(std::shared_ptr<logicalaccess::ReaderProvider>* rp)
    {
        if (rp != NULL)
        {
            *rp = logicalaccess::RFIDeasReaderProvider::getSingletonInstance();
        }
    }

    LIBLOGICALACCESS_API bool getReaderInfoAt(unsigned int index, char* readername, size_t readernamelen, void** getterfct)
    {
        bool ret = false;
        if (readername != NULL && readernamelen == PLUGINOBJECT_MAXLEN && getterfct != NULL)
        {
            switch (index)
            {
            case 0:
            {
                *getterfct = (void*)&getRFIDeasReader;
                sprintf(readername, READER_RFIDEAS);
                ret = true;
            }
                break;
            }
        }

        return ret;
    }
}