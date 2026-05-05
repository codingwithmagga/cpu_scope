#pragma once

class PerfEvent
{
public:
    struct Config
    {
        int pid = 0;
        int cpu = -1;
    };

    static bool open(const Config& config) noexcept;
};