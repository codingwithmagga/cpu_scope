#include "cpu_profiler.hpp"

#include <cstdint>

using namespace cpuscope;

CpuProfiler::CpuProfiler(PerfEvent::ISysCalls& sysCalls) : m_sysCalls(sysCalls) {}

bool CpuProfiler::start()
{
    return true;
}

bool CpuProfiler::stop()
{
    return true;
}

std::optional<uint64_t> CpuProfiler::get_metrics() const
{
    uint64_t buffer = 0;
    return m_sysCalls.read(42, &buffer, sizeof(uint64_t)) == sizeof(uint64_t) ? std::make_optional<uint64_t>(buffer) : std::nullopt;
}