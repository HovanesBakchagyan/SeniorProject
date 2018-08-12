#pragma once

#ifdef _WIN32

#include "logicalaccess/logicalaccess_api.hpp"
#include <string>
#include <vector>
#include <cstdint>

namespace logicalaccess
{
    /**
     * This class is a static helper to interact with the Windows registry.
     *
     * Currently this helper is usable only for keys in the
     * HKEY_LOCAL_MACHINE namespace.
     */
    class LIBLOGICALACCESS_API WindowsRegistry
    {
      public:
        WindowsRegistry();
        ~WindowsRegistry();

        static bool writeBinary(const std::string &keypath,
                                const std::string &keyname,
                                const std::vector<uint8_t> &data);

        static bool readBinary(const std::string &keypath,
                               const std::string &keyname,
                               std::vector<uint8_t> &out);
    };
}

#endif
