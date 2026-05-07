#include "perf_event.hpp"

bool PerfEvent::open(const Config& config, IPerfSysCall& perfSysCall) noexcept
{
    if (config.pid < 0 || config.cpu < -1)
    {
        return false;
    }

    if (perfSysCall.perf_event_open(nullptr, config.pid, config.cpu, -1, 0) < 0)
    {
        return false;
    }

    return true;
}