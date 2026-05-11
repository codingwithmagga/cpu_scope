#pragma once

#include <linux/perf_event.h>
#include <sys/types.h>

#include <cstdint>
#include <optional>
#include <system_error>

namespace cpuscope
{

struct PerfEventOpenResult;

class PerfEvent
{
public:
    enum class Scope
    {
        CPU,
        Process
    };

    enum class Event
    {
        CPU_CYCLES,
        HW_Instructions,
        HW_Cache_Misses
    };

    struct Config
    {
        pid_t pid = 0;
        int cpu = -1;
        Scope scope = Scope::CPU;
        Event event = Event::CPU_CYCLES;
    };

    struct ISysCalls
    {
        virtual ~ISysCalls() = default;
        virtual int perf_event_open(const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags) = 0;
        virtual int close(int file_descriptor) = 0;
        virtual ssize_t read(int file_descriptor, void* buf, size_t count) = 0;
    };

    struct LinuxSysCalls final : ISysCalls
    {
        int perf_event_open(const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags) noexcept override;
        int close(int file_descriptor) noexcept override;
        ssize_t read(int file_descriptor, void* buf, size_t count) noexcept override;
    };

    ~PerfEvent() noexcept;

    PerfEvent(const PerfEvent&) = delete;
    PerfEvent& operator=(const PerfEvent&) = delete;

    PerfEvent(PerfEvent&& other) noexcept;
    PerfEvent& operator=(PerfEvent&& other) noexcept;

    static PerfEventOpenResult open(const Config& config, ISysCalls& sysCalls) noexcept;
    std::optional<uint64_t> read_counter() noexcept;

private:
    PerfEvent(const int file_descriptor, ISysCalls& sysCalls) noexcept;

    int m_file_descriptor = -1;
    ISysCalls& m_sysCalls;
};

struct PerfEventError
{
    int code;
    std::string message;
};

struct PerfEventOpenResult
{
    std::optional<PerfEvent> event;
    PerfEventError error;

    bool has_error() const noexcept
    {
        return error.code != 0;
    }
};

}  // namespace cpuscope
