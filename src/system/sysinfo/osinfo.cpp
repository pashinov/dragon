// system includes
#include <sys/utsname.h>

// project includes
#include <system/sysinfo/osinfo.hpp>

namespace sys::sysinfo
{
    std::string os_name()
    {
#ifdef __APPLE__
        return std::string("Mac OSX");
#elif __linux__
        return std::string("Linux");
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
