#pragma once

#include <cstdint>
#include <optional>

#include "perf_event.hpp"

namespace cpuscope
{
class CpuProfiler
{
public:
    CpuProfiler(PerfEvent::ISysCalls& sysCalls);

    bool start();
    bool stop();

    std::optional<uint64_t> get_metrics() const;

private:
    PerfEvent::ISysCalls& m_sysCalls;
};
}  // namespace cpuscope