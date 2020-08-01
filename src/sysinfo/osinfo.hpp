//
// Created by apashinov on 01/08/20
//

#ifndef SYSINFO_OSINFO_HPP
#define SYSINFO_OSINFO_HPP

// system includes
#include <string>

namespace sysinfo
{
    std::string os_name();
    std::string os_system_name();
    std::string os_release();
    std::string os_version();
    std::string os_machine();
} // namespace sysinfo

#endif // SYSINFO_OSINFO_HPP
