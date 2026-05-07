#pragma once

#include <linux/perf_event.h>
#include <sys/types.h>

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

    struct IPerfSysCall
    {
        virtual ~IPerfSysCall() = default;
        virtual int perf_event_open(const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags) = 0;
    };

    struct LinuxPerfSysCall final : IPerfSysCall
    {
        int perf_event_open(const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags) noexcept override;
    };

    static bool open(const Config& config, IPerfSysCall& perfSysCall) noexcept;
};