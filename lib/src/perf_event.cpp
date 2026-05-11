#include "perf_event.hpp"

#include <linux/perf_event.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <optional>

namespace cpuscope
{

PerfEvent::PerfEvent(const int file_descriptor, ISysCalls& sysCalls) noexcept : m_file_descriptor(file_descriptor), m_sysCalls(sysCalls) {}

PerfEvent::~PerfEvent() noexcept
{
    if (m_file_descriptor >= 0)
    {
        m_sysCalls.close(m_file_descriptor);
    }
}

PerfEvent::PerfEvent(PerfEvent&& other) noexcept : m_file_descriptor(other.m_file_descriptor), m_sysCalls(other.m_sysCalls)
{
    other.m_file_descriptor = -1;
}

PerfEvent& PerfEvent::operator=(PerfEvent&& other) noexcept
{
    if (this != &other)
    {
        if (m_file_descriptor >= 0)
        {
            m_sysCalls.close(m_file_descriptor);
        }

        m_file_descriptor = other.m_file_descriptor;

        other.m_file_descriptor = -1;
    }

    return *this;
}

PerfEventOpenResult PerfEvent::open(const Config& config, ISysCalls& sysCalls) noexcept
{
    if ((config.scope == Scope::CPU && config.cpu < 0) || (config.scope == Scope::Process && config.pid < 0))
    {
        return PerfEventOpenResult{std::nullopt, PerfEventError{EINVAL, "Invalid configuration: CPU or PID must be set based on the scope"}};
    }

    const pid_t pid = Scope::CPU == config.scope ? -1 : config.pid;
    const int cpu = Scope::CPU == config.scope ? config.cpu : -1;

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

    const auto file_descriptor = sysCalls.perf_event_open(&attr, pid, cpu, -1, 0);
    if (file_descriptor < 0)
    {
        return PerfEventOpenResult{std::nullopt, PerfEventError{errno, "Failed to open perf event"}};
    }

    return PerfEventOpenResult{std::make_optional<PerfEvent>(PerfEvent(file_descriptor, sysCalls)), PerfEventError{0, ""}};
}

std::optional<uint64_t> PerfEvent::read_counter() noexcept
{
    if (m_file_descriptor < 0)
    {
        return std::nullopt;
    }

    uint64_t value = 0;
    const ssize_t bytesRead = m_sysCalls.read(m_file_descriptor, &value, sizeof(value));
    if (bytesRead != static_cast<ssize_t>(sizeof(value)))
    {
        return std::nullopt;
    }

    return value;
}

int PerfEvent::LinuxSysCalls::perf_event_open(const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags) noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    return static_cast<int>(::syscall(SYS_perf_event_open, attr, pid, cpu, group_fd, flags));
}

int PerfEvent::LinuxSysCalls::close(int file_descriptor) noexcept
{
    return ::close(file_descriptor);
}

ssize_t PerfEvent::LinuxSysCalls::read(int file_descriptor, void* buf, size_t count) noexcept
{
    return ::read(file_descriptor, buf, count);
}

}  // namespace cpuscope