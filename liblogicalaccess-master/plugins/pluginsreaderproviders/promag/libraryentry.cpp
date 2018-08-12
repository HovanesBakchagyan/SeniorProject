#include <string>
#include <memory>
#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "promagreaderprovider.hpp"
#include "logicalaccess/logicalaccess_api.hpp"

extern "C"
{
    LIBLOGICALACCESS_API char *getLibraryName()
    {
        return (char *)"Promag";
    }

    LIBLOGICALACCESS_API void getPromagReader(std::shared_ptr<logicalaccess::ReaderProvider>* rp)
    {
        if (rp != NULL)
        {
            *rp = logicalaccess::PromagReaderProvider::getSingletonInstance();
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
                *getterfct = (void*)&getPromagReader;
                sprintf(readername, READER_PROMAG);
                ret = true;
            }
                break;
            }
        }

        return ret;
    }
}