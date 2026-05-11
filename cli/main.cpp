#include <iostream>
#include <utility>

#include "perf_event.hpp"

using namespace cpuscope;

int main(int /*argc*/, char** /*argv[]*/)
{
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
