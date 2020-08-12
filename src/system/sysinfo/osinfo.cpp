// system includes
#include <sys/utsname.h>

// project includes
#include <system/sysinfo/osinfo.hpp>

namespace sys::sysinfo
{
    std::string os_name()
    {
#ifdef _WIN32
        return std::string("Windows 32-bit");
#elif _WIN64
        return std::string("Windows 64-bit");
#elif __APPLE__
        return std::string("Mac OSX");
#elif __linux__
        return std::string("Linux");
#elif __unix__
        return std::string("Unix");
#elif __FreeBSD__
        return std::string("FreeBSD");
#else
        return std::string("Other");
#endif
    }

    std::string os_system_name()
    {
        struct utsname uname_data{};
        uname(&uname_data);

        return std::string(uname_data.sysname);
    }

    std::string os_release()
    {
        struct utsname uname_data{};
        uname(&uname_data);

        return std::string(uname_data.release);
    }

    std::string os_machine()
    {
        struct utsname uname_data{};
        uname(&uname_data);

        return std::string(uname_data.machine);
    }
}
