#pragma once

#include <cstdint>
#include <optional>

namespace cpuscope
{
class CounterReader
{
public:
    CounterReader() = default;

    bool start();
    bool stop();

    std::optional<uint64_t> get_metrics() const;
};
}  // namespace cpuscope