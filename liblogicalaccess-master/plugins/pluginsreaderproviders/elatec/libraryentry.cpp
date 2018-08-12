#include <string>
#include <memory>
#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "elatecreaderprovider.hpp"
#include "logicalaccess/logicalaccess_api.hpp"

extern "C"
{
    LIBLOGICALACCESS_API char *getLibraryName()
    {
        return (char *)"Elatec";
    }

    LIBLOGICALACCESS_API void getElatecReader(std::shared_ptr<logicalaccess::ReaderProvider>* rp)
    {
        if (rp != NULL)
        {
            *rp = logicalaccess::ElatecReaderProvider::getSingletonInstance();
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
                *getterfct = (void*)&getElatecReader;
                sprintf(readername, READER_ELATEC);
                ret = true;
            }
                break;
            }
        }

        return ret;
    }
}