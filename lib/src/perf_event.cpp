#include "perf_event.hpp"

#include <sys/syscall.h>
#include <unistd.h>

#include <cstring>

PerfEvent::PerfEvent(int fd, ISysCalls& sysCalls) noexcept : m_fd(fd), m_sysCalls(sysCalls) {}

PerfEvent::~PerfEvent() noexcept
{
    if (m_fd >= 0)
    {
        m_sysCalls.close(m_fd);
    }
}

PerfEvent::PerfEvent(PerfEvent&& other) noexcept : m_fd(other.m_fd), m_sysCalls(other.m_sysCalls)
{
    other.m_fd = -1;
}

PerfEvent& PerfEvent::operator=(PerfEvent&& other) noexcept
{
    if (this != &other)
    {
        if (m_fd >= 0)
        {
            m_sysCalls.close(m_fd);
        }

        m_fd = other.m_fd;

        other.m_fd = -1;
    }

    return *this;
}

std::optional<PerfEvent> PerfEvent::open(const Config& config, ISysCalls& sysCalls) noexcept
{
    if ((config.scope == Scope::CPU && config.cpu < 0) || (config.scope == Scope::Process && config.pid < 0))
    {
        return std::nullopt;
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

    const int fd = sysCalls.perf_event_open(&attr, pid, cpu, -1, 0);
    if (fd < 0)
    {
        return std::nullopt;
    }

    return std::make_optional<PerfEvent>(PerfEvent(fd, sysCalls));
}

int PerfEvent::LinuxSysCalls::perf_event_open(const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags) noexcept
{
    return ::syscall(SYS_perf_event_open, attr, pid, cpu, group_fd, flags);
}

int PerfEvent::LinuxSysCalls::close(int fd) noexcept
{
    return ::close(fd);
}
