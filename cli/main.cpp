#include <cstring>
#include <iostream>
#include <span>
#include <string_view>
#include <utility>

#include "perf_event.hpp"

using namespace cpuscope;

static constexpr std::string_view VERSION = "0.1.0";

int main(int argc, char** argv)
{
    // Simplified parsing of command-line arguments for demonstration purposes
    // Later add a cli lib like CLI11 or cxxopts for more robust argument parsing

    std::span args{argv, static_cast<size_t>(argc)};

    for (auto arg : args)
    {
        if (std::strcmp(arg, "--version") == 0)
        {
            std::cout << "cpu_scope version " << VERSION << '\n';
            return 0;
        }
    }

    PerfEvent::LinuxSysCalls sysCalls;
    PerfEvent::Config config;
    config.scope = PerfEvent::Scope::CPU;
    config.cpu = 1;
    config.event = PerfEvent::Event::CPU_CYCLES;

    auto perfEventOpt = PerfEvent::open(config, sysCalls);
    if (perfEventOpt.has_error() || !perfEventOpt.event.has_value())
    {
        std::cerr << "Error: " << perfEventOpt.error.message << " " << perfEventOpt.error.code << '\n';
        return 1;
    }

    auto perfEvent = std::move(perfEventOpt.event.value());
    auto counterOpt = perfEvent.read_counter();
    if (!counterOpt)
    {
        std::cerr << "Failed to read perf event counter\n";
        return 1;
    }

    std::cout << "Counter value: " << counterOpt.value() << '\n';

    return 0;
}
