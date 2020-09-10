//
// Created by Alexey Pashinov on 10/09/20
//

#pragma once

// system includes
#include <optional>
#include <string>
#include <variant>

// proto
#include <lion.pb.h>

namespace sys::sysinfo
{
    std::optional<std::variant<bool, double, std::string, std::uint32_t, std::int32_t>> get(lion_protocol::SysInfoType type);
}
