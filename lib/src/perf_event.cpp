#include "perf_event.hpp"

#include <sys/syscall.h>
#include <unistd.h>

#include <cstring>

bool PerfEvent::open(const Config& config, IPerfSysCall& perfSysCall) noexcept
{
    if ((config.scope == Scope::CPU && config.cpu < 0) || (config.scope == Scope::Process && config.pid < 0))
    {
        return false;
    }

    pid_t pid = Scope::CPU == config.scope ? -1 : config.pid;
    int cpu = Scope::CPU == config.scope ? config.cpu : -1;

    perf_event_attr attr{};
    std::memset(&attr, 0, sizeof(attr));
    attr.type = PERF_TYPE_HARDWARE;
    attr.size = sizeof(attr);

    switch (config.event)
    {
        case Event::CPU_CYCLES:
            attr.config = PERF_COUNT_HW_CPU_CYCLES;
            break;
        case Event::HW_Instructions:
            attr.config = PERF_COUNT_HW_INSTRUCTIONS;
            break;
        case Event::HW_Cache_Misses:
            attr.config = PERF_COUNT_HW_CACHE_MISSES;
            break;
    }

    if (perfSysCall.perf_event_open(&attr, pid, cpu, -1, 0) < 0)
    {
        return false;
    }

    return true;
}

int PerfEvent::LinuxPerfSysCall::perf_event_open(const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags) noexcept
{
    return ::syscall(SYS_perf_event_open, attr, pid, cpu, group_fd, flags);
}
