#pragma once

#include <format>
#include <span>
#include <string>
#include <string_view>

namespace cpuscope
{

inline std::string format_message(std::span<const std::string_view> args)
{
    return std::format("CPUScope CLI placeholder: {} arguments received.", args.size());
}

}  // namespace cpuscope
