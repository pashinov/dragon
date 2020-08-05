//
// Created by Alexey Pashinov on 01/08/20
//

#pragma once

// system includes
#include <string>

namespace sys::sysinfo
{
    std::string os_name();
    std::string os_system_name();
    std::string os_release();
    std::string os_version();
    std::string os_machine();
}
