#include <string>
#include <memory>
#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "gigatmsreaderprovider.hpp"
#include "logicalaccess/logicalaccess_api.hpp"

extern "C"
{
    LIBLOGICALACCESS_API char *getLibraryName()
    {
        return (char *)"GigaTMS";
    }

    LIBLOGICALACCESS_API void getGigaTMSReader(std::shared_ptr<logicalaccess::ReaderProvider>* rp)
    {
        if (rp != NULL)
        {
            *rp = logicalaccess::GigaTMSReaderProvider::getSingletonInstance();
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
                *getterfct = (void*)&getGigaTMSReader;
                sprintf(readername, READER_GIGATMS);
                ret = true;
            }
                break;
            }
        }

        return ret;
    }
}