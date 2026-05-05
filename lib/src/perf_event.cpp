#include "perf_event.hpp"

bool PerfEvent::open(const Config& config) noexcept
{
    if (config.pid < 0 || config.cpu < -1)
    {
        return false;
    }

    return true;
}