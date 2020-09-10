//
// Created by Alexey Pashinov on 10/09/20
//

#pragma once

// system includes
#include <string>
#include <optional>

namespace sys::sysinfo
{
    std::optional<std::string> get_uptime();
    std::optional<std::string> get_boot_time();
    std::optional<std::string> get_temperature();
    std::optional<std::string> get_os_info();
    std::optional<std::string> get_cpu_info();
    std::optional<std::string> get_disk_info();
}
