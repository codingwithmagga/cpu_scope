#include <iostream>
#include <vector>
#include <string_view>

#include "cpuscope_lib.hpp"

int main(int argc, char* argv[])
{
    std::vector<std::string_view> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    std::cout << cpuscope::format_message(args) << '\n';
    return 0;
}
