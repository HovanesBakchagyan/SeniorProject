#include <string>
#include <memory>
#include "logicalaccess/readerproviders/readerprovider.hpp"
#include "stidstrreaderprovider.hpp"
#include "logicalaccess/logicalaccess_api.hpp"

extern "C"
{
    LIBLOGICALACCESS_API char *getLibraryName()
    {
        return (char *)"STidSTR";
    }

    LIBLOGICALACCESS_API void getSTidSTRReader(std::shared_ptr<logicalaccess::STidSTRReaderProvider>* rp)
    {
        if (rp != NULL)
        {
            *rp = logicalaccess::STidSTRReaderProvider::getSingletonInstance();
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
                *getterfct = (void*)&getSTidSTRReader;
                sprintf(readername, READER_STIDSTR);
                ret = true;
            }
                break;
            }
        }

        return ret;
    }
}