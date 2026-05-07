#include "perf_event.hpp"

#include <sys/syscall.h>
#include <unistd.h>

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

int PerfEvent::LinuxPerfSysCall::perf_event_open(const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags) noexcept
{
    return ::syscall(SYS_perf_event_open, attr, pid, cpu, group_fd, flags);
}
