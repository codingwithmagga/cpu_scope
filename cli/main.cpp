#include <iostream>
#include <span>
#include <string_view>
#include <vector>

#include "cpuscope_lib.hpp"

int main(int argc, char* argv[])
{
    std::vector<std::string_view> args;
    args.reserve(argc);

    const std::span<char*> arguments(argv, argc);
    for (char* arg : arguments)
    {
        args.emplace_back(arg);
    }

    std::cout << cpuscope::format_message(args) << '\n';
    return 0;
}
